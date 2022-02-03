// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file sha384.c
 * @brief Crypto implementation of SHA384
 */
#include "crypto/sha384.h"
#include "helpers/helpers.h"
#include "ippcp.h"

crypto_api_error sha384_generate_hash(const uint8_t * block,
                                      uint32_t block_size,
                                      uint64_t * hash)
{
    int32_t method_buffer_size;
    uint8_t hash_method_buff[HASH_METHOD_BUFFER_SIZE];
    IppStatus ret_val = ippStsErr;

    ret_val = ippsHashMethodGetSize(&method_buffer_size);

    if (ret_val != ippStsNoErr || method_buffer_size > (int32_t)sizeof(hash_method_buff))
    {
        TDX_ERROR("Required method buffer size is %d\n", method_buffer_size);
        return ippStsErr;
    }

    IppsHashMethod* hash_method_ptr = (IppsHashMethod*)hash_method_buff;
    ret_val = ippsHashMethodSet_SHA384(hash_method_ptr);
    if (ret_val != ippStsNoErr)
    {
        TDX_ERROR("SHA384 Method setting failed\n");
        return ippStsErr;
    }

    return ippsHashMessage_rmf(block, (int)block_size, (Ipp8u*)hash, hash_method_ptr);
}

