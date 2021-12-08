//**********************************************************************;
//*                                                                    *;
//* Intel Proprietary                                                  *;
//*                                                                    *;
//* Copyright 2021 Intel Corporation All Rights Reserved.              *;
//*                                                                    *;
//* Your use of this software is governed by the TDX Source Code       *;
//* LIMITED USE LICENSE.                                               *;
//*                                                                    *;
//* The Materials are provided "as is," without any express or         *;
//* implied warranty of any kind including warranties of               *;
//* merchantability, non-infringement, title, or fitness for a         *;
//* particular purpose.                                                *;
//*                                                                    *;
//**********************************************************************;

#include "common.h"
#include "Common64.h"
#include "Header.h"
#include "NpSeamldrApi.h"
#include "NpSeamldr.h"
#include "msr.h"
#include "paging.h"
#include "accessors.h"
#include "vmcs.h"
#ifndef _SEAMLDR_VALIDATION_
#define uint8_t  UINT8
#define uint16_t UINT16
#define uint32_t UINT32
#define uint64_t UINT64
#endif
#include "vmx_vmcs.h"
#include "vmx_vmcs_accessors.h"
#include "simics_breakpoint.h"

void MemZeroWithMovdir64B(UINT8* dst, UINT64 size) {
    __declspec(align(MOVDIR64B_BLOCK_SIZE)) UINT8 ZeroBlock[MOVDIR64B_BLOCK_SIZE] = { 0 };
    UINT64 NumOfBlocks = size / MOVDIR64B_BLOCK_SIZE;

    for (UINT32 Idx = 0; Idx < NumOfBlocks; Idx++) {
        Movdir64B(ZeroBlock, dst);
        dst += MOVDIR64B_BLOCK_SIZE;
    }

    _mm_sfence();
}

static UINT32 GetX2ApicId()
{
    UINT32 CpuidEdx;

    AsmCpuidEx(CPUID_V2_EXTENDED_TOPOLOGY, CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_SMT, NULL, NULL, NULL, &CpuidEdx);

    return CpuidEdx;
}

UINT64 LoadModuleCode(UINT8 *Module, UINT32 ModuleSize) {   
    UINT64 Status = NP_SEAMLDR_PARAMS_STATUS_SUCCESS;

    Status = memcpy_s((void *)(SeamldrData.SeamrrVaLimit - (SeamldrData.PSeamldrConsts->CCodeRgnSize + C_P_SYS_INFO_TABLE_SIZE)), ModuleSize, Module, ModuleSize);

    return Status;
}

UINT64 MapModulePages(SEAMRR_PT_CTX * SeamrrPtCtx, UINT32 ModuleSize) {
    UINT64 CurCodeLinearAddr = C_CODE_RGN_BASE | SeamldrData.AslrRand;
    UINT64 CurCodePhysicalAddr;
    uint32_t Idx;
        
    CurCodePhysicalAddr = SeamldrData.SeamrrBase + SeamldrData.SeamrrSize - (SeamldrData.PSeamldrConsts->CCodeRgnSize + C_P_SYS_INFO_TABLE_SIZE);
    
    for (Idx = 0; Idx < ModuleSize / MOD_PAGE_SIZE; Idx++) {
        if (MapPage(SeamrrPtCtx, CurCodeLinearAddr, CurCodePhysicalAddr, IA32_PG_A | IA32_PG_P, PAGE_4K, FALSE) == NULL) {

            return NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
        }
        CurCodeLinearAddr += SEAMRR_PAGE_SIZE;
        CurCodePhysicalAddr += SEAMRR_PAGE_SIZE;
    }    
    return NP_SEAMLDR_PARAMS_STATUS_SUCCESS;
}

