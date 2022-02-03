// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file seamldr_update.c
 * @brief SEAMLDR.UPDATE API handler
 */
#include "../../../include/pseamldr_basic_defs.h"
#include "pseamldr_api_defs.h"
#include "pseamldr_api_handlers.h"
#include "x86_defs/x86_defs.h"
#include "memory_handlers/keyhole_manager.h"
#include "helpers/helpers.h"
#include "accessors/data_accessors.h"
#include "accessors/ia32_accessors.h"
#include "seam_sigstruct.h"
#include "x86_defs/msr_defs.h"
#include "data_structures/pseamldr_data_types.h"
#include "memory_handlers/seam_memory_map.h"
#include "data_structures/seam_vmcs_setup.h"
#include "crypto/intel_key_hash.h"

static bool_t check_seam_sigstruct(seam_sigstruct_t* seam_sigstruct)
{
    // Following checks required for SEAM Sigstruct:
    // - Reserved fields are zero
    // - Key size is supported key size
    // - Exponent is supported exponent
    // - Header Type, Header Length, Header Version, and module type are correct
    // - Module Vendor is 0x8086
    // - All fields marked as "must be" have the specified values/sizes

    IF_RARE (!pseamldr_memcmp_to_zero(seam_sigstruct->reserved0, sizeof(seam_sigstruct->reserved0)) ||
             !pseamldr_memcmp_to_zero(seam_sigstruct->reserved1, sizeof(seam_sigstruct->reserved1))
            )
    {
        TDX_ERROR("SEAM sigstruct reserved fields are not zero\n");
        return false;
    }

    IF_RARE (seam_sigstruct->header_type != SEAM_SIGSTRUCT_HEADER_TYPE_GENERIC_FW ||
             seam_sigstruct->header_length != SEAM_SIGSTRUCT_HEADER_LENGTH_DWORDS ||
             seam_sigstruct->header_version != SEAM_SIGSTRUCT_HEADER_VERSION)
    {
        TDX_ERROR("SEAM sigstruct header = 0x%lx or header len = 0x%l  or version = 0x%lx are unsupported\n",
                seam_sigstruct->header_type, seam_sigstruct->header_length, seam_sigstruct->header_version);
        return false;
    }

    IF_RARE (seam_sigstruct->key_size != SEAM_SIGSTRUCT_KEY_SIZE_DWORDS ||
             seam_sigstruct->exponent_size != SEAM_SIGSTRUCT_EXPONENT_SIZE_DWORDS ||
             seam_sigstruct->modulus_size != SEAM_SIGSTRUCT_MODULUS_SIZE_DWORDS)
    {
        TDX_ERROR("SEAM sigstruct key = %d, exponent = %d or modulus size = %d, are unsupported\n",
                seam_sigstruct->key_size, seam_sigstruct->exponent_size, seam_sigstruct->modulus_size);
        return false;
    }

    IF_RARE (seam_sigstruct->module_vendor != SEAM_SIGSTRUCT_INTEL_MODULE_VENDOR ||
             seam_sigstruct->module_type.reserved != 0 ||
             seam_sigstruct->attributes != 0)
    {
        TDX_ERROR("SEAM sigstruct module vendor = 0x%lx, type = 0x%llx or attributes 0x%lx not supported\n",
                seam_sigstruct->module_vendor, seam_sigstruct->module_type.raw, seam_sigstruct->attributes);
        return false;
    }

    IF_RARE (seam_sigstruct->size != SEAM_SIGSTRUCT_SIZE_DWORDS)
    {
        TDX_ERROR("SEAM sigstruct size = 0x%lx is unsupported\n", seam_sigstruct->size);
        return false;
    }

    IF_RARE (seam_sigstruct->cpuid_table_size > SEAM_SIGSTRUCT_MAX_CPUID_TABLE_SIZE)
    {
        TDX_ERROR("SEAM sigstruct unsupported CPUID table size = %d\n", seam_sigstruct->cpuid_table_size);
        return false;
    }

    IF_RARE (seam_sigstruct->exponent != SEAM_SIGSTRUCT_RSA_EXPONENT)
    {
        TDX_ERROR("SEAM sigstruct RSA exponent = 0x%lx is unsupported\n", seam_sigstruct->exponent);
        return false;
    }

    return true;
}

