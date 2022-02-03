// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file seamldr_info.c
 * @brief SEAMLDR.SEAMINFO API handler
 */
#include "../../../include/pseamldr_basic_defs.h"
#include "pseamldr_api_handlers.h"
#include "x86_defs/x86_defs.h"
#include "memory_handlers/keyhole_manager.h"
#include "helpers/helpers.h"
#include "accessors/data_accessors.h"
#include "accessors/ia32_accessors.h"

api_error_type seamldr_seaminfo(uint64_t output_buffer_pa)
{
    uint8_t*           out_seaminfo_la = NULL;
    p_sysinfo_table_t* p_sysinfo_table = get_psysinfo_table();
    api_error_type     return_value = PSEAMLDR_EUNSPECERR;

    //Check SEAMLDR SEAMINFO output PA
    return_value = check_and_map_aligned_shared_hpa((pa_t)output_buffer_pa, _2KB, TDX_RANGE_RW,
                                                    (void**)&out_seaminfo_la);
    IF_RARE (return_value != PSEAMLDR_SUCCESS)
    {
        TDX_ERROR("Output PA is not a valid/aligned shared HPA pa=0x%llx\n", output_buffer_pa);
        goto EXIT;
    }

    pseamldr_memcpy(out_seaminfo_la, _2KB, p_sysinfo_table, _2KB);

    return_value = PSEAMLDR_SUCCESS;

EXIT:

    IF_COMMON (out_seaminfo_la != NULL)
    {
        free_la(out_seaminfo_la);
    }

    return return_value;
}