UINT64 SetupStacks(SEAMRR_PT_CTX *SeamrrPtCtx) {
    UINT32 StkPageIdx;
    UINT64 CurLinAddr = C_STACK_RGN_BASE | SeamldrData.AslrRand;
    UINT64 CurPhysAddr;
    
    CurPhysAddr = SeamldrData.SeamrrBase + SeamldrData.SeamrrSize - C_P_SYS_INFO_TABLE_SIZE - (SeamldrData.PSeamldrConsts->CCodeRgnSize) - (SeamldrData.PSeamldrConsts->CDataStackSize) - P_SEAMLDR_SHADOW_STACK_SIZE;

    for (StkPageIdx = 0; StkPageIdx < (UINT32)SeamldrData.PSeamldrConsts->CDataStackSize / SEAMRR_PAGE_SIZE; StkPageIdx++) {
        if (MapPage(SeamrrPtCtx, CurLinAddr, CurPhysAddr, IA32_PG_RW | IA32_PG_A | IA32_PG_D | IA32_PG_NX | IA32_PG_P, PAGE_4K, FALSE) == NULL) {
            return NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
        }
        CurLinAddr += SEAMRR_PAGE_SIZE;
        CurPhysAddr += SEAMRR_PAGE_SIZE;
    }
    // shadow stack page
    if (MapPage(SeamrrPtCtx, CurLinAddr, CurPhysAddr, IA32_PG_A | IA32_PG_D | IA32_PG_NX | IA32_PG_P, PAGE_4K, FALSE) == NULL) {
        return NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
    }
    
    return NP_SEAMLDR_PARAMS_STATUS_SUCCESS;
}


UINT64 SetupKeyholeMapping(SEAMRR_PT_CTX *SeamrrPtCtx) {
    UINT64 CurLinAddr = C_KEYHOLE_RGN_BASE | SeamldrData.AslrRand;
    UINT64 PrevMappedPtPa = (UINT64)-1;
    UINT64 CurMappedPtPa = (UINT64)NULL;
    UINT64 CurEditRgnLinAddr = C_KEYHOLE_EDIT_REGION_BASE | SeamldrData.AslrRand;
    UINT32 Idx;

      
    for (Idx = 0; Idx < SeamldrData.PSeamldrConsts->CKeyholeRgnSize / SEAMRR_PAGE_SIZE; Idx++) {
        CurMappedPtPa = (UINT64)MapPage(SeamrrPtCtx, CurLinAddr, 0, 0, PAGE_4K, TRUE);
        if (CurMappedPtPa == (UINT64) NULL) {
            return NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
        }
        if (CurMappedPtPa != PrevMappedPtPa) {
            if (MapPage(SeamrrPtCtx, CurEditRgnLinAddr, CurMappedPtPa,
                IA32_PG_P | IA32_PG_RW | IA32_PG_U | IA32_PG_A | IA32_PG_D | IA32_PG_NX, PAGE_4K, FALSE) == NULL) {
                return NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
            }

            PrevMappedPtPa = CurMappedPtPa;
            CurEditRgnLinAddr += SEAMRR_PAGE_SIZE;
        }
        CurLinAddr += SEAMRR_PAGE_SIZE;
    }

    return NP_SEAMLDR_PARAMS_STATUS_SUCCESS;
}

UINT64 SetupDataRegion(SEAMRR_PT_CTX *SeamrrPtCtx) {
    UINT64 CurLinAddr = C_DATA_RGN_BASE | SeamldrData.AslrRand;     
    UINT64 CurPhysAddr = SeamldrData.SeamrrBase + SeamldrData.SeamrrSize - (_4KB + SeamldrData.PSeamldrConsts->CCodeRgnSize + SeamldrData.PSeamldrConsts->CDataStackSize + C_P_SYS_INFO_TABLE_SIZE + SeamldrData.PSeamldrConsts->CDataRgnSize);
    UINT32 Idx;

    for (Idx = 0; Idx < SeamldrData.PSeamldrConsts->CDataRgnSize / SEAMRR_PAGE_SIZE; Idx++) {
        if (MapPage(SeamrrPtCtx, CurLinAddr, CurPhysAddr, IA32_PG_P | IA32_PG_RW | IA32_PG_A | IA32_PG_D | IA32_PG_NX, PAGE_4K, FALSE) == NULL) {
            return NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
        }          
        CurLinAddr += SEAMRR_PAGE_SIZE;
        CurPhysAddr += SEAMRR_PAGE_SIZE;
    }

    return NP_SEAMLDR_PARAMS_STATUS_SUCCESS;
}

