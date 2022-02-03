// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file pseamldr_vmm_api_handelrs.h
 * @brief PSEAMLDR VMM API Handelrs
 */
#ifndef __PSEAMLDR_VMM_API_HANDLERS_H_INCLUDED__
#define __PSEAMLDR_VMM_API_HANDLERS_H_INCLUDED__


#include "pseamldr_api_defs.h"

/**
 * @brief Copies the low 2KB (MCHECK info) of P_SYS_INFO_TABLE to the output buffer
 *
 * @param output_buffer_pa PA of output buffer to be filled with SEAM information data
 *
 * @return Success or Error type
 */
api_error_type seamldr_seaminfo(uint64_t output_buffer_pa);

/**
 * @brief Provide information about P-SEAMLDR.
 *
 * @param output_buffer_pa PA of output buffer to be filled with SEAMLDR_INFO data
 *
 * @return Success or Error type
 */
api_error_type seamldr_info(uint64_t output_buffer_pa);


/**
 * @brief Loads or updates a given SEAM module in SEAM range.
 *
 * Returns Success (0) or error code in RAX.
 * In case of error, returns recoverable (0) or unrecoverable (1) status in RDX.
 * This API should be called on all LPs serially – the first invocation in the “update session” blocks further
 * SEAMCALLs to the SEAM module; the last invocation performs the update according to the given SEAMLDR_PARAMS.
 * On load/update success, the new SEAM module becomes executable.
 * On recoverable error (update scenario only), the old SEAM module is restored.
 * Unrecoverable errors make the old SEAM module unusable.
 * After the “update session” was completed (either successfully or unsuccessfully), another update session can start.
 *
 * @param seamldr_params_pa PA of SEAMLDR_PARAMS structure
 *
 * @return Success or Error type
 */
api_error_type seamldr_install(uint64_t seamldr_params_pa);


/**
 * @brief Shutdowns P-SEAMLDR.
 *
 * This API is used to prepare for P-SEAMLDR update by NP-SEAMLDR ACM.
 * It should be called on all LPs serially, such that the last LP on which it’s called is
 * the same LP on which NP-SEAMLDR was launched.
 * The first invocation of the “shutdown session” blocks further SEAMCALLs to the SEAM module;
 * the last invocation also blocks further SEAMCALLs to the P-SEAMLDR itself.
 *
 * @return Success or Error type
 */
api_error_type seamldr_shutdown(void);

#endif // __PSEAMLDR_VMM_API_HANDLERS_H_INCLUDED__
