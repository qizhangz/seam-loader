// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file error_reporting.c
 * @brief Runtime error reporting features for TDX module
 */

#include "accessors/ia32_accessors.h"
#include "accessors/data_accessors.h"
#include "error_reporting.h"
#include "pseamldr_basic_defs.h"
#include "pseamldr_basic_types.h"

void pseamldr_report_error_and_halt(uint32_t source_id, uint32_t code)
{
    uint64_t error_code = ERROR_CODE(source_id, code);

    TDX_ERROR("Error 0x%llx reported by the TDX Module\n", error_code);
    TDX_ERROR("Module shutdown initiated - UD2 expected\n");

    UNUSED(error_code);

    pseamldr_arch_fatal_error();
}

void pseamldr_arch_fatal_error( void )
{
#ifdef DEBUGFEATURE_TDX_DBG_TRACE
    TDX_ERROR("!!!!!!!!!!!!!!!!!! - Module critical error - UD2 - !!!!!!!!!!!!!!!!!!\n");
    TDX_ERROR("!!!!!!!!!!!!!!!!!!         - LAST MESSAGE -        !!!!!!!!!!!!!!!!!!\n");
    debug_control_t* p_ctl = &(get_pseamldr_data()->debug_control);
    dump_print_buffer_to_vmm_memory(p_ctl->emergency_buffer, 0);
#endif

    ia32_ud2();
}



