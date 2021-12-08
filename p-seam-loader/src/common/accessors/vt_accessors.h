// Intel Proprietary 
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file vmcs_accessors.h
 * @brief VMCS Accessors Definitions
 */

#ifndef SRC_COMMON_ACCESSORS_VT_ACCESSORS_H_
#define SRC_COMMON_ACCESSORS_VT_ACCESSORS_H_

#include "../../../include/pseamldr_basic_defs.h"
#include "../../../include/pseamldr_basic_types.h"
#include "x86_defs/x86_defs.h"
#include "x86_defs/vmcs_defs.h"
#include "ia32_accessors.h"
#include "helpers/error_reporting.h"

typedef uint64_t vmcs_ptr_t;

/**
 * @brief Read from VMCS entry
 * @param encoding
 *
 * @return value
 */
_STATIC_INLINE_ void ia32_vmread(uint64_t encoding, uint64_t *value) {
    //According to SDM, in 64-bit mode the instruction will fail is given an
    //operand that sets encoding bit beyond 32-bit
    pseamldr_debug_assert(encoding < BIT(32));

    //asm instruction expects both operands to be 64bit.
    ia32_rflags_t rflags;
    _ASM_VOLATILE_ ("vmreadq %2,%0\n"
			"pushfq\n"
			"popq %1"
			: "=m"(*value), "=r"(rflags.raw)
			:"r"(encoding)
			:"memory", "cc");

	pseamldr_sanity_check((rflags.cf == 0 && rflags.zf == 0), SCEC_VT_ACCESSORS_SOURCE, (uint32_t)encoding);
}

/**
 * @brief Write to VMCS entry
 * @param encoding
 * @param value
 * @return
 */
_STATIC_INLINE_ void ia32_vmwrite(uint64_t encoding, uint64_t value)
{
    //According to SDM, in 64-bit mode the instruction will fail is given an
    //operand that sets encoding bit beyond 32-bit
    pseamldr_debug_assert(encoding < BIT(32));

    //asm instruction expects both operands to be 64bit.
    ia32_rflags_t rflags;
    _ASM_VOLATILE_ ("vmwriteq %1,%2\n"
			"pushfq\n"
			"popq %0"
			: "=r"(rflags.raw)
            :"r"(value), "r"(encoding)
            : "cc");

	pseamldr_sanity_check((rflags.cf == 0 && rflags.zf == 0), SCEC_VT_ACCESSORS_SOURCE, (uint32_t)encoding);
}

/**
 * @brief Clear VMCS
 * @param vmcs_p
 */
_STATIC_INLINE_ void ia32_vmclear(vmcs_ptr_t *vmcs_p) {
	_ASM_VOLATILE_ ("vmclear %0"::"m"(vmcs_p):"memory");
}

#endif /* SRC_COMMON_ACCESSORS_VT_ACCESSORS_H_ */
