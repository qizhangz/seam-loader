// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file seam_vmcs_setup.h
 * @brief SEAM VMCS definitions
 */
#ifndef SRC_COMMON_DATA_STRUCTURES_SEAM_VMCS_SETUP_H_
#define SRC_COMMON_DATA_STRUCTURES_SEAM_VMCS_SETUP_H_

#include "../../../include/pseamldr_basic_defs.h"
#include "../../../include/pseamldr_basic_types.h"
#include "pseamldr_data.h"

#define SEAM_VMCS_NON_CANONICAL_RIP  0x1000000000000000ULL
#define SEAM_VMCS_CR0_BITS           (CR0_PE | CR0_ET | CR0_NE | CR0_WP | CR0_PG)
#define SEAM_VMCS_CR4_BITS           (CR4_DE | CR4_PAE | CR4_PGE | CR4_OSFXSR | CR4_OSXMMEXCPT | \
                                      CR4_VMXE | CR4_FSGSBASE | CR4_OSXSAVE | CR4_SMEP | CR4_SMAP | CR4_CET)
#define SEAM_VMCS_CS_SELECTOR        0x8U
#define SEAM_VMCS_SS_SELECTOR        0x10U
#define SEAM_VMCS_FS_SELECTOR        0x18U
#define SEAM_VMCS_GS_SELECTOR        0x18U
#define SEAM_VMCS_TR_SELECTOR        0x20U
#define SEAM_VMCS_PAT_MSR_VALUE      0x0006060606060606ULL
#define SEAM_VMCS_S_CET_MSR_VALUE    (IA32_S_CET_SH_STK_EN_BIT | IA32_S_CET_ENDBR_EN_BIT | IA32_S_CET_NO_TRACK_EN_BIT)
#define SEAM_VMCS_EFER_MSR_VALUE     (IA32_EFER_LME_BIT | IA32_EFER_LMA_BIT | IA32_EFER_NXE_BIT)

#define SEAM_VMCS_EXIT_CTLS_VALUE    (VM_EXIT_CTRL_SAVE_DEBUG_CTRL | VM_EXIT_CTRL_HOST_ADDR_SPACE_SIZE | \
                                      VM_EXIT_CTRL_SAVE_IA32_PAT | VM_EXIT_CTRL_LOAD_IA32_PAT | \
                                      VM_EXIT_CTRL_SAVE_IA32_EFER | VM_EXIT_CTRL_LOAD_IA32_EFER | \
                                      VM_EXIT_CTRL_CONCEAL_VMX_FROM_PT | VM_EXIT_CTRL_CLEAR_IA32_RTIT_CTL | \
                                      VM_EXIT_CTRL_CLEAR_LBR_CTL | VM_EXIT_CTRL_LOAD_CET_HOST_STATE | \
                                      VM_EXIT_CTRL_LOAD_IA32_PERF_GLOBAL_CTL | VM_EXIT_CTRL_SAVE_IA32_PERF_GLOBAL_CTL)

#define SEAM_VMCS_ENTRY_CTLS_VALUE   (VM_ENTRY_CTRL_LOAD_DEBUG_CTRL | VM_ENTRY_CTRL_LOAD_IA32_PERF_GLOBAL_CTL | \
                                      VM_ENTRY_CTRL_LOAD_IA32_PAT | VM_ENTRY_CTRL_LOAD_IA32_EFER | \
                                      VM_ENTRY_CTRL_CONCEAL_VMX_FROM_PT | VM_ENTRY_CTRL_LOAD_IA32_RTIT_CTL | \
                                      VM_ENTRY_CTRL_LOAD_UINV | VM_ENTRY_CTRL_LOAD_GUEST_CET_STATE | \
                                      VM_ENTRY_CTRL_LOAD_LBR_CTL | VM_ENTRY_CTRL_LOAD_IA32_PKRS)

void setup_seam_vmcs(uint64_t vmcs_la_base, memory_constants_t* mem_consts, uint64_t rip_offset);

#endif /* SRC_COMMON_DATA_STRUCTURES_SEAM_VMCS_SETUP_H_ */