static bool_t check_cpuid_support(seam_sigstruct_t* seam_sigstruct)
{
    p_sysinfo_table_t* p_sysinfo_table = get_psysinfo_table();

    // - The list of all the CPUs in the platform is in P_SYS_INFO_TABLE.SOCKET_CPUID_TABLE.
    // - The list of all the CPUs that this module can be loaded on is in TMP_SIGSTRUCT.CPUID_TABLE.
    // - The comparison is done with the low 4 bits (stepping number) masked off.

    bool_t cpuid_found = false;

    for (uint32_t i = 0; i < p_sysinfo_table->tot_num_sockets; i++)
    {
        for (uint32_t j = 0; j < seam_sigstruct->cpuid_table_size; j++)
        {
            if ((p_sysinfo_table->socket_cpuid_table[i].raw & ~CPUID_FMS_STEPPING_MASK) ==
                (seam_sigstruct->cpuid_table[j] & ~CPUID_FMS_STEPPING_MASK))
            {
                cpuid_found = true;
                break;
            }
        }

        if (!cpuid_found)
        {
            return false;
        }
        else
        {
            cpuid_found = false;
        }
    }

    return true;
}

static bool_t verify_seam_sigstruct_signature(seam_sigstruct_t* seam_sigstruct)
{
    // Verify the TMP_SIGSTRUCT.SIGNATURE field using RSA-3072 with TMP_SIGSTRUCT.EXPONENT as exponent
    // and TMP_SIGSTRUCT.MODULUS as key. The signature verification will check for EMSA-PKCS1-v1.5
    // format with DER encoding of the "Digest Info" (as in PKCS#1 v2.1).
    // The Digest Info must match the DER encoding for SHA-384.

    // Clear the MODULUS, EXPONENT, and SIGNATURE fields of TMP_SIGSTRUCT,
    // compute the SHA-384 hash of TMP_SIGSTRUCT, and compare the result with the hash value
    // recorded in encoded message from the decrypted SIGNATURE field.

    uint8_t modulus[SIGSTRUCT_MODULUS_SIZE];
    uint32_t exponent;
    uint8_t signature[SIGSTRUCT_SIGNATURE_SIZE];

    pseamldr_memcpy(modulus, sizeof(modulus), seam_sigstruct->modulus, SIGSTRUCT_MODULUS_SIZE);
    pseamldr_memcpy(signature, sizeof(signature), seam_sigstruct->signature, SIGSTRUCT_MODULUS_SIZE);
    exponent = seam_sigstruct->exponent;

    uint8_t comp_sigs[sizeof(seam_sigstruct_t) - SEAM_SIGSTRUCT_SIG_SIZE];

    pseamldr_memcpy(comp_sigs, sizeof(comp_sigs), seam_sigstruct, SEAM_SIGSTRUCT_HEADER_SIZE);

    pseamldr_memcpy(&comp_sigs[SEAM_SIGSTRUCT_HEADER_SIZE], sizeof(comp_sigs) - SEAM_SIGSTRUCT_HEADER_SIZE,
                    &seam_sigstruct->seamhash, SEAM_SIGSTRUCT_BODY_SIZE);

    bool_t is_valid = false;

    if (rsa_verify_signature(signature, (const uint8_t*)comp_sigs, sizeof(comp_sigs),
                             exponent, modulus, &is_valid) != 0)
    {
        FATAL_ERROR();
    }

    return is_valid;
}

