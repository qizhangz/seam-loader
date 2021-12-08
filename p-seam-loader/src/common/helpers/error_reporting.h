// Intel Proprietary 
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file error_reporting.h
 * @brief Runtime error reporting features for TDX module
 */

#ifndef SRC_COMMON_HELPERS_ERROR_REPORTING_H_
#define SRC_COMMON_HELPERS_ERROR_REPORTING_H_

#include "debug/tdx_debug.h"

void pseamldr_report_error_and_halt(uint32_t source_id, uint32_t code);

void pseamldr_arch_fatal_error(void);

//Architectural Fatal Error Macro.
#define FATAL_ERROR()       {\
                                TDX_ERROR("Architectural fatal error at line: %d , in file %s\n", __LINE__, __FILENAME__);\
                                pseamldr_arch_fatal_error();\
                            };

//Runtime (includes product-build) Assertion
#define pseamldr_sanity_check(cond, source_id, code) if (!(cond)) {\
                                                      TDX_ERROR("Runtime panic at line: %d , in file %s\n", __LINE__, __FILENAME__);\
                                                      pseamldr_report_error_and_halt(source_id, code);\
                                                     };


// SCEC - Sanity Check Error Code
#define SCEC_HELPERS_SOURCE            0xF002
#define SCEC_KEYHOLE_MANAGER_SOURCE    0xF005
#define SCEC_VT_ACCESSORS_SOURCE       0xF006
#define SCEC_VMM_DISPATCHER_SOURCE     0xF008
#define SCEC_CANARY_CORRUPT_SOURCE     0xCF00


#define ERROR_CODE(source_id, code)    (uint64_t)(((uint64_t)(source_id) << 32U) | (uint64_t)(code))

#endif /* SRC_COMMON_HELPERS_ERROR_REPORTING_H_ */
