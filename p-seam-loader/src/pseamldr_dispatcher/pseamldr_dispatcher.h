// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file pseamldr_dispatcher.h
 * @brief VMM entry point and API dispatcher
 */
#ifndef __PSEAMLDR_DISPATCHER_H_INCLUDED__
#define __PSEAMLDR_DISPATCHER_H_INCLUDED__


#include "../../include/pseamldr_basic_defs.h"
#include "../../include/pseamldr_basic_types.h"


/**
 * @brief Entry point to PSEAMLDR module from VMM
 *
 * @note Written in assembly and defined as the binary's entry point
 *
 * @return None
 */
void pseamldr_entry_point(void);


/**
 * @brief Dispatcher for VMM side API calls
 *
 * @note
 *
 * @return None
 */
void pseamldr_dispatcher(void);


/**
 * @brief Return sequence from the module to the VMM
 *
 * Restores VMM state and eventually calls SEAMRET
 *
 * @return None
 */
void pseamldr_post_dispatching(void);


/**
 * @brief Exit the module
 *
 * @note Written in assembly
 *
 * @return None
 */
__attribute__((visibility("hidden"))) void pseamldr_exit_point(void);


#endif // __PSEAMLDR_DISPATCHER_H_INCLUDED__
