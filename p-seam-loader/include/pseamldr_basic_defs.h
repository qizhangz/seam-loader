// Intel Proprietary 
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file pseamldr_basic_defs.h
 * @brief PSEAMLDR Basic Definitions
 */

#ifndef SRC_COMMON_PSEAMLDR_BASIC_DEFS_H_
#define SRC_COMMON_PSEAMLDR_BASIC_DEFS_H_

#define _STATIC_INLINE_ static inline
#define _ASM_VOLATILE_ __asm__ volatile
#define _ASM_          __asm__


#ifndef NULL
#define NULL            ((void*)0)
#endif

//Static Assertion
#define pseamldr_static_assert(e,x) typedef char assertion_##x  [(e)?1:-1]

#define _1KB                    0x400ULL
#define _2KB                    (2 * _1KB)
#define _4KB                    (4 * _1KB)
#define _1MB                    (_1KB * _1KB)
#define _2MB                    (2 * _1MB)
#define _1GB                    (_1MB * _1KB)

#define BIT_MASK_8BITS        0xFFULL
#define BIT_MASK_16BITS       0xFFFFULL
#define BIT_MASK_32BITS       0xFFFFFFFFULL
#define BIT_MASK_64BITS       0xFFFFFFFFFFFFFFFFULL

#define HIGH_32BITS(a)        (((a) >> 32) & BIT_MASK_32BITS)
#define LOW_32BITS(a)         ((a) & BIT_MASK_32BITS)

#define PAGE_SIZE_IN_BYTES    _4KB

#define PG_OFFSET(a)            ((a) & 0xFFFLLU)
#define PG_START(a)             ((a) & ~0xFFFLLU)

#ifndef offsetof
#define offsetof(type, field)   (uint64_t)&(((type *)0)->field)
#endif

#define PACKED                  __attribute__((__packed__))

#define RARE_COND(cond)             __builtin_expect((cond), 0)
#define COMMON_COND(cond)           __builtin_expect((cond), 1)
#define IF_RARE(rare_condition)     if ( RARE_COND(rare_condition) )
#define IF_COMMON(common_condition) if ( COMMON_COND(common_condition) )


#endif /* SRC_COMMON_PSEAMLDR_BASIC_DEFS_H_ */
