// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file seam_sigstruct.h
 * @brief SEAM signature structure
 */
#ifndef INCLUDE_SEAM_SIGSTRUCT_H_
#define INCLUDE_SEAM_SIGSTRUCT_H_

#define SIGSTRUCT_MODULUS_SIZE 384
#define SIGSTRUCT_SIGNATURE_SIZE 384
#define SIGSTRUCT_SEAMHASH_SIZE 48

#include "pseamldr_basic_types.h"
#include "debug/tdx_debug.h"

#pragma pack(push,1)

#define SEAM_SIGSTRUCT_KEY_SIZE_DWORDS        0x60
#define SEAM_SIGSTRUCT_MODULUS_SIZE_DWORDS    0x60
#define SEAM_SIGSTRUCT_EXPONENT_SIZE_DWORDS   0x1
#define SEAM_SIGSTRUCT_RSA_EXPONENT           0x10001 // (2^16 + 1)
#define SEAM_SIGSTRUCT_HEADER_TYPE_GENERIC_FW 0x6
#define SEAM_SIGSTRUCT_HEADER_LENGTH_DWORDS   0xE1
#define SEAM_SIGSTRUCT_HEADER_VERSION_MINOR   0x0UL
#define SEAM_SIGSTRUCT_HEADER_VERSION_MAJOR   0x1UL
#define SEAM_SIGSTRUCT_HEADER_VERSION         ((SEAM_SIGSTRUCT_HEADER_VERSION_MAJOR << 16) | \
                                                SEAM_SIGSTRUCT_HEADER_VERSION_MINOR)
#define SEAM_SIGSTRUCT_SIZE_DWORDS            0x200

#define SEAM_SIGSTRUCT_INTEL_MODULE_VENDOR    0x8086
#define SEAM_SIGSTRUCT_MAX_CPUID_TABLE_SIZE   255

typedef union
{
    struct
    {
        uint32_t reserved        : 31;
        uint32_t is_debug_signed : 1;
    };

    uint32_t raw;
} module_type_t;

#define SEAM_SIGSTRUCT_SIZE            2048
#define SEAM_SIGSTRUCT_HEADER_SIZE     128
#define SEAM_SIGSTRUCT_SIG_OFFSET      SEAM_SIGSTRUCT_HEADER_SIZE
#define SEAM_SIGSTRUCT_SIG_SIZE        (384+4+384)
#define SEAM_SIGSTRUCT_BODY_OFFSET     (SEAM_SIGSTRUCT_SIG_OFFSET + SEAM_SIGSTRUCT_SIG_SIZE)
#define SEAM_SIGSTRUCT_BODY_SIZE       (SEAM_SIGSTRUCT_SIZE - SEAM_SIGSTRUCT_HEADER_SIZE - SEAM_SIGSTRUCT_SIG_SIZE)

#if ((SEAM_SIGSTRUCT_BODY_OFFSET + SEAM_SIGSTRUCT_BODY_SIZE) != SEAM_SIGSTRUCT_SIZE)
#error "Wrong SEAM SIGSTRUCT size constants!!!"
#endif

typedef struct
{
    uint32_t header_type;
    uint32_t header_length;
    uint32_t header_version;
    module_type_t module_type;
    uint32_t module_vendor;
    uint32_t date;
    uint32_t size;
    uint32_t key_size;
    uint32_t modulus_size;
    uint32_t exponent_size;
    uint8_t reserved0[88];

    uint8_t modulus[SIGSTRUCT_MODULUS_SIZE];
    uint32_t exponent;
    uint8_t signature[SIGSTRUCT_SIGNATURE_SIZE];

    uint8_t seamhash[SIGSTRUCT_SEAMHASH_SIZE];
    uint16_t seamsvn;
    uint64_t attributes;
    uint32_t rip_offset;
    uint8_t num_stack_pages;
    uint8_t num_tls_pages;
    uint16_t num_keyhole_pages;
    uint16_t num_global_data_pages;
    uint16_t max_tdmrs;
    uint16_t max_rsvd_per_tdmr;
    uint16_t pamt_entry_size_4k;
    uint16_t pamt_entry_size_2m;
    uint16_t pamt_entry_size_1g;
    uint8_t reserved1[46];

    uint32_t cpuid_table_size;
    uint32_t cpuid_table[SEAM_SIGSTRUCT_MAX_CPUID_TABLE_SIZE];

} seam_sigstruct_t;

#pragma pack(pop)

pseamldr_static_assert(sizeof(seam_sigstruct_t) == SEAM_SIGSTRUCT_SIZE, seam_sigstruct_t);
pseamldr_static_assert(offsetof(seam_sigstruct_t, modulus) == SEAM_SIGSTRUCT_SIG_OFFSET, seam_sigstruct_t);
pseamldr_static_assert(offsetof(seam_sigstruct_t, seamhash) == SEAM_SIGSTRUCT_BODY_OFFSET, seam_sigstruct_t);


#endif /* INCLUDE_SEAM_SIGSTRUCT_H_ */