UINT64 MapSysInfoTables(SEAMRR_PT_CTX* SeamrrPtCtx) {
    UINT64 Status = NP_SEAMLDR_PARAMS_STATUS_SUCCESS;    
    UINT64 CurPhysAddress = SeamldrData.SeamrrBase + SeamldrData.SeamrrSize - PAGE4K;
    UINT64 CurLinAddress = C_SYS_INFO_TABLE_BASE | SeamldrData.AslrRand;

    if (MapPage(SeamrrPtCtx, CurLinAddress, CurPhysAddress, IA32_PG_P | IA32_PG_A | IA32_PG_NX, PAGE_4K, FALSE) == NULL) {
        Status = NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
        ComSerialOut("Failed to map sysinfo table!\n");
        goto EXIT;
    }

#ifdef _TDXIO_SUPPORT        
    UINT32 Idx;
    CurLinAddress += SEAMRR_PAGE_SIZE;
    CurPhysAddress = SeamldrData.SeamrrBase + SeamldrData.SeamrrSize - C_P_SYS_INFO_TABLE_SIZE;

    for (Idx = 0; Idx < C_P_IO_SYS_INF_TABLE_SIZE / PAGE4K; Idx++) {
        if (MapPage(SeamrrPtCtx, CurLinAddress, CurPhysAddress, IA32_PG_P | IA32_PG_A | IA32_PG_NX, PAGE_4K, FALSE) == NULL) {
            Status = NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
            ComSerialOut("Failed to map io sysinfo table!\n");
            goto EXIT;
        }
        CurLinAddress += SEAMRR_PAGE_SIZE;
        CurPhysAddress += SEAMRR_PAGE_SIZE;
    }
#endif

EXIT:
    return Status;
}

UINT64 MapModuleRegion(SEAMRR_PT_CTX* SeamrrPtCtx) {
    UINT64 CurLinAddr = C_MODULE_RGN_BASE | SeamldrData.AslrRand;
    UINT64 CurPhysAddr = SeamldrData.SeamrrBase;
    UINT32 Idx;
    UINT64 SeamrrSize = SeamldrData.SeamrrSize;
    UINT64 EndOf2MbMappingPhysAddr;
    
    for (Idx = 0; Idx < (SeamrrSize - SeamldrData.PSysInfoTable->PSeamldrRange.Size) / PAGE2M; Idx++) {
        if (CurPhysAddr + PAGE2M > SeamldrData.PSysInfoTable->PSeamldrRange.Base) {
            break;
        }
        if (MapPage(SeamrrPtCtx, CurLinAddr, CurPhysAddr, IA32_PG_P | IA32_PG_RW | IA32_PG_A | IA32_PG_D | IA32_PG_NX, PAGE_2M, FALSE) == NULL) {
            return NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
        }
        CurLinAddr += PAGE2M;
        CurPhysAddr += PAGE2M;
    }
    
    if (CurPhysAddr < SeamldrData.SeamrrBase + SeamldrData.SeamrrSize - SeamldrData.PSysInfoTable->PSeamldrRange.Size) {
        EndOf2MbMappingPhysAddr = CurPhysAddr;
        for (Idx = 0; Idx < (SeamldrData.SeamrrBase + SeamldrData.SeamrrSize - SeamldrData.PSysInfoTable->PSeamldrRange.Size - EndOf2MbMappingPhysAddr) / PAGE4K; Idx++) {
            if (MapPage(SeamrrPtCtx, CurLinAddr, CurPhysAddr, IA32_PG_P | IA32_PG_RW | IA32_PG_A | IA32_PG_D | IA32_PG_NX, PAGE_4K, FALSE) == NULL) {
                return NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
            }
            CurLinAddr += PAGE4K;
            CurPhysAddr += PAGE4K;
        }
    }

    return NP_SEAMLDR_PARAMS_STATUS_SUCCESS;
}

