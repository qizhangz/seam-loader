// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file seamldr_shutdown.c
 * @brief SEAMLDR.SHUTDOWN API handler
 */
#include "../../../include/pseamldr_basic_defs.h"
#include "pseamldr_api_handlers.h"
#include "x86_defs/x86_defs.h"
#include "memory_handlers/keyhole_manager.h"
#include "helpers/helpers.h"
#include "accessors/data_accessors.h"
#include "accessors/ia32_accessors.h"

api_error_type seamldr_shutdown(void)
{
    pseamldr_data_t             * pseamldr_data = get_pseamldr_data();
    p_sysinfo_table_t           * p_sysinfo_table = get_psysinfo_table();
    uint32_t                      lpid;
    api_error_type                return_value = PSEAMLDR_EUNSPECERR;

    lpid = get_current_lpid();

    // If X2APICID = CPUID.1FH.0:EDX >= MIN_LP_BITMAP_SIZE, then fatal error
    IF_RARE (lpid >= MAX_NUM_OF_LPS)
    {
        TDX_ERROR("Incorrect LPID=%d, bigger than defined max num of LP's=%d\n", lpid, MAX_NUM_OF_LPS);
        return_value = PSEAMLDR_EUNSPECERR;
        goto EXIT;
    }

    // Mark this LP as invoked in the current update session (i.e. set SHUTDOWN_BITMAP[X2APICID] = 1).
    IF_COMMON (bit_test_and_set(pseamldr_data->shutdown_bitmap, lpid) == 0)
    {
        pseamldr_data->lps_in_shutdown++;
    }

    // If this is the first LP on which this API is invoked (i.e. bitcount(SHUTDOWN_BITMAP) == 1),
    // then clear the SEAM_READY field in the SEAMEXTEND register (using IA32_SEAMEXTEND copy-out and copy-in commands).
    // This blocks further SEAMCALLs to the SEAM module.
    IF_RARE (pseamldr_data->lps_in_shutdown == 1)
    {
        seamextend_read(&pseamldr_data->seamextend_snapshot);
        pseamldr_data->seamextend_snapshot.seam_ready = 0;
        seamextend_write(&pseamldr_data->seamextend_snapshot);
    }

    // If this is not the last LP on which this API is invoked then set RAX = 0 and return.
    IF_COMMON (pseamldr_data->lps_in_shutdown < p_sysinfo_table->tot_num_lps)
    {
        return_value = PSEAMLDR_SUCCESS;
        goto EXIT;
    }

    // The following steps apply only to the “last” LP:

    // If this LP is not the LP on which NP-SEAMLDR ACM was launched
    // (i.e. P_SYS_INFO_TABLE.LAST_SHUTDOWN_X2APICID != X2APICID), then set RAX = EBADCALL and return.
    IF_RARE (lpid != p_sysinfo_table->acm_x2apicid)
    {
        TDX_ERROR("LPID=%d Is different than last shutdown LPID=%d\n", lpid, p_sysinfo_table->acm_x2apicid);
        return_value = PSEAMLDR_EBADCALL;
        goto EXIT;
    }

    // Clear the SEAMEXTEND register (using IA32_SEAMEXTEND copy-in command). This blocks further SEAMCALLs to P-SEAMLDR.
    basic_memset_to_zero(&pseamldr_data->seamextend_snapshot, sizeof(seamextend_t));
    seamextend_write(&pseamldr_data->seamextend_snapshot);

    return_value = PSEAMLDR_SUCCESS;

EXIT:

    return return_value;
}
