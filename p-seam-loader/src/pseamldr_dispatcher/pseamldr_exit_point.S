// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file pseamldr_exit_point.S
 * @brief Module's VMM exit point - restores GPRs from local data and call SEAMRET
 */

#include "data_structures/pseamldr_data_offsets.h"


.section .text
.globl pseamldr_exit_point
.type  pseamldr_exit_point,@function
pseamldr_exit_point:

    /**
     * Restore all VMM GPRs on module entry from global data
     * Global data is located at GSBASE
     */
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET,     %rax
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+8,   %rcx
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+16,  %rdx
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+24,  %rbx
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+32,  %rsp // not actually needed
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+40,  %rbp
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+48,  %rsi
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+56,  %rdi
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+64,  %r8
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+72,  %r9
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+80,  %r10
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+88,  %r11
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+96,  %r12
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+104, %r13
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+112, %r14
    movq %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+120, %r15

    /**
     * Call SEAMRET
     */
#ifdef SEAM_INSTRUCTIONS_SUPPORTED_IN_COMPILER
	seamret
#else
	.byte 0x66, 0x0F, 0x01, 0xCD
#endif

    /**
     * No accesible code from this point
     * Panic with UD2 if reached
     */
    ud2

