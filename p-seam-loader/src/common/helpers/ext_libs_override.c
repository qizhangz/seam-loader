// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file ext_libs_override.c
 * @brief  Override functions from external libs
 */

#include "helpers/ext_libs_override.h"
#include "helpers/error_reporting.h"

void __wrap___stack_chk_fail(void)
{
    pseamldr_sanity_check(false, SCEC_CANARY_CORRUPT_SOURCE, 0);
}
