// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file ia32_accessors.h
 * @brief IA32 Accessors Definitions
 */

#ifndef SRC_COMMON_ACCESSORS_IA32_ACCESSORS_H_
#define SRC_COMMON_ACCESSORS_IA32_ACCESSORS_H_

#include "../../../include/pseamldr_basic_defs.h"
#include "../../../include/pseamldr_basic_types.h"
#include "x86_defs/x86_defs.h"

/**
 * @brief Generate random 64-bit number. Returns false if generation failed (no entropy)
 *
 * @param rand_out
 */
_STATIC_INLINE_ bool_t ia32_rdrand64(uint64_t* rand_out)
{
    ia32_rflags_t rflags;
    uint64_t rand = 0;

    _ASM_VOLATILE_("rdrandq %0 \n"
                   "pushfq; popq %1\n"
                   : "=r" (rand), "=r" (rflags.raw));

    *rand_out = rand;

    return (rflags.cf != 0);
}

/**
 * @brief Invalidate TLB entries by calling INVLPG instruction
 * @param addr
 */
_STATIC_INLINE_ void ia32_invalidate_tlb_entries(uint64_t addr)
{
	_ASM_VOLATILE_ ("invlpg (%0);"::"r"(addr):"memory");
}

/**
 * @brief Call CPUID instruction
 * @param leaf
 * @param subleaf
 * @param eax
 * @param ebx
 * @param ecx
 * @param edx
 */
_STATIC_INLINE_ void ia32_cpuid(uint32_t leaf, uint32_t subleaf, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{

	_ASM_VOLATILE_ ("cpuid;"              // CPUID
                     : "=a" (*eax),        // Outputs: eax = %eax
                       "=b" (*ebx),        //          ebx = %ebx
                       "=c" (*ecx),        //          ecx = %ecx
                       "=d" (*edx)         //          edx = %edx
                     : "a"  (leaf),        // Inputs:  eax = leaf
                       "c"  (subleaf) );   //          ecx = subleaf
}

_STATIC_INLINE_ void ia32_clear_ac( void )
{
	_ASM_VOLATILE_ ("clac;":::"cc");
}

_STATIC_INLINE_ void ia32_set_ac( void )
{
	_ASM_VOLATILE_ ("stac;":::"cc");
}

/**
 * @brief Call UD2 instruction
 */
_STATIC_INLINE_ void ia32_ud2( void )
{
    _ASM_VOLATILE_ ("ud2" ::: ) ;
}

_STATIC_INLINE_ uint64_t ia32_rdmsr(uint64_t addr)
{
    uint32_t low,high;
    _ASM_VOLATILE_ ("rdmsr" : "=a"(low), "=d"(high) : "c"(addr));
    return (uint64_t)((((uint64_t)(high)) << 32) | (uint64_t)(low));
}

_STATIC_INLINE_ uint64_t ia32_rdmsr_with_input(uint64_t addr, uint64_t input)
{
    uint32_t low,high;
    _ASM_VOLATILE_ ("rdmsr" : "=a"(low), "=d"(high) : "a"((uint32_t)input), "d"((uint32_t)(input >> 32)), "c"(addr));
    return (uint64_t)((((uint64_t)(high)) << 32) | (uint64_t)(low));
}

_STATIC_INLINE_ void ia32_wrmsr(uint64_t addr, uint64_t value)
{
    _ASM_VOLATILE_ ("wrmsr" : : "a"((uint32_t)value), "d"((uint32_t)(value >> 32)), "c"(addr));
}

_STATIC_INLINE_ void ia32_out16( uint16_t port, uint16_t val )
{
    _ASM_VOLATILE_ ("outw %0,%w1" : : "a" (val), "dN" (port));
}

_STATIC_INLINE_ void ia32_pause( void )
{
    _ASM_VOLATILE_ ("pause" ) ;
}

_STATIC_INLINE_ void ia32_out8( uint16_t port, uint8_t val )
{
    _ASM_VOLATILE_ ("outb %0,%w1" : : "a" (val), "dN" (port));
}

_STATIC_INLINE_ uint8_t ia32_in8( uint16_t port )
{
    uint8_t v;

    _ASM_VOLATILE_ ("inb %w1,%0" : "=a" (v) : "Nd" (port));

    return v;
}

_STATIC_INLINE_ uint64_t ia32_rdtsc( void )
{
    uint32_t a, d;

    _ASM_VOLATILE_ ("rdtsc"
                   : "=a"(a), "=d"(d));
    return ( ((uint64_t) d << 32) | (uint64_t) a );
}

_STATIC_INLINE_ uint64_t ia32_set_timeout(uint64_t period)
{
    return ia32_rdtsc() + period;
}

_STATIC_INLINE_ bool_t ia32_is_timeout_expired(uint64_t endtime)
{
    return (int64_t)(endtime - ia32_rdtsc()) < 0;
}

_STATIC_INLINE_ uint64_t bit_scan_forward64(uint64_t mask)
{
    uint64_t lsb_position;
    _ASM_VOLATILE_ ("bsfq %1, %0 \n"
                        :"=r"(lsb_position)
                        :"r"(mask)
                        :);

    return lsb_position;
}

_STATIC_INLINE_ uint64_t bit_scan_reverse64(uint64_t mask)
{
    uint64_t msb_position;
    _ASM_VOLATILE_ ("bsrq %1, %0 \n"
                        :"=r"(msb_position)
                        :"r"(mask)
                        :);

    return msb_position;
}

_STATIC_INLINE_ void movdir64b(const void *src, uint64_t dst)
{
    _ASM_VOLATILE_ (".byte  0x66, 0x0F, 0x38, 0xF8," /*movdir64b op*/ "0x37;" /*ModRM = RDI->RSI*/
                    : : "D"(src), "S"(dst) : "memory" );
}

_STATIC_INLINE_ void lfence(void)
{
    _ASM_VOLATILE_ ("lfence" : : : "memory");
}

_STATIC_INLINE_ void mfence(void)
{
    _ASM_VOLATILE_ ("mfence" : : : "memory");
}

_STATIC_INLINE_ void sfence(void)
{
    _ASM_VOLATILE_ ("sfence" : : : "memory");
}

_STATIC_INLINE_ void serialize(void)
{
    _ASM_VOLATILE_ (".byte  0x0F, 0x01, 0xE8" /*SERIALIZE op*/ : : : "memory");
}


#endif /* SRC_COMMON_ACCESSORS_IA32_ACCESSORS_H_ */
