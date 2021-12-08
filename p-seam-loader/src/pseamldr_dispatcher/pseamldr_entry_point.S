// Intel Proprietary 
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file pseamldr_entry_point.S
 * @brief Module's VMM entry point - saves GPRs to local data and invokes the API dispatcher
 */
#include "data_structures/pseamldr_data_offsets.h"

.hidden pseamldr_dispatcher

.section .text
.globl pseamldr_entry_point
.type  pseamldr_entry_point,@function
pseamldr_entry_point:

    /**
     * Save all VMM GPRs on module entry to global data
     * Global data is located at GSBASE
     */
    movq %rax,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET
    movq %rcx,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+8
    movq %rdx,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+16
    movq %rbx,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+24
    movq %rsp,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+32 // not actually needed
    movq %rbp,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+40
    movq %rsi,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+48
    movq %rdi,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+56
    movq %r8,   %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+64
    movq %r9,   %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+72
    movq %r10,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+80
    movq %r11,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+88
    movq %r12,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+96
    movq %r13,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+104
    movq %r14,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+112
    movq %r15,  %gs:PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET+120

    /**
     * Call the PSEAMLDR dispatcher
     */
     callq pseamldr_dispatcher

    /**
     * Dispatcher should not return to this code
     * It returns to the VMM using SEAMRET
     * Panic with UD2 if reached
     */
    ud2
