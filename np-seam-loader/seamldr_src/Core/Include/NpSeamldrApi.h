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

#ifndef NP_SEAMLDR_API_H
#define NP_SEAMLDR_API_H

#pragma once
#pragma pack (push, 1)


#define NP_SEAMLDR_PARAMS_STATUS_SUCCESS           0
#define NP_SEAMLDR_ERROR_CLASS_ECPARAM      0000
#define NP_SEAMLDR_ERROR_CLASS_EPLAT        0001
#define NP_SEAMLDR_PARAMS_STATUS_EBADPARAM  0x8000000000000000
#define NP_SEAMLDR_PARAMS_STATUS_EMODBUSY   0x8000000000000001
#define NP_SEAMLDR_PARAMS_STATUS_ELDRINPROG 0x8000000000000002
#define NP_SEAMLDR_PARAMS_STATUS_EBADRANGE  0x8000000000010000
#define NP_SEAMLDR_PARAMS_STATUS_EBADPLATF  0x8000000000010001
#define NP_SEAMLDR_PARAMS_STATUS_ENOMEM     0x8000000000010002
#define NP_SEAMLDR_PARAMS_STATUS_EUNSPECERR 0x8000000000010003

#define NP_SEAMLDR_MAX_CPUID_TABLE_SIZE 255
#define SYS_INFO_TABLE_SOCKET_CPUID_TABLE_SIZE 8
#define SYS_INFO_TABLE_NUM_CMRS 32
#define SYS_INFO_TABLE_SEAM_STATUS_NOT_LOADED 0
#define SYS_INFO_TABLE_SEAM_STATUS_LOADED 1
#define SYS_INFO_TABLE_SEAM_STATUS_LOAD_IN_PROGRESS 2
#define SYS_INFO_TABLE_X2APICID_VALID 1

#define MOD_PAGE_SIZE   _4KB
#define ASLR_MASK       0x7FFC

typedef struct {
    UINT64 Base;
    UINT64 Size;
} MemRange_t;

typedef struct {
    // fields populated by mcheck
    UINT64 Version;
    UINT32 TotNumLps;
    UINT32 TotNumSockets;
    UINT32  SocketCpuidTable[SYS_INFO_TABLE_SOCKET_CPUID_TABLE_SIZE];
    MemRange_t PSeamldrRange;
    UINT8 SkipSMRR2Check;
    UINT8 TDX_AC;
#ifdef _TDXIO_SUPPORT    
    UINT16 IoSysInfoTableVer;
    UINT8 Reserved_0[60];
#else
    UINT8  Reserved_0[62];
#endif
    MemRange_t Cmr[SYS_INFO_TABLE_NUM_CMRS];
    UINT8  Reserved_1[1408];
    // fields populated by NP-SEAMLDR
    UINT64 NpSeamldrMutex;
    MemRange_t CodeRgn;
    MemRange_t DataRgn;
    MemRange_t StackRgn;
    MemRange_t KeyholeRgn;
    MemRange_t KeyholeEditRgn;
    UINT64 ModuleRgnBase;
    UINT32 AcmX2ApicId;
    UINT32 AcmX2ApicIdValid;
    UINT8  Reserved2[1944];
} P_SYS_INFO_TABLE_t;
C_ASSERT(sizeof(P_SYS_INFO_TABLE_t) == 4096);

typedef struct {
    UINT64 Valid;           // 8
    UINT8  TeeTcbSvn[16];   // 16
    UINT8  MrSeam[48];      // 48
    UINT8  MrSigner[48];    // 48
    UINT64 Attributes;      // 8
    UINT8  SeamReady;       // 1
    UINT8  SeamUnderDebug;  // 1
    UINT8  PSeamldrReady;   // 1
    UINT8  Reserved[5];     // 5
} SEAM_EXTEND_t;

C_ASSERT(sizeof(P_SEAMLDR_CONSTS_t) == 48);

#pragma pack (pop)
#endif // NP_SEAMLDR_API_H