void SetupSysInfoTable() {
    SeamldrData.PSysInfoTable->CodeRgn.Base = C_CODE_RGN_BASE | SeamldrData.AslrRand;
    SeamldrData.PSysInfoTable->CodeRgn.Size = SeamldrData.PSeamldrConsts->CCodeRgnSize;
    SeamldrData.PSysInfoTable->DataRgn.Base = C_DATA_RGN_BASE | SeamldrData.AslrRand;
    SeamldrData.PSysInfoTable->DataRgn.Size = SeamldrData.PSeamldrConsts->CDataRgnSize;
    SeamldrData.PSysInfoTable->StackRgn.Base = C_STACK_RGN_BASE | SeamldrData.AslrRand;
    SeamldrData.PSysInfoTable->StackRgn.Size = SeamldrData.PSeamldrConsts->CDataStackSize + P_SEAMLDR_SHADOW_STACK_SIZE;
    SeamldrData.PSysInfoTable->KeyholeRgn.Base = C_KEYHOLE_RGN_BASE | SeamldrData.AslrRand;
    SeamldrData.PSysInfoTable->KeyholeRgn.Size = SeamldrData.PSeamldrConsts->CKeyholeRgnSize;
    SeamldrData.PSysInfoTable->KeyholeEditRgn.Base = C_KEYHOLE_EDIT_REGION_BASE | SeamldrData.AslrRand;
    SeamldrData.PSysInfoTable->KeyholeEditRgn.Size = SeamldrData.PSeamldrConsts->CKeyholeEditRgnSize;
    SeamldrData.PSysInfoTable->ModuleRgnBase = C_MODULE_RGN_BASE | SeamldrData.AslrRand;
    SeamldrData.PSysInfoTable->AcmX2ApicId = GetX2ApicId();
    SeamldrData.PSysInfoTable->AcmX2ApicIdValid = SYS_INFO_TABLE_X2APICID_VALID;
}


#define SPR_A0_FMS          0x806F1
#define SPR_B0_FMS          0x806F2

void ReadSeamExtendMsr(UINT64 SeamExtendAddr) {
    writeMsr64(MSR_IA32_SEAMEXTEND, SeamExtendAddr | 0x1);
}

void RecordSeamIdentity() {
    __declspec(align(256)) SEAM_EXTEND_t SeamExtend = { 0 };

    SeamExtend.SeamReady =  SEAM_EXTEND_SEAM_READY_VAL;
    SeamExtend.SeamUnderDebug = (readMsr64(MSR_SGX_DEBUG_MODE) & BIT1) >> 1;
    SeamExtend.PSeamldrReady = SP_SEAMLDR_PLAG_READY;
    writeMsr64(MSR_IA32_SEAMEXTEND, (UINT64) &SeamExtend);
}