static api_error_type verify_manifest(pseamldr_data_t* pseamldr_data, seamldr_params_t* seamldr_params,
                                      seam_sigstruct_t* seam_sigstruct)
{
    IF_RARE (!check_seam_sigstruct(seam_sigstruct))
    {
        TDX_ERROR("Incorrect SEAM sigstruct field values!\n");
        return PSEAMLDR_EBADSIG;
    }

    // Compute the SHA-384 hash of the SEAM module signers key (TMP_SIGSTRUCT.MODULUS).
    // If the result is not equal to the INTEL_SIGNER_KEY_HASH constant, then set ERROR_CODE = EBADSIG
    IF_RARE (!compute_and_verify_hash(seam_sigstruct->modulus, SIGSTRUCT_MODULUS_SIZE, intel_key_hash))
    {
        TDX_ERROR("Incorrect SEAM module signers key (modulus)!\n");
        return PSEAMLDR_EBADSIG;
    }

    // If the new module is declared as debug-signed (i.e. TMP_SIGSTRUCT.MODULE_TYPE[31] == 1),
    // but the NPK_ACTIVE bit in SGX_DEBUG_MODE MSR (0x503) is not set, then set ERROR_CODE = EBADSIG
    IF_RARE (seam_sigstruct->module_type.is_debug_signed)
    {
        ia32_sgx_debug_mode_t sgx_debug_mode;
        sgx_debug_mode.raw = ia32_rdmsr(IA32_SGX_DEBUG_MODE_MSR_ADDR);

        if (!sgx_debug_mode.debug_unlock_mode)
        {
            TDX_ERROR("Debug module type is set, but platform is not debug-unlocked\n");
            return PSEAMLDR_EBADSIG;
        }
    }

    // If the scenario is UPDATE (i.e. TMP_PARAMS.SCENARIO == 1) but the new module has
    // smaller SEAM SVN than the SEAM SVN of the currently loaded module
    // (i.e. TMP_SIGSTRUCT.SEAMSVN < TMP_UDPATE_SEAMEXTEND.TEE_TCB_INFO.SEAMSVN), then return EBADPARAM
    IF_RARE ((seamldr_params->scenario == SEAMLDR_SCENARIO_UPDATE) &&
             (seam_sigstruct->seamsvn < pseamldr_data->seamextend_snapshot.tee_tcb_svn.current_seam_svn))
    {
        TDX_ERROR("New module has smaller SEAM SVN than of the current module\n");
        return PSEAMLDR_EBADSIG;
    }

    // If the new module is restricted to a limited set of CPUs (i.e. TMP_SIGSTRUCT.CPUID_TABLE_SIZE > 0),
    // and it can’t run on all the CPU in the platform, then set ERROR_CODE = EUNSUPCPU
    IF_RARE ((seam_sigstruct->cpuid_table_size > 0) && !check_cpuid_support(seam_sigstruct))
    {
        TDX_ERROR("CPUID list is unsupported\n");
        return PSEAMLDR_EUNSUPCPU;
    }

    IF_RARE (!verify_seam_sigstruct_signature(seam_sigstruct))
    {
        TDX_ERROR("Signature verification failed!\n");
        return PSEAMLDR_EBADSIG;
    }

    return PSEAMLDR_SUCCESS;
}

