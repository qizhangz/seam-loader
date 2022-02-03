// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file pseamldr_data.h
 * @brief PSEAMLDR data (per LP) definitions
 */
#ifndef __PSEAMLDR_DATA_H_INCLUDED__
#define __PSEAMLDR_DATA_H_INCLUDED__


#include "../../../include/pseamldr_basic_defs.h"
#include "../../../include/pseamldr_basic_types.h"
#include "pseamldr_data_offsets.h"
#include "pseamldr_api_defs.h"
#include "crypto/rsa.h"
#include "x86_defs/msr_defs.h"
#include "memory_handlers/keyhole_manager.h"
#include "seam_sigstruct.h"
#include "debug/tdx_debug.h"

/**
 * @struct gprs_state_t
 *
 * @brief Holds the state of the GPRs
 */
typedef union PACKED gprs_state_u
{
    struct
    {
        uint64_t rax;
        uint64_t rcx;
        uint64_t rdx;
        uint64_t rbx;
        uint64_t rsp_placeholder;
        uint64_t rbp;
        uint64_t rsi;
        uint64_t rdi;
        uint64_t r8;
        uint64_t r9;
        uint64_t r10;
        uint64_t r11;
        uint64_t r12;
        uint64_t r13;
        uint64_t r14;
        uint64_t r15;
    };

    uint64_t gprs[16];
} gprs_state_t;


#define KH_ENTRY_FREE           0
#define KH_ENTRY_MAPPED         1
#define KH_ENTRY_CAN_BE_REMOVED 2
/**
 * @struct keyhole_entry_t
 *
 * @brief Holds physical to linear PTE mappings
 *
 * It implements an LRU list and a hash list entry.
 */
typedef struct PACKED keyhole_entry_s
{
    uint64_t  mapped_pa;  /**< mapped physical address of this keyhole entry */
    /**
     * lru_next and lru_prev present an LRU doubly linked-list.
     */
    uint16_t  lru_next;
    uint16_t  lru_prev;
    uint16_t  hash_list_next;  /**< next element in hash list */
    /**
     * state can be KH_ENTRY_FREE or KH_ENTRY_MAPPED or KH_ENTRY_CAN_BE_REMOVED.
     */
    uint8_t   state;
    bool_t    is_writable;  /**< is PTE set to be Read-only or RW */
    bool_t    is_wb_memtype; /**< is PTE should be with WB or UC memtype */
} keyhole_entry_t;


/**
 * @struct keyhole_state_t
 *
 * @brief Holds the state of the keyhole mappings for this lp
 *
 * It implements an LRU list and a hash list.
 */
typedef struct PACKED keyhole_state_s
{
    /**
     * Each index in the keyhole_array presents an offset of the mapped linear address.
     * The array also implement and LRU doubly linked-list.
     */
    keyhole_entry_t keyhole_array[MAX_KEYHOLE_PER_LP];
    /**
     * A hash table, its index represents the index in the keyhole_array
     * that it is mapped to.
     */
    uint16_t  hash_table[MAX_KEYHOLE_PER_LP];
    /**
     * lru_head and lru_tail present the index of the keyhole_array LRU
     * doubly linked-list.
     */
    uint16_t  lru_head;
    uint16_t  lru_tail;
} keyhole_state_t;

typedef struct PACKED system_info_s
{
    bool_t   initialized;
    uint64_t max_pa;
    uint64_t seamrr_base;
    uint64_t seamrr_size;
    uint64_t hkid_mask;
    uint64_t max_mktme_hkids;
} system_info_t;

#define MAX_NUM_OF_LPS              1024

typedef struct pseamldr_data_s
{
    uint8_t               canary_padding[PSEAMLDR_DATA_CANARY_OFFSET];
    uint64_t              canary;
    gprs_state_t          vmm_regs; /**< vmm host saved GPRs */

    ia32_spec_ctrl_t      vmm_spec_ctrl;

    system_info_t         system_info;

    ALIGN(2048) seam_sigstruct_t  seam_sigstruct_snapshot;
    ALIGN(256) seamextend_t seamextend_snapshot;
    ALIGN(256) seamextend_t seamextend_tmp_buf;

    uint8_t               update_bitmap[MAX_NUM_OF_LPS / 8];
    uint32_t              lps_in_update;
    uint8_t               shutdown_bitmap[MAX_NUM_OF_LPS / 8];
    uint32_t              lps_in_shutdown;

    uint32_t              last_interrupted_lp;
    uint32_t              reserved;
    bool_t                module_range_initialized;
    uint32_t              num_remaining_updates;

    keyhole_state_t       keyhole_state;

    void*                 seamldr_data_fast_ref_ptr;
    void*                 psysinfo_fast_ref_ptr;

    rsa_ctx_t             rsa_context;

#ifdef DEBUGFEATURE_TDX_DBG_TRACE
    uint32_t              local_dbg_msg_num;
    debug_control_t       debug_control;
    debug_message_t       trace_buffer[TRACE_BUFFER_SIZE];
#endif

} pseamldr_data_t;
pseamldr_static_assert(offsetof(pseamldr_data_t, canary) == PSEAMLDR_DATA_CANARY_OFFSET, pseamldr_data_t_canary);
pseamldr_static_assert(offsetof(pseamldr_data_t, vmm_regs) == PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET, pseamldr_data_t_vmm_regs);
pseamldr_static_assert((offsetof(pseamldr_data_t, seam_sigstruct_snapshot) % 2048) == 0, pseamldr_data_t_sigstruct);
pseamldr_static_assert((offsetof(pseamldr_data_t, seamextend_snapshot) % 256) == 0, pseamldr_data_t_seamextend);

#endif // __PSEAMLDR_DATA_H_INCLUDED__

