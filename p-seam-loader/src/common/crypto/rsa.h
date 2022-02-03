// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file rsa.h
 * @brief Crypto API for RSA verification
 */

#ifndef SRC_COMMON_CRYPTO_RSA_H_
#define SRC_COMMON_CRYPTO_RSA_H_

#include "../../../include/pseamldr_basic_defs.h"
#include "../../../include/pseamldr_basic_types.h"
#include "sha384.h"

#define SIZE_OF_RSA_SIGNATURE      384
#define SIZE_OF_RSA_CTX_BUFFER     0x2000

/**
 * @struct rsa_ctx_t
 *
 * @brief Context of an RSA process.
 */
typedef struct rsa_ctx_s
{
    uint8_t buffer[SIZE_OF_RSA_CTX_BUFFER];
} rsa_ctx_t;

crypto_api_error rsa_verify_signature(uint8_t* signature, const uint8_t* message,
                                      uint32_t message_len, uint32_t public_exp,
                                      const uint8_t* modulus, bool_t* is_valid);

#endif /* SRC_COMMON_CRYPTO_RSA_H_ */