static api_error_type check_seamldr_params(pseamldr_data_t* pseamldr_data, seamldr_params_t* seamldr_params)
{
    // Following checks required for SEAMLDR params:
    // - Reserved fields are zero
    // - The SCENARIO field is either LOAD or UPDATE
    // - All physical address pointers are valid input addresses (as defined for RCX)
    // - The NUM_MODULE_PAGES is within bounds

    IF_RARE (seamldr_params->version != 0)
    {
        TDX_ERROR("Seamldr params version field is not zero\n");
        return PSEAMLDR_EBADPARAM;
    }
    
    IF_RARE (!pseamldr_memcmp_to_zero(seamldr_params->reserved, sizeof(seamldr_params->reserved)))
    {
        TDX_ERROR("Seamldr params reserved fields are not zero\n");
        return PSEAMLDR_EBADPARAM;
    }

    IF_RARE ((seamldr_params->scenario != SEAMLDR_SCENARIO_LOAD) &&
             (seamldr_params->scenario != SEAMLDR_SCENARIO_UPDATE))
    {
        TDX_ERROR("Seamldr params unknown scenario - %d\n", seamldr_params->scenario);
        return PSEAMLDR_EBADPARAM;
    }

    IF_RARE (seamldr_params->num_module_pages > SEAMLDR_PARAMS_MAX_MODULE_PAGES)
    {
        TDX_ERROR("Seamldr params num of module pages - %d, exceed maximum - %d\n",
                seamldr_params->num_module_pages, SEAMLDR_PARAMS_MAX_MODULE_PAGES);
        return PSEAMLDR_EBADPARAM;
    }

    for (uint32_t i = 0; i < seamldr_params->num_module_pages; i++)
    {
        IF_RARE (!shared_hpa_check_with_pwr_2_alignment((pa_t)seamldr_params->mod_pages_pa_list[i], _4KB))
        {
            TDX_ERROR("Invalid physical address - 0x%llx of module page %d\n",
                    seamldr_params->mod_pages_pa_list[i], i);
            return PSEAMLDR_EBADPARAM;
        }
    }

    IF_RARE (!shared_hpa_check_with_pwr_2_alignment((pa_t)seamldr_params->sigstruct_pa, _4KB))
    {
        TDX_ERROR("Invalid physical address - 0x%llx of the sigstruct\n", seamldr_params->sigstruct_pa);
        return PSEAMLDR_EBADPARAM;
    }

    // If the requested scenario is update and there’s no current SEAM module
    // (i.e. TMP_PARAMS.SCENARIO == UPDATE && SEAMEXTEND_SNAPSHOT.SEAM_READY == 0),
    IF_RARE ((seamldr_params->scenario == SEAMLDR_SCENARIO_UPDATE) &&
             !pseamldr_data->seamextend_snapshot.seam_ready)
    {
        TDX_ERROR("Seamldr params have update scenario, but there's no current SEAM module\n");
        return PSEAMLDR_EBADPARAM;
    }

    return PSEAMLDR_SUCCESS;
}

