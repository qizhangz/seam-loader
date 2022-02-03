// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file pseamldr_basic_types.h
 * @brief PSEAMLDR basic types definitions
 */
#ifndef __PSEAMLDR_BASIC_TYPES_H_INCLUDED__
#define __PSEAMLDR_BASIC_TYPES_H_INCLUDED__

////////////////////////////////////////////////////////////////////////////
// ANSI C18 Compliant data types.
////////////////////////////////////////////////////////////////////////////
#if (!defined(__cplusplus) || defined(CTE_BUILD))

// unsigned
typedef unsigned char             uint8_t;
typedef unsigned short int        uint16_t;
typedef unsigned int              uint32_t;
typedef unsigned long long int    uint64_t;

// signed
typedef signed char            int8_t;
typedef signed short int       int16_t;
typedef signed int             int32_t;
typedef signed long long int   int64_t;

#else

#include <cstdint>

#endif

////////////////////////////////////////////////////////////////////////////
// Boolean and true/false definitions.
////////////////////////////////////////////////////////////////////////////

// Universal TRUE and FALSE values.

#ifndef __cplusplus

typedef uint8_t                  bool_t;

// Universal true and false values.
#ifndef true
#   define true  ((bool_t)1)
#endif
#ifndef false
#   define false ((bool_t)0)
#endif

#else
typedef bool                  bool_t;
#endif //__cplusplus

////////////////////////////////////////////////////////////////////////////
// Bits operations and definitions.
////////////////////////////////////////////////////////////////////////////

#define BIT(n)               (uint64_t)(1ULL<<(n))

#define BIT_MASK(value,n)    ({tdx_debug_assert(((value)==0) || ((value)==1)); (uint64_t)((value)<<(n));})

#define BITS(high,low)       ((BIT(high) - BIT(low) + BIT(high)))

#define BITS_MASK(value,high,low)  ((value) & BITS((high),(low)))

////////////////////////////////////////////////////////////////////////////
// Longer than 64bit types.
////////////////////////////////////////////////////////////////////////////
#define ALIGN(n) __attribute__ ((aligned(n)))

typedef union ALIGN(16)
{
    uint64_t  qwords[2];
    uint32_t  dwords[4];
    uint8_t   bytes[16];
} uint128_t;


typedef union
{
    uint64_t qwords[4];
    uint32_t dwords[8];
    uint8_t bytes[32];
} uint256_t;

// Only for power of 2 alignment
#define ROUND_UP(value, align)    ( ((value) + ((align) - 1)) & (~((align) - 1)) )


///////////////////////////////////////////////////////////////////////////
/// Min Max helpers
///////////////////////////////////////////////////////////////////////////

#define MIN(A,B) ( (A) < (B) ? (A) : (B) )

#endif // __PSEAMLDR_BASIC_TYPES_H_INCLUDED__

