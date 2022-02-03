// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file ext_libs_override.h
 * @brief Override functions from external libs
 */

#ifndef SRC_COMMON_EXT_LIBS_OVERRIDE_H_
#define SRC_COMMON_EXT_LIBS_OVERRIDE_H_

/**
 * @brief overrides standard lib canary corruption function call
 */
void __wrap___stack_chk_fail(void);


#endif // SRC_COMMON_EXT_LIBS_OVERRIDE_H_