static api_error_type initialize_memory_constants(pseamldr_data_t* pseamldr_data, seamldr_params_t* seamldr_params,
                                                 seam_sigstruct_t* seam_sigstruct, memory_constants_t* mem_consts)
{
    p_sysinfo_table_t* p_sysinfo_table = get_psysinfo_table();

    // Generate 13-bit random number for address space linear randomization using RDSEED instruction:
    // - ASLR_RAND = (RDSEED & 0x7FFC) << 32
    // - If no entropy found, then set ERROR_CODE = EUNSPECERR
    uint64_t rdrand, aslr_mask;

    IF_RARE (!ia32_rdrand64(&rdrand))
    {
        TDX_ERROR("Insufficient entropy to generate random ASLR number\n");
        return PSEAMLDR_ENOENTROPY;
    }

    aslr_mask = (rdrand & ASLR_BIT_MASK) << 32;

    // Set C_NUM_ADDR_LP = num_addr_lp_per_skt * P_SYS_INFO_TABLE.TOT_NUM_SOCKETS
    mem_consts->num_addressable_lps = get_num_addressable_lps_on_socket() *
                                                         p_sysinfo_table->tot_num_sockets;

    mem_consts->module_physlimit = p_sysinfo_table->p_seamldr_range_base;

    // SEAMCFG region:
    // SYS_INFO_TABLE and SEAM VMCS array
    mem_consts->sysinfo_table_linbase = LINEAR_BASE_SYSINFO_TABLE | aslr_mask;
    // SYSINFO table
    mem_consts->seamcfg_region_size = _4KB;
    mem_consts->vmcs_region_size = _4KB * mem_consts->num_addressable_lps;

    // Code region:
    mem_consts->code_region_size = seamldr_params->num_module_pages * _4KB;
    mem_consts->code_region_linbase = LINEAR_BASE_CODE_REGION | aslr_mask;
    mem_consts->code_region_physbase = mem_consts->module_physlimit - _2MB;

    // Data region:
    mem_consts->local_data_size = (seam_sigstruct->num_tls_pages + 1) * _4KB;
    mem_consts->global_data_size = (seam_sigstruct->num_global_data_pages + 1) * _4KB;
    mem_consts->data_region_size = (mem_consts->local_data_size * mem_consts->num_addressable_lps) +
                                    mem_consts->global_data_size;
    mem_consts->data_region_linbase = LINEAR_BASE_DATA_REGION | aslr_mask;
    mem_consts->data_region_physbase = pseamldr_data->system_info.seamrr_base +
                                       _4KB + mem_consts->vmcs_region_size; // Physical SYSINFO table and VMCS's

    // Stack region:
    mem_consts->data_stack_size = (seam_sigstruct->num_stack_pages + 1) * _4KB;
    mem_consts->shadow_stack_size = _4KB;
    mem_consts->lp_stack_size = mem_consts->shadow_stack_size + mem_consts->data_stack_size;
    mem_consts->stack_region_size  = mem_consts->lp_stack_size * mem_consts->num_addressable_lps;
    mem_consts->stack_region_linbase = LINEAR_BASE_STACK_REGION | aslr_mask;
    mem_consts->stack_region_physbase = mem_consts->code_region_physbase - mem_consts->stack_region_size;

    // Keyhole region:
    uint64_t keyhole_size = (seam_sigstruct->num_keyhole_pages + 1) * _4KB;
    mem_consts->keyhole_region_size = keyhole_size * mem_consts->num_addressable_lps;
    mem_consts->keyhole_region_linbase = LINEAR_BASE_KEYHOLE_REGION | aslr_mask;

    // Keyhole edit region:
    mem_consts->keyedit_region_size = ROUND_UP((mem_consts->keyhole_region_size / _4KB) * sizeof(ia32e_pxe_t), _4KB);
    mem_consts->keyedit_region_linbase = LINEAR_BASE_KEYEDIT_REION | aslr_mask;

    // Page tables:
    mem_consts->paging_structure_size = _4KB * (
            page_tables_needed_to_map(mem_consts->seamcfg_region_size) +
            page_tables_needed_to_map(mem_consts->stack_region_size) +
            page_tables_needed_to_map(mem_consts->data_region_size) +
            page_tables_needed_to_map(mem_consts->keyhole_region_size) +
            page_tables_needed_to_map(mem_consts->keyedit_region_size) +
            page_tables_needed_to_map(mem_consts->code_region_size) +
            1 // One PML4 page table
            );

    // If the size of the SEAM range part dedicated to SEAM module (i.e. C_MODULE_PHYSLIMIT – SEAMRR.BASE)
    // is smaller than the minimum size required to load and map the new SEAM module
    // and prepare to execution, then set ERROR_CODE = ENOMEM

    uint64_t min_required_size = mem_consts->seamcfg_region_size +
                                 mem_consts->vmcs_region_size +
                                 mem_consts->stack_region_size +
                                 mem_consts->data_region_size +
                                 mem_consts->paging_structure_size +
                                 SEAMRR_MODULE_CODE_REGION_SIZE;

    IF_RARE ((mem_consts->module_physlimit - pseamldr_data->system_info.seamrr_base) < min_required_size)
    {
        TDX_ERROR("Not enough memory to load, map and prepare new SEAM module\n");
        TDX_ERROR("Available size = 0x%llx , minimal required = 0x%llx\n",
                (mem_consts->module_physlimit - pseamldr_data->system_info.seamrr_base), min_required_size);
        return PSEAMLDR_ENOMEM;
    }

    return PSEAMLDR_SUCCESS;
}

static api_error_type seam_module_load_and_verify(pseamldr_data_t* pseamldr_data, p_sysinfo_table_t* p_sysinfo_table,
                                                  seamldr_params_t* seamldr_params, seam_sigstruct_t* seam_sigstruct)
{
    uint64_t code_region_start_la;

    code_region_start_la = p_sysinfo_table->module_region_base + pseamldr_data->system_info.seamrr_size
                           - p_sysinfo_table->p_seamldr_range_size - SEAMRR_MODULE_CODE_REGION_SIZE;

    // Copy and measure SEAM module image pages to the last 2M of the SEAM range
    for (uint64_t i = 0; i < seamldr_params->num_module_pages; i++)
    {
        void* src_page_la = map_pa((void*)seamldr_params->mod_pages_pa_list[i], TDX_RANGE_RO);
        void* dst_page_la = (void*)(code_region_start_la + (i * SEAM_MODULE_PAGE_SIZE));
        pseamldr_memcpy(dst_page_la, SEAM_MODULE_PAGE_SIZE, src_page_la, SEAM_MODULE_PAGE_SIZE);
        free_la(src_page_la);
    }

    // Measure the image (in SEAM range) using SHA-384.
    // If the result is not equal to TMP_SIGSTRUCT.SEAMHASH then set ERROR_CODE = EBADHASH.
    uint32_t module_size = (uint32_t)(seamldr_params->num_module_pages * SEAM_MODULE_PAGE_SIZE);
    IF_RARE (!compute_and_verify_hash((const uint8_t*)code_region_start_la, module_size, seam_sigstruct->seamhash))
    {
        TDX_ERROR("Incorrect SEAM module hash!\n");
        return PSEAMLDR_EBADHASH;
    }

    return PSEAMLDR_SUCCESS;
}

