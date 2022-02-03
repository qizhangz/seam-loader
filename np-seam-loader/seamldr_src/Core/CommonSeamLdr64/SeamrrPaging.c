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
#include "paging.h"
#include "accessors.h"
#include "Header.h"
#include "NpSeamldr.h"


SEAMRR_PT_CTX* InitPseamldrPtCtx(OUT SEAMRR_PT_CTX* SeamrrPtCtx, UINT64 SeamRrVa, UINT64 SeamRrBase, UINT64 SeamRrSize, UINT64 PSeamldrRangeBase, UINT64 PagingStructSize)
{
    ComSerialOut("InitSeamrrPtCtx\n");
    PRINT_HEX_VAL("SeamRrBase: 0x", SeamRrBase);
    PRINT_HEX_VAL("SeamRrSize: 0x", SeamRrSize);
        
    SeamrrPtCtx->PtBaseAddrLa = SeamRrVa + (PSeamldrRangeBase - SeamRrBase) + _8KB;    
    SeamrrPtCtx->PtBaseAddrPa = SeamRrBase + (PSeamldrRangeBase - SeamRrBase) + _8KB;
    
    SeamrrPtCtx->PtAllocatorPa = SeamrrPtCtx->PtBaseAddrPa + _4KB;
    SeamrrPtCtx->NumPageLevels = 4;

    SeamrrPtCtx->VPDelta = SeamRrVa - SeamRrBase;

    SeamrrPtCtx->PagingStructSize = PagingStructSize;

    PRINT_HEX_VAL("SeamrrPtCtx->PtBaseAddrLa: 0x", SeamrrPtCtx->PtBaseAddrLa);
    PRINT_HEX_VAL("SeamrrPtCtx->PtBaseAddrPa: 0x", SeamrrPtCtx->PtBaseAddrPa);
    PRINT_HEX_VAL("SeamrrPtCtx->PtAllocatorPa: 0x", SeamrrPtCtx->PtAllocatorPa);
    PRINT_HEX_VAL("SeamrrPtCtx->PagingStructSize: 0x", SeamrrPtCtx->PagingStructSize);
    return SeamrrPtCtx;
}

UINT64* MapPage(SEAMRR_PT_CTX* SeamrrPtCtx, UINT64 LinearAddr, UINT64 PhysAddr, UINT64 Attr, PAGE_SIZE PageMappingSize, BOOL IsKeyHoleMapping) {
    UINT32 PtIdx, Idx;
    UINT64 *PxeLinear;
    UINT64 *PxePhysical = NULL;
    UINT64 CurNumPageLevels = SeamrrPtCtx->NumPageLevels;
    UINT32 PtShiftBits = 12;

    if (PageMappingSize == PAGE_2M) {
        CurNumPageLevels--;
        PtShiftBits = 21;
        Attr |= IA32_PG_PS;
    }

    PxeLinear = (UINT64 *)SeamrrPtCtx->PtBaseAddrLa;
    // walk and fill if needed non leaf levels
    for (Idx = 0; Idx < CurNumPageLevels - 1; Idx++) {
        PtIdx = (LinearAddr >> ((SeamrrPtCtx->NumPageLevels - 1) * 9 - Idx * 9 + 12)) & 0x1ff;
        // check if PT exists
        if (PxeLinear[PtIdx] == 0) {
            // if the allocator reached the data region - error
            if (SeamrrPtCtx->PtAllocatorPa >= SeamldrData.SeamrrBase + SeamldrData.SeamrrSize -
                (C_P_SYS_INFO_TABLE_SIZE + SeamldrData.PSeamldrConsts->CCodeRgnSize + SeamldrData.PSeamldrConsts->CDataStackSize + P_SEAMLDR_SHADOW_STACK_SIZE + 
                    SeamldrData.PSeamldrConsts->CDataRgnSize)) {
                return NULL;
            }
            PxeLinear[PtIdx] = SeamrrPtCtx->PtAllocatorPa | IA32_PG_P | IA32_PG_RW | IA32_PG_A | IA32_PG_U;
            // non leaf PDE mapping keyhole pages
            if (IsKeyHoleMapping && (2 == SeamrrPtCtx->NumPageLevels - Idx)) {
                PxeLinear[PtIdx] |= IA32_PG_NX;
            }
            SeamrrPtCtx->PtAllocatorPa += SEAMRR_PAGE_SIZE;
        }
        PxePhysical = (UINT64 *)(PxeLinear[PtIdx] & GetPhyAddrMask() & (~(SEAMRR_PAGE_SIZE - 1)));
        PxeLinear = (UINT64 *)((INT64)PxePhysical + SeamrrPtCtx->VPDelta);        // read pxe from virtual address
    }
    // map leaf level
    PtIdx = (LinearAddr >> PtShiftBits) & 0x1ff;
    PxeLinear[PtIdx] = PhysAddr | Attr;
    // return PT used for mapping
    return PxePhysical;
}
