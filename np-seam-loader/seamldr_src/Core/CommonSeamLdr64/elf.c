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
#include "elf.h"

UINT64 RelocateSection(UINTN ElfImage, Elf64_Shdr* SectionHeader, UINTN RelocationAddr) {
    Elf64_Rela* CurRela;

    for (UINT32 i = 0; i < SectionHeader->sh_size / SectionHeader->sh_entsize; i++) {        
        CurRela = (Elf64_Rela*)(ElfImage + SectionHeader->sh_addr + i * SectionHeader->sh_entsize);
        if (CurRela->r_info == R_X86_64_RELATIVE) {            
            *(UINT64 *)(ElfImage + CurRela->r_offset) = RelocationAddr + CurRela->r_addend;
        }
        else {
            ComSerialOut("Unsupported relocation!!\n");
            return NP_SEAMLDR_PARAMS_STATUS_EBADPARAM;
        }
    }

    return NP_SEAMLDR_PARAMS_STATUS_SUCCESS;
}

UINT64 RelocateImage(UINTN ElfImage, UINTN RelocationAddr) {
    Elf64_Ehdr* ElfHeader = (Elf64_Ehdr*)ElfImage;
    Elf64_Shdr* CurSheader;
    UINT64 Status = NP_SEAMLDR_PARAMS_STATUS_SUCCESS;

    for (UINT32 i = 0; i < ElfHeader->e_shnum; i++) {        
        CurSheader = (Elf64_Shdr*)(ElfImage + ElfHeader->e_shoff + (UINT64)i * ElfHeader->e_shentsize);
        if (CurSheader->sh_type == SHT_RELA) {
            Status = RelocateSection(ElfImage, CurSheader, RelocationAddr);
            if (Status != NP_SEAMLDR_PARAMS_STATUS_SUCCESS) {
                return Status;
            }
        }
    }

    return Status;
}