static void setup_system_information(p_sysinfo_table_t* p_sysinfo_table, memory_constants_t* mem_consts)
{
    // Copy MCHECK information from P_SYS_INFO_TABLE to SYS_INFO_TABLE
    sysinfo_table_t* sysinfo_table = (sysinfo_table_t*)p_sysinfo_table->module_region_base;

    pseamldr_memcpy(sysinfo_table, SYSINFO_TABLE_MCHECK_DATA_SIZE, p_sysinfo_table, SYSINFO_TABLE_MCHECK_DATA_SIZE);

    sysinfo_table->code_rgn_base = mem_consts->code_region_linbase;
    sysinfo_table->code_rgn_size = mem_consts->code_region_size;
    sysinfo_table->data_rgn_base = mem_consts->data_region_linbase;
    sysinfo_table->data_rgn_size = mem_consts->data_region_size;
    sysinfo_table->stack_rgn_base = mem_consts->stack_region_linbase;
    sysinfo_table->stack_rgn_size = mem_consts->stack_region_size;
    sysinfo_table->keyhole_rgn_base = mem_consts->keyhole_region_linbase;
    sysinfo_table->keyhole_rgn_size = mem_consts->keyhole_region_size;
    sysinfo_table->keyhole_edit_rgn_base = mem_consts->keyedit_region_linbase;
    sysinfo_table->keyhole_edit_rgn_size = mem_consts->keyedit_region_size;
    sysinfo_table->num_stack_pages = (mem_consts->data_stack_size / _4KB) - 1;
    sysinfo_table->num_tls_pages = (mem_consts->local_data_size / _4KB) - 1;
}

_STATIC_INLINE_ void fill_seamextend_structure(seamextend_t* seamextend, seam_sigstruct_t* sigstruct,
                                               bool_t seam_ready)
{
    seamextend->tee_tcb_svn.current_seam_svn = sigstruct->seamsvn;
    pseamldr_memcpy(seamextend->mrseam, sizeof(seamextend->mrseam), sigstruct->seamhash, sizeof(sigstruct->seamhash));
    basic_memset_to_zero(seamextend->mrsigner, sizeof(seamextend->mrsigner));
    seamextend->attributes = 0;
    seamextend->seam_ready = seam_ready ? 1 : 0;
    seamextend->valid = 0x1FF;
}

_STATIC_INLINE_ void zero_seamextend_structure(seamextend_t* seamextend)
{
    seamextend->tee_tcb_svn.current_seam_svn = 0;
    basic_memset_to_zero(seamextend->mrseam, sizeof(seamextend->mrseam));
    basic_memset_to_zero(seamextend->mrsigner, sizeof(seamextend->mrsigner));
    seamextend->attributes = 0;
    seamextend->seam_ready = 0;
    seamextend->valid = 0;
}

static api_error_type install_epilogue(api_error_type flow_status, uint64_t rec_status,
                                       pseamldr_data_t* pseamldr_data)
{
    IF_COMMON (flow_status == PSEAMLDR_SUCCESS)
    {
        fill_seamextend_structure(&pseamldr_data->seamextend_snapshot,
                                  &pseamldr_data->seam_sigstruct_snapshot, true);
    }
    else
    {
        if (rec_status == PSEAMLDR_UNRECOVERABLE_ERROR)
        {
            zero_seamextend_structure(&pseamldr_data->seamextend_snapshot);
        }
    }

    seamextend_write(&pseamldr_data->seamextend_snapshot);

    return flow_status;
}