// main SEAMLDR flow function
void SeamldrAcm(SEAMLDR_COM64_DATA *pCom64, PT_CTX* PtCtx) {
    SEAMRR_PT_CTX SeamrrPtCtx;
    UINT16 Rrrr;
    UINT8  Comparand = NP_SEAMLDR_MUTEX_CLEAR;
    UINT8  SeamldrMutexStatus;
    UINT64 Status = NP_SEAMLDR_PARAMS_STATUS_SUCCESS;
	BOOL MutexAcquired = FALSE;
    BOOL SEAMRRUnlocked = FALSE;
    UINT64 NumTdxPrivateBits;
    UINT64 CPagingStructSize;
    UINT64 OriginalBIOSID;
    SeamrrBase_u     SeamrrBaseMsr;
    SeamrrMask_u     SeamrrMaskMsr;


    ComSerialOut("SeamldrAcm\n");
    SIMICS_BREAKPOINT;
    //     DEBUG ((EFI_D_INFO, ("SeamldrAcm\n Param struct PA: 0x%x", OriginalEDX));

    // SAVE BIOS ID before the first CPUID
    OriginalBIOSID = readMsr64(MSR_IA32_BIOS_SIGN_ID);

    // SAVE OS XMM's

    SeamldrData.MaximumSupportMemAddress = GetPhyAddrMask();
    SeamldrData.PSeamldrConsts = (P_SEAMLDR_CONSTS_t*)pCom64->PseamldrConstsOffset;

    SeamrrBaseMsr.raw = readMsr64(MSR_IA32_SEAMRR_BASE);
    SeamrrMaskMsr.raw = readMsr64(MSR_IA32_SEAMRR_MASK);

    // Read the IA32_SEAMRR_MASK range register and if the VALID bit is not set in the IA32_SEAMRR_MASK register, then set error code to EBADRANGE
    if (SeamrrMaskMsr.valid == 0) {
        ComSerialOut("Seamrr mask not valid\n");
        Status = NP_SEAMLDR_PARAMS_STATUS_EBADRANGE;
        goto EXIT;
    }

    NumTdxPrivateBits = (readMsr64(MSR_MKTME_ACTIVATE) >> 36ULL) & 0xFULL; // bits 39:36
    UINT64 KidBitMask = ((1ULL << NumTdxPrivateBits) - 1ULL);
    SeamldrData.TdxPrivateKidMask = KidBitMask << ((UINT64)GetMaxPhyAddr() - NumTdxPrivateBits);

    SeamldrData.SeamrrBase = (SeamrrBaseMsr.raw & B_SEAMRR_BASE);
    SeamldrData.SeamrrSize = ~(shiftLeft64(SeamrrMaskMsr.mask, N_SEAMRR_MASK_MASK) | SeamldrData.TdxPrivateKidMask | (~SeamldrData.MaximumSupportMemAddress)) + 1;

    SeamrrMaskMsr.valid = 0;

    PRINT_HEX_VAL("Writing seamrr mask - ", SeamrrMaskMsr.raw);
    writeMsr64(MSR_IA32_SEAMRR_MASK, SeamrrMaskMsr.raw);

	// If the SEAMRR is successfully unlocked, then set the SEAMRR_UNLOCKED flag to 1.
	SEAMRRUnlocked = TRUE;

    // Map the SysInfoTable as a single 4KB page with UC memtype
    SeamldrData.PSysInfoTable = (P_SYS_INFO_TABLE_t*)MapPhysicalRange(PtCtx, SeamldrData.SeamrrBase + SeamldrData.SeamrrSize - PAGE4K, PAGE4K, PAGE_WRITABLE, PAGE_4K, PAGE_UC_MEMTYPE);

    PRINT_HEX_VAL("SeamldrData.SysInfoTable->PSeamldrRange.Size: 0x", SeamldrData.PSysInfoTable->PSeamldrRange.Size);
    PRINT_HEX_VAL("SeamldrData.PSeamldrConsts->CCodeRgnSize: 0x", SeamldrData.PSeamldrConsts->CCodeRgnSize);
    PRINT_HEX_VAL("SeamldrData.PSeamldrConsts->CDataStackSize: 0x", SeamldrData.PSeamldrConsts->CDataStackSize);
    PRINT_HEX_VAL("SeamldrData.PSeamldrConsts->CDataRgnSize: 0x", SeamldrData.PSeamldrConsts->CDataRgnSize);
    //PRINT_HEX_VAL("pCom64: ", pCom64);
    //for (UINT32 i = 0; i < sizeof(SEAMLDR_COM64_DATA)/8; i++) {
    //    PRINT_HEX_VAL("pCom64 data: ", *((UINT64*)pCom64 + i));
    //}

    if ((UINT64)SeamldrData.PSysInfoTable == BAD_MAPPING) {
        ComSerialOut("Can't map the Sysinfo table, not enough page table entries\n");
        Status = NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
        goto EXIT;
    }

    // Verify that all sockets in the platform have identical family and model numbers by comparing bits 31:4 of first entry 
    // in SYS_INFO_TABLE.SOCKET_CPUID_TABLE to all valid entries in the table.
    // The stepping numbers can be different.If this check fails then set error code to EBADPLATF

    // System info table is in the first 4k of the SEAMRR
    
    if (SeamldrData.SeamrrSize > _1GB) {
        PRINT_HEX_VAL("Seamrr range too big: ", SeamldrData.SeamrrSize);
        Status = NP_SEAMLDR_PARAMS_STATUS_EBADRANGE;
        goto EXIT;
    }

    // Map the SEAMRR with 2MB pages
    SeamldrData.SeamrrVa = MapPhysicalRange(PtCtx, SeamldrData.SeamrrBase, SeamldrData.SeamrrSize, PAGE_WRITABLE, PAGE_2M, PAGE_WB_MEMTYPE);

    if (SeamldrData.SeamrrVa == BAD_MAPPING) {
        ComSerialOut("Can't map the SEAMRR region, not enough page table entries\n");
        Status = NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
        goto EXIT;
    }
    //    DEBUG ((EFI_D_INFO, "Seamrr VA: 0x%llx", SeamldrData.SeamrrVa));
    SeamldrData.SeamrrVaLimit = SeamldrData.SeamrrVa + SeamldrData.SeamrrSize;

    if ((SeamldrData.PSysInfoTable->TotNumSockets > SYS_INFO_TABLE_SOCKET_CPUID_TABLE_SIZE) ||
        (SeamldrData.PSysInfoTable->TotNumSockets == 0)){
        PRINT_HEX_VAL("Invalid number of sockets: \n", SYS_INFO_TABLE_SOCKET_CPUID_TABLE_SIZE);
        Status = NP_SEAMLDR_PARAMS_STATUS_EBADPARAM;
        goto EXIT;
    }

    for (UINT8 Idx = 1; Idx < SeamldrData.PSysInfoTable->TotNumSockets; Idx++) {
        if ((SeamldrData.PSysInfoTable->SocketCpuidTable[0] & CPUID_FM_MASK) != (SeamldrData.PSysInfoTable->SocketCpuidTable[Idx] & CPUID_FM_MASK)) {
            ComSerialOut("Socket CPUID table not valid\n");
            Status = NP_SEAMLDR_PARAMS_STATUS_EBADPLATF;
            goto EXIT;
        }
    }

    if (SeamldrData.PSysInfoTable->Version != P_SYS_INFO_TABLE_VERSION) {
        PRINT_HEX_VAL("P_SYS_INFO_TABLE version mismatch, got version \n", SeamldrData.PSysInfoTable->Version);
        Status = NP_SEAMLDR_PARAMS_STATUS_EBADPLATF;
        goto EXIT;
    }

#ifdef _TDXIO_SUPPORT
    if (SeamldrData.PSysInfoTable->IoSysInfoTableVer > IO_SYS_INFO_TABLE_MAX_VERSION) {
        PRINT_HEX_VAL("IO_SYS_INFO_TABLE version too high, got version \n", SeamldrData.PSysInfoTable->IoSysInfoTableVer);
        Status = NP_SEAMLDR_PARAMS_STATUS_EBADPLATF;
        goto EXIT;
    }
#endif

    ComSerialOut("Acquiring SEAMRR lock\n");

    // Acquire exclusive access to the SEAMRR by atomically setting the NP_SEAMLDR_MUTEX of the SYS_INFO_TABLE 
    // in the SEAMRR using a locked CMPXCHG8B 
    SeamldrMutexStatus = InterlockedCompareExchange8((UINT8 *)&SeamldrData.PSysInfoTable->NpSeamldrMutex, NP_SEAMLDR_MUTEX_ACQUIRED, Comparand);
    if (SeamldrMutexStatus != Comparand) {
        ComSerialOut("SeamldrMutexStatus is not correct\n");
        Status = NP_SEAMLDR_PARAMS_STATUS_ELDRINPROG;
        goto EXIT;
    }

	// If the compare succeeds, then the SeamldrMutexStatus moves to LOAD_IN_PROGRESS and set a flag – SET_LOAD_IN_PROGRESS to 1 – 
	// to indicate that this instance of the SEAMLDR moved the SeamldrMutexStatus field to LOAD_IN_PROGRESS.
	MutexAcquired = TRUE;

    if ((SeamldrData.PSysInfoTable->AcmX2ApicIdValid == SYS_INFO_TABLE_X2APICID_VALID) && (SeamldrData.PSysInfoTable->AcmX2ApicId != GetX2ApicId())) {
        ComSerialOut("AcmX2ApicId error\n");
        Status = NP_SEAMLDR_PARAMS_STATUS_EMODBUSY;
        goto EXIT;
    }

    ReadSeamExtendMsr((UINT64) &SeamldrData.SeamExtend);
    if ((SeamldrData.SeamExtend.PSeamldrReady != 0) || (SeamldrData.SeamExtend.SeamReady != 0)) {
        ComSerialOut("Error: P Seamldr hasn't shut down properly\n");
        SeamldrData.PSysInfoTable->NpSeamldrMutex = NP_SEAMLDR_MUTEX_CLEAR;
        Status = NP_SEAMLDR_PARAMS_STATUS_EMODBUSY;
        goto EXIT;
    }
    
    if (!RdSeed16(&Rrrr)) {
        Status = NP_SEAMLDR_PARAMS_STATUS_EUNSPECERR;
        ComSerialOut("RDSEED failed\n");
        goto EXIT;
    }

    SeamldrData.AslrRand = (((UINT64)(Rrrr & ASLR_MASK)) << 32);
#if (BULLSEYE_BUILD==1)
    SeamldrData.AslrRand = 0;
#endif

    CPagingStructSize = PAGING_STRUCTURE_SIZE(SeamldrData.PSeamldrConsts->CDataRgnSize) + PAGING_STRUCTURE_SIZE(SeamldrData.PSeamldrConsts->CCodeRgnSize) +
        PAGING_STRUCTURE_SIZE(SeamldrData.PSeamldrConsts->CDataStackSize + P_SEAMLDR_SHADOW_STACK_SIZE) + PAGING_STRUCTURE_SIZE(SeamldrData.PSeamldrConsts->CKeyholeRgnSize) +
        PAGING_STRUCTURE_SIZE(SeamldrData.PSeamldrConsts->CKeyholeEditRgnSize) + 
        PAGING_STRUCTURE_SIZE(SeamldrData.SeamrrSize - SeamldrData.PSysInfoTable->PSeamldrRange.Size) + _4KB;
    

    if (SeamldrData.PSysInfoTable->PSeamldrRange.Size < SeamldrData.PSeamldrConsts->CCodeRgnSize + SeamldrData.PSeamldrConsts->CDataStackSize + P_SEAMLDR_SHADOW_STACK_SIZE +
        SeamldrData.PSeamldrConsts->CDataRgnSize + CPagingStructSize) {
        ComSerialOut("Pseamldr Range too small\n");
        Status = NP_SEAMLDR_PARAMS_STATUS_ENOMEM;
        goto EXIT;
    }
    PRINT_HEX_VAL("SeamRR VA: 0x", SeamldrData.SeamrrVa);
    PRINT_HEX_VAL("SeamRR Size: 0x", SeamldrData.SeamrrSize);
    
    MemZeroWithMovdir64B((UINT8*)(SeamldrData.SeamrrVa + SeamldrData.PSysInfoTable->PSeamldrRange.Base - SeamldrData.SeamrrBase), (SeamldrData.SeamrrBase + SeamldrData.SeamrrSize -
        SeamldrData.PSysInfoTable->PSeamldrRange.Base - C_P_SYS_INFO_TABLE_SIZE));
    MemZeroWithMovdir64B((UINT8*)(SeamldrData.SeamrrVa + SeamldrData.SeamrrSize - _2KB), _2KB);

    ComSerialOut("Load module code\n");
    SIMICS_BREAKPOINT;
    //    DEBUG ((EFI_D_INFO, "Load module code\n"));

    // After copying(or as part of copying) into the SEAMRR, measure the copied image using SHA384.
    // Specifically, the measurement must not be done on the module image outside SEAMRR.
    Status = LoadModuleCode((UINT8*) pCom64->PseamldrOffset, pCom64->PseamldrSize);
    if (Status != NP_SEAMLDR_PARAMS_STATUS_SUCCESS) {
        ComSerialOut("Loading P-Seamldr code failed\n");
        goto EXIT;
    }
    
    InitPseamldrPtCtx(&SeamrrPtCtx, SeamldrData.SeamrrVa, SeamldrData.SeamrrBase, SeamldrData.SeamrrSize, SeamldrData.PSysInfoTable->PSeamldrRange.Base, CPagingStructSize);

    Status = RelocateImage(SeamldrData.SeamrrVaLimit - (SeamldrData.PSeamldrConsts->CCodeRgnSize + C_P_SYS_INFO_TABLE_SIZE), C_CODE_RGN_BASE | SeamldrData.AslrRand);
    if (Status != NP_SEAMLDR_PARAMS_STATUS_SUCCESS) {
        ComSerialOut("Failed to relocate P-Seamldr\n");
        goto EXIT;
    };

    Status = MapModulePages(&SeamrrPtCtx, pCom64->PseamldrSize);
    if (Status != NP_SEAMLDR_PARAMS_STATUS_SUCCESS) {
        ComSerialOut("Failed to map module pages\n");
        goto EXIT;
    };

    ComSerialOut("Setup stacks\n");
    Status = SetupStacks(&SeamrrPtCtx);
    if (Status != NP_SEAMLDR_PARAMS_STATUS_SUCCESS) {
        ComSerialOut("Failed to setup stacks\n");
        goto EXIT;
    };
    ComSerialOut("Setup keyhole\n");
    //    DEBUG ((EFI_D_INFO, ("Setup keyhole\n"));
    Status = SetupKeyholeMapping(&SeamrrPtCtx);
    if (Status != NP_SEAMLDR_PARAMS_STATUS_SUCCESS) {
        ComSerialOut("Failed to setup keyholes\n");
        goto EXIT;
    };

    ComSerialOut("Setup Data Region\n");
    (void)SetupDataRegion(&SeamrrPtCtx);

    // map system information tables
    ComSerialOut("Map SysInfoTable\n");
    Status = MapSysInfoTables(&SeamrrPtCtx);
    if (Status != NP_SEAMLDR_PARAMS_STATUS_SUCCESS) {
        goto EXIT;
    }

    ComSerialOut("Setup Module Region\n");
    Status = MapModuleRegion(&SeamrrPtCtx);
    if (Status != NP_SEAMLDR_PARAMS_STATUS_SUCCESS) {
        ComSerialOut("Failed to map Module Region!\n");
        goto EXIT;
    }


    ComSerialOut("Setup PSysinfo table\n");
    SetupSysInfoTable();

    ComSerialOut("Setup VMCS\n");
    SetupVmcs(SeamrrPtCtx.PtBaseAddrPa);

    ComSerialOut("Record Seam identity\n");
    SIMICS_BREAKPOINT;
    RecordSeamIdentity();

EXIT:
    pCom64->RetVal = Status;
    if (SEAMRRUnlocked) {
        if (MutexAcquired) {
            SeamldrData.PSysInfoTable->NpSeamldrMutex = NP_SEAMLDR_MUTEX_CLEAR;
        }

        // https://hsdes.intel.com/appstore/article/#/1308083915
        // SEAM range relocking:
        SeamrrMaskMsr.valid = 1;
        writeMsr64(MSR_IA32_SEAMRR_MASK, SeamrrMaskMsr.raw);
    }


    // RESTORE BIOS ID after the last CPUID
    writeMsr64(MSR_IA32_BIOS_SIGN_ID, OriginalBIOSID);

    PRINT_HEX_VAL("Exitingseamldr\nStatus: 0x", Status);
    SIMICS_BREAKPOINT;
//    DEBUG ((EFI_D_INFO, ("Exit 64 bit code\n"));
}
