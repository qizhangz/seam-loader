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
 * @brief SEAMLDR.INFO API handler
 */
#include "../../../include/pseamldr_basic_defs.h"
#include "pseamldr_api_handlers.h"
#include "x86_defs/x86_defs.h"
#include "memory_handlers/keyhole_manager.h"
#include "helpers/helpers.h"
#include "accessors/data_accessors.h"
#include "accessors/ia32_accessors.h"

api_error_type seamldr_info(uint64_t output_buffer_pa)
{
    seamldr_info_t              * out_seamldr_info_la = NULL;
    ALIGN(256) seamextend_t       seamextend;
    api_error_type                return_value = PSEAMLDR_EUNSPECERR;

    //Check SEAMLDR INFO output PA
    return_value = check_and_map_aligned_shared_hpa((pa_t)output_buffer_pa, 256, TDX_RANGE_RW, (void**)&out_seamldr_info_la);
    IF_RARE (return_value != PSEAMLDR_SUCCESS)
    {
        TDX_ERROR("Output PA is not a valid/aligned shared HPA pa=0x%llx\n", output_buffer_pa);
        goto EXIT;
    }

    basic_memset_to_zero(out_seamldr_info_la, sizeof(seamldr_info_t));

    out_seamldr_info_la->version = 0;
#ifdef DEBUG
    out_seamldr_info_la->attributes.is_debug = 1;
#endif
    out_seamldr_info_la->vendor_id  = 0x8086;
    out_seamldr_info_la->build_date = PSEAMLDR_BUILD_DATE;
    out_seamldr_info_la->build_num  = PSEAMLDR_BUILD_NUM;
    out_seamldr_info_la->minor      = PSEAMLDR_MINOR_VER;
    out_seamldr_info_la->major      = PSEAMLDR_MAJOR_VER;
    out_seamldr_info_la->acm_x2apic = get_psysinfo_table()->acm_x2apicid;

    seamextend_read(&seamextend);

    pseamldr_memcpy(&out_seamldr_info_la->seamextend, sizeof(seamextend_t), &seamextend, sizeof(seamextend_t));

    return_value = PSEAMLDR_SUCCESS;

EXIT:

    IF_COMMON (out_seamldr_info_la != NULL)
    {
        free_la(out_seamldr_info_la);
    }

    return return_value;
}