api_error_type seamldr_install(uint64_t seamldr_params_pa)
{
    pseamldr_data_t             * pseamldr_data = get_pseamldr_data();
    p_sysinfo_table_t           * p_sysinfo_table = get_psysinfo_table();
    seamldr_params_t            * seamldr_params_la = NULL;
    seamldr_params_t              seamldr_params;
    seam_sigstruct_t            * seam_sigstruct_la = NULL;
    uint32_t                      lpid;
    memory_constants_t            mem_consts;

    uint64_t                      rec_status = PSEAMLDR_RECOVERABLE_ERROR;
    api_error_type                return_value = PSEAMLDR_EUNSPECERR;

    lpid = get_current_lpid();

    // If X2APICID = CPUID.1FH.0:EDX >= MIN_LP_BITMAP_SIZE, then fatal error
    IF_RARE (lpid >= MAX_NUM_OF_LPS)
    {
        TDX_ERROR("Incorrect LPID=%d, bigger than defined max num of LP's=%d\n", lpid, MAX_NUM_OF_LPS);
        return_value = PSEAMLDR_EUNSPECERR;
        goto EXIT;
    }

    // If SEAMLDR.SHUTDOWN session already started (i.e. bitcount(SHUTDOWN_BITMAP) > 0),
    // then set RAX = EBADCALL, RDX = 0 and return.
    IF_RARE (pseamldr_data->lps_in_shutdown != 0)
    {
        TDX_ERROR("Shutdown process started, can't update\n");
        return_value = PSEAMLDR_EBADCALL;
        goto EXIT;
    }

    // If this API was already invoked on this LP in the current update session,
    // then set RAX = EBADCALL, RDX = 0 and return
    // Mark this LP as invoked in the current update session
    IF_RARE (bit_test_and_set(pseamldr_data->update_bitmap, lpid))
    {
        TDX_ERROR("Update already occurred on this LPID=%d\n", lpid);
        return_value = PSEAMLDR_EBADCALL;
        goto EXIT;
    }

    // If this is the first LP on which this API is invoked
    // then capture the value of SEAMEXTEND register in SEAMEXTEND_SNAPSHOT,
    // and block further SEAMCALLs to the old SEAM module
    // (by clearing the SEAM_READY field in the SEAMEXTEND register).
    IF_RARE (pseamldr_data->lps_in_update == 0)
    {
        seamextend_read(&pseamldr_data->seamextend_snapshot);
        uint8_t seam_ready_original = pseamldr_data->seamextend_snapshot.seam_ready;
        pseamldr_data->seamextend_snapshot.seam_ready = 0;
        seamextend_write(&pseamldr_data->seamextend_snapshot);
        pseamldr_data->seamextend_snapshot.seam_ready = seam_ready_original;
    }

    pseamldr_data->lps_in_update++;

    // Evict the SEAM module’s VMCS associated with this LP from the VMCS cache
    // (by executing VMCLEAR with physical address SEAMRR.BASE + 4K + CPUID.B.0:EDX * 4K).
    // This helps ensure that old cached SEAM VMCS won’t later be evicted and corrupt SEAM VMCS in memory.
    if (pseamldr_data->module_range_initialized)
    {
        uint64_t current_seam_vmcs_pa = pseamldr_data->system_info.seamrr_base + _4KB + (lpid * _4KB);
        ia32_vmclear((vmcs_ptr_t*)current_seam_vmcs_pa);
    }

    // If this is not the last LP on which this API is invoked then set RAX = RDX = 0 and return.
    IF_COMMON (pseamldr_data->lps_in_update < p_sysinfo_table->tot_num_lps)
    {
        return_value = PSEAMLDR_SUCCESS;
        goto EXIT;
    }

    // The following steps apply only to the last LP:

    // ************************ Input checks ************************
    // Check SEAMLDR PARAMS input PA
    return_value = check_and_map_aligned_shared_hpa((pa_t)seamldr_params_pa, _4KB, TDX_RANGE_RO, (void**)&seamldr_params_la);
    IF_RARE (return_value != PSEAMLDR_SUCCESS)
    {
        TDX_ERROR("Seamldr Params PA is not a valid/aligned shared HPA pa=0x%llx\n", seamldr_params_pa);
        goto UPDATE_END;
    }

    pseamldr_memcpy(&seamldr_params, sizeof(seamldr_params_t), seamldr_params_la, sizeof(seamldr_params_t));

    return_value = check_seamldr_params(pseamldr_data, &seamldr_params);
    IF_RARE (return_value != PSEAMLDR_SUCCESS)
    {
        TDX_ERROR("Seamldr params invalid\n");
        goto UPDATE_END;
    }

    // ******************* Manifest verification *******************
    // Sigstruct PA is verified at this point
    seam_sigstruct_la = map_pa((void*)seamldr_params.sigstruct_pa, TDX_RANGE_RO);
    pseamldr_memcpy(&pseamldr_data->seam_sigstruct_snapshot, sizeof(seam_sigstruct_t), seam_sigstruct_la, sizeof(seam_sigstruct_t));

    return_value = verify_manifest(pseamldr_data, &seamldr_params, &pseamldr_data->seam_sigstruct_snapshot);
    IF_RARE (return_value != PSEAMLDR_SUCCESS)
    {
        TDX_ERROR("Manifest invalid\n");
        goto UPDATE_END;
    }

    // ******************* Memory initialization *******************
    return_value = initialize_memory_constants(pseamldr_data, &seamldr_params, &pseamldr_data->seam_sigstruct_snapshot, &mem_consts);
    IF_RARE (return_value != PSEAMLDR_SUCCESS)
    {
        TDX_ERROR("Memory constants calculation failed\n");
        goto UPDATE_END;
    }

    // Clear the entire SEAM range, except the P-SEAMLDR sub-range using MOVDIR64 instruction.
    // from SEAMRR.BASE to C_MODULE_PHYSLIMIT – 1
    zero_area_cacheline((void*)p_sysinfo_table->module_region_base,
            mem_consts.module_physlimit - pseamldr_data->system_info.seamrr_base);

    // Remember that the module range was initialized once
    pseamldr_data->module_range_initialized = true;
    // Set UNRECOVERABLE = 1 (memory has been modified).
    rec_status = PSEAMLDR_UNRECOVERABLE_ERROR;

    // Memory map
    return_value = seam_module_memory_map(pseamldr_data, &mem_consts);
    IF_RARE (return_value != PSEAMLDR_SUCCESS)
    {
        TDX_ERROR("Memory mapping failed\n");
        goto UPDATE_END;
    }

    // Image load and verify
    return_value = seam_module_load_and_verify(pseamldr_data, p_sysinfo_table,
                             &seamldr_params, &pseamldr_data->seam_sigstruct_snapshot);
    IF_RARE (return_value != PSEAMLDR_SUCCESS)
    {
        TDX_ERROR("Seam module load and verification failed\n");
        goto UPDATE_END;
    }

    // System information setup
    setup_system_information(p_sysinfo_table, &mem_consts);

    // SEAM VMCS setup
    setup_seam_vmcs(p_sysinfo_table->module_region_base + _4KB, &mem_consts, pseamldr_data->seam_sigstruct_snapshot.rip_offset);

    // All done!
    return_value = PSEAMLDR_SUCCESS;

UPDATE_END:

    return_value = install_epilogue(return_value, rec_status, pseamldr_data);

    basic_memset_to_zero(pseamldr_data->update_bitmap, sizeof(pseamldr_data->update_bitmap));
    pseamldr_data->lps_in_update = 0;

EXIT:

    pseamldr_data->vmm_regs.rdx = (return_value == PSEAMLDR_SUCCESS) ? 0 : rec_status;

    IF_RARE (seamldr_params_la != NULL)
    {
        free_la(seamldr_params_la);
    }

    IF_RARE (seam_sigstruct_la != NULL)
    {
        free_la(seam_sigstruct_la);
    }

    return return_value;
}
