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


#ifndef TPM_H
#define TPM_H
#pragma pack (push, 1)

#include <common.h>

//
// Initial part of file contains definitions that are not family
// related.
//
#define BIG_ENDIAN_WORD(p) (((const UINT16)(((p)& 0xFF) << 8)) | ((const UINT16)(((p)& 0xFF00) >> 8)))

#define BIG_ENDIAN_DWORD(p) ( \
          ((const UINT32)(((p)& 0xFF) << 24))    | \
          ((const UINT32)(((p)& 0xFF00) << 8))   | \
          ((const UINT32)(((p)& 0xFF0000) >> 8)) | \
          ((const UINT32)(((p)& 0xFF000000) >> 24)))

#define CLIENT_AUX  0
#define SERVER_AUX  1

#ifndef FN_STARTUPACM
#define FN_STARTUPACM         0XFF
#endif
//
// TPM indices
//
#define TPM12_INDEX_AUX                       0x50000002  // NV Storage
#define TPM12_INDEX_AUX_NEW                   0x50000003  // NV Storage   
#define TPM12_INDEX_LCP_OWN                   0x40000001  // Launch Policy Owner
#define TPM12_INDEX_LCP_SUP                   0x50000001  // Launch Policy Default (Supplier)
#ifndef TPM20_INDEX_AUX
#define TPM20_INDEX_AUX                       0x01C10102   // NV Storage
#endif
#ifndef TPM20_INDEX_LCP_SUP
#define TPM20_INDEX_LCP_SUP                   0x01C10103  // Launch Policy Default (Supplier)
#endif
#ifndef TPM20_INDEX_LCP_OWN
#define TPM20_INDEX_LCP_OWN                   0x01C10106   // Launch Policy Owner
#endif
//
// LTSX INDEX_AUX  offsets
//
#define PROMOTE_HASH_OFFSET             32
#define TPM12_PCR0_OFFSET                     64

#define TPM_BASE_0                      0x0FED40000
//
// TPM Registers for loc 0, 2, and 3
//
#define TPM_ACCESS_L0                   0x0FED40000
#define TPM_ACCESS_L1                   0x0FED41000
#define TPM_ACCESS_L2                   0x0FED42000
#define TPM_ACCESS_L3                   0x0FED43000

#define PCR_REG_00                      0x00
#define PCR_REG_01                      0x01
#define PCR_REG_02                      0x02
#define PCR_REG_03                      0x03
#define PCR_REG_04                      0x04
#define PCR_REG_05                      0x05
#define PCR_REG_06                      0x06
#define PCR_REG_07                      0x07
#define PCR_REG_08                      0x08
#define PCR_REG_09                      0x09
#define PCR_REG_10                      0x0a
#define PCR_REG_11                      0x0b
#define PCR_REG_12                      0x0c
#define PCR_REG_13                      0x0d
#define PCR_REG_14                      0x0e
#define PCR_REG_15                      0x0f
#define PCR_REG_16                      0x10
#define PCR_REG_17                      0x11
#define PCR_REG_18                      0x12
#define PCR_REG_19                      0x13
#define PCR_REG_20                      0x14
#define PCR_REG_21                      0x15
#define PCR_REG_22                      0x16
#define MAX_PCR                         22

#define TPM20_MAX_DATA_SIZE_INDEX_AUX         (sizeof(AUX_DATA) + SHA384_DIGEST_SIZE + SHA384_DIGEST_SIZE + 4)
#define TPM12_MAX_DATA_SIZE_INDEX_AUX         (sizeof(AUX_DATA) + SHA1_DIGEST_SIZE + 2)
// #define TPM12_CLIENT_DATA_SIZE_INDEX_AUX  0x40
// #define TPM12_SERVER_DATA_SIZE_INDEX_AUX  0x60



#define BIG_ENDIAN_DATA_SIZE_INDEX_AUX  0x68000000

#define DATA_SIZE_INDEX_LCP             0x36
#define BIG_ENDIAN_DATA_SIZE_INDEX_LCP  0x36000000
#define LOCALITY_0                      BIT0
#define LOCALITY_1                      BIT1
#define LOCALITY_2                      BIT2
#define LOCALITY_3                      BIT3
#define LOCALITY_4                      BIT4
#define AUX_READ_LOCALITIES  LOCALITY_0+LOCALITY_1+LOCALITY_2+LOCALITY_3+LOCALITY_4
#define AUX_WRITE_LOCALITIES LOCALITY_3+LOCALITY_4
#define SUP_READ_LOCALITIES  LOCALITY_0+LOCALITY_1+LOCALITY_2+LOCALITY_3+LOCALITY_4
#define SUP_WRITE_LOCALITIES LOCALITY_0+LOCALITY_1+LOCALITY_2+LOCALITY_3+LOCALITY_4
#define BIG_ENDIAN_SUP_ATTRIBUTES       0x00200000

#define OWN_READ_LOCALITIES  LOCALITY_0+LOCALITY_1+LOCALITY_2+LOCALITY_3+LOCALITY_4
#define OWN_WRITE_LOCALITIES LOCALITY_0+LOCALITY_1+LOCALITY_2+LOCALITY_3+LOCALITY_4
#define BIG_ENDIAN_OWN_ATTRIBUTES       0x02000000

#define TPM20_TPMA_NV_OWNERWRITE BIT1
#define TPM20_TPMA_NV_AUTHWRITE BIT2
#define TPM20_TPMA_NV_POLICYWRITE BIT3
#define TPM20_TPMA_NV_POLICY_DELETE BIT10
#define TPM20_TPMA_NV_WRITEDEFINE BIT13
#define TPM20_TPMA_NV_WRITE_STCLEAR BIT14
#define TPM20_TPMA_NV_AUTHREAD BIT18
#define TPM20_TPMA_NV_NO_DA BIT25
#define TPM20_TPMA_NV_PLATFORMCREATE BIT30


#define TPM20_AUX_ATTR TPM20_TPMA_NV_POLICYWRITE + TPM20_TPMA_NV_POLICY_DELETE +\
                       TPM20_TPMA_NV_WRITE_STCLEAR + TPM20_TPMA_NV_AUTHREAD  +\
                       TPM20_TPMA_NV_NO_DA + TPM20_TPMA_NV_PLATFORMCREATE

// TPM20_LCP_PS_ATTR is PS1 attributes
// TPM20_LCP_PS_ATTR2 is PS2 attributes
#define TPM20_LCP_PS_ATTR TPM20_TPMA_NV_POLICYWRITE + TPM20_TPMA_NV_POLICY_DELETE +\
                          TPM20_TPMA_NV_AUTHREAD + TPM20_TPMA_NV_NO_DA +\
                          TPM20_TPMA_NV_PLATFORMCREATE

#define TPM20_LCP_PS_ATTR2 TPM20_TPMA_NV_AUTHWRITE + TPM20_TPMA_NV_POLICY_DELETE +\
                          TPM20_TPMA_NV_AUTHREAD + TPM20_TPMA_NV_NO_DA +\
                          TPM20_TPMA_NV_PLATFORMCREATE + TPM20_TPMA_NV_WRITEDEFINE

#define TPM20_LCP_PO_ATTR TPM20_TPMA_NV_OWNERWRITE + TPM20_TPMA_NV_POLICYWRITE +\
                          TPM20_TPMA_NV_AUTHREAD + TPM20_TPMA_NV_NO_DA


//-----------------------------------------------------------------------------
//  AUX index
//
#define AUX_DATA_REV_MAJOR_30 03
#define AUX_DATA_REV_MAJOR_20 02
#define AUX_DATA_REV_MAJOR_10 01
#define AUX_DATA_REV_MINOR_00 00
#define AUX_DATA_REV_10       ((AUX_DATA_REV_MAJOR_10 << 8) + AUX_DATA_REV_MINOR_00)
#define AUX_DATA_REV_20       ((AUX_DATA_REV_MAJOR_20 << 8) + AUX_DATA_REV_MINOR_00)
#define AUX_DATA_REV_30       ((AUX_DATA_REV_MAJOR_30 << 8) + AUX_DATA_REV_MINOR_00)
//
// Inverted logic of EPS bit:
// EPS == 1 - de-asserted
// EPS == 0 - asserted
//
#define EPS BIT0

//
// This is shared common part of AUX index data. Rest of content is client and
// server specific.
//
// typedef struct _TPM12_COM_AUX_DATA {
//   UINT32 Res;
//   UINT32 AcmRevision;
//   UINT16 ModuleID;
//   UINT16 ModuleFlags;
//   UINT16 AuxDataRev;
//   UINT16 Res2;
//   UINT32 Flags;
//   UINT16 Res3;
//   UINT16 REF_time;
// } TPM12_COM_AUX_DATA;
//
// typedef struct _TPM20_COM_AUX_DATA {
//   UINT32 Res;
//   UINT32 AcmRevision;
//   UINT16 ModuleID;
//   UINT16 ModuleFlags;
//   UINT16 AuxDataRev;
//   UINT16 Res2;
//   UINT32 Flags;
//   UINT16 Res3;
//   UINT16 REF_time;
//   UINT32 AuxHashAlgId;
//   UINT32 Res4;
// } TPM20_COM_AUX_DATA;

//#define AUX_DATA_REV_20_BIOSAC_REG_SIZE 32
#define AUX_DATA_BIOSAC_REGISTRATION_SIZE 20
typedef struct _ACM_AUX_REVOCATION {
  union {
    struct {
      UINT8 MinVer;
      UINT8 Flags;
    };
    UINT16  Val;
  };
} ACM_AUX_REVOCATION;

// typedef struct _AUX_REVOCATION20 {
//   ACM_AUX_REVOCATION SinitRevocation;
//   ACM_AUX_REVOCATION BiosacRevocation;
//   ACM_AUX_REVOCATION Reserved;
//   ACM_AUX_REVOCATION Reserved2;
// } AUX_REVOCATION20;
//
// typedef struct _AUX_REVOCATION12 {
//   ACM_AUX_REVOCATION SinitRevocation;
//   ACM_AUX_REVOCATION BiosacRevocation;
// } AUX_REVOCATION12;

// typedef struct _AUX_DATA20 {
//   TPM20_COM_AUX_DATA ComAuxData;
//   union {
//     struct {
//       UINT8             AutoPromotionHash[32];
//       UINT8             PCR0SnapShot[32];
//     };
//     struct {
//       UINT8         McConfigHash[48];   // SHA384 digest size.
//       UINT32        McInputData;
//       UINT8         Reserved[12];
//     };
//   };
//   AUX_REVOCATION20  AuxRevocation;
// } AUX_DATA20;

// Client specific TPM 1.2 Aux struct
// typedef struct _AUX_DATA12_CLIENT {
//   TPM12_COM_AUX_DATA  ComAuxData;
//   union {
//     UINT8             Res[TPM12_CLIENT_DATA_SIZE_INDEX_AUX - sizeof (TPM12_COM_AUX_DATA) - sizeof (AUX_REVOCATION12)];
//     struct {
//       UINT8           Reserved[8];
//       UINT8           McConfigHash[20]; // SHA1 digest size.
//       UINT32          McInputData;
//       UINT8           Reserved2[8];
//     };
//   };
//   AUX_REVOCATION12  AuxRevocation;
// } AUX_DATA12_CLIENT;


// Serevr specific TPM 1.2 Aux struct
// typedef struct _AUX_DATA12_SERVER {
//   TPM12_COM_AUX_DATA      ComAuxData;
//   UINT8             Res[TPM12_SERVER_DATA_SIZE_INDEX_AUX - sizeof (TPM12_COM_AUX_DATA) - sizeof (AUX_REVOCATION12)];
//   AUX_REVOCATION12  AuxRevocation;
// } AUX_DATA12_SERVER;

//
// BPT specific AUX data structures. Will use the same structure for
// TPM 1.2 and TPM 2.0 They are different in tyoes of fields but have
// identical layout.
//

#define TPM_LOC_DIGEST { \
                         0x07, 0x03, 0x9B, 0x45, 0xBA, 0xF2, 0xCC, 0x16, 0x9B, 0x0D, 0x84, 0xAF, 0x7C, 0x53, 0xFD, 0x16, \
                         0x22, 0xB0, 0x33, 0xDF, 0x0A, 0x5D, 0xCD, 0xA6, 0x63, 0x60, 0xAA, 0x99, 0xE5, 0x49, 0x47, 0xCD}

#define TPM_LOC_CC_DIGEST { \
                            0x06, 0xC7, 0xD8, 0x05, 0xAD, 0x3B, 0xEC, 0x11, 0x06, 0x50, 0x2A, 0x44, 0xC6, 0xB2, 0xE3, 0xB3, \
                            0x6D, 0x15, 0x77, 0x50, 0xE8, 0xEF, 0xCA, 0x1F, 0xFF, 0x99, 0x8C, 0x87, 0x4A, 0x76, 0x64, 0xC5}

#define TPM_LOC_OR_LOC_CC_DIGEST { \
                                   0xEF, 0x9A, 0x26, 0xFC, 0x22, 0xD1, 0xAE, 0x8C, 0xEC, 0xFF, 0x59, 0xE9, 0x48, 0x1A, 0xC1, 0xEC, \
                                   0x53, 0x3D, 0xBE, 0x22, 0x8B, 0xEC, 0x6D, 0x17, 0x93, 0x0F, 0x4C, 0xB2, 0xCC, 0x5B, 0x97, 0x24}

#define TPM_SHA1_LOC_OR_LOC_CC_DIGEST { \
	0xDD, 0x28, 0xEE, 0xAA, 0x86, 0x54, 0xB1, 0x56, 0xCA, 0x11, 0x95, 0xA9, 0x48, 0x22, 0x1F, 0x93, \
	0xE7, 0x37, 0xF7, 0x6F}
						
#define TPM_SM3_LOC_OR_LOC_CC_DIGEST { \
	0xB3, 0x04, 0x52, 0xDB, 0x01, 0xF3, 0x50, 0xE0, 0xA2, 0xF8, 0x25, 0xF4, 0x5D, 0x62, 0xC5, 0xB6, \
	0x2F, 0xBA, 0x7E, 0xF1, 0x0A, 0x16, 0x23, 0xB5, 0x8B, 0x07, 0x59, 0xD2, 0x78, 0x2D, 0xB1, 0x7F}

#define TPM_SHA256_LOC_DIGEST { \
    0x07, 0x03, 0x9B, 0x45, 0xBA, 0xF2, 0xCC, 0x16, 0x9B, 0x0D, 0x84, 0xAF, 0x7C, 0x53, 0xFD, 0x16, \
    0x22, 0xB0, 0x33, 0xDF, 0x0A, 0x5D, 0xCD, 0xA6, 0x63, 0x60, 0xAA, 0x99, 0xE5, 0x49, 0x47, 0xCD}

#define TPM_SHA256_LOC_CC_DIGEST { \
    0x06, 0xC7, 0xD8, 0x05, 0xAD, 0x3B, 0xEC, 0x11, 0x06, 0x50, 0x2A, 0x44, 0xC6, 0xB2, 0xE3, 0xB3, \
    0x6D, 0x15, 0x77, 0x50, 0xE8, 0xEF, 0xCA, 0x1F, 0xFF, 0x99, 0x8C, 0x87, 0x4A, 0x76, 0x64, 0xC5}

#define TPM_SHA256_LOC_OR_LOC_CC_DIGEST { \
    0xEF, 0x9A, 0x26, 0xFC, 0x22, 0xD1, 0xAE, 0x8C, 0xEC, 0xFF, 0x59, 0xE9, 0x48, 0x1A, 0xC1, 0xEC, \
    0x53, 0x3D, 0xBE, 0x22, 0x8B, 0xEC, 0x6D, 0x17, 0x93, 0x0F, 0x4C, 0xB2, 0xCC, 0x5B, 0x97, 0x24}

#define TPM_SHA384_LOC_OR_LOC_CC_DIGEST { \
	0xC6, 0x31, 0xE9, 0x59, 0x98, 0x79, 0x5D, 0x9A, 0xEF, 0x1E, 0xEE, 0x1A, 0xE4, 0x58, 0xEB, 0xA4, \
	0xAD, 0x45, 0x83, 0xE1, 0xB7, 0x21, 0x22, 0x79, 0x6E, 0xA3, 0xE1, 0xFE, 0x6C, 0x4F, 0xF6, 0x6C, \
	0x00, 0xEE, 0x3D, 0x51, 0xAB, 0xEF, 0xB4, 0x9A, 0xC5, 0x16, 0x67, 0xFB, 0x7A, 0x49, 0x7C, 0x20}

#define TPM_SHA512_LOC_OR_LOC_CC_DIGEST { \
	0x0D, 0x1C, 0x53, 0x86, 0x86, 0xD3, 0x18, 0x76, 0xD7, 0xF8, 0x0D, 0x19, 0x51, 0x77, 0x22, 0xD8, \
	0xBA, 0xFB, 0x89, 0xEB, 0x7B, 0x53, 0x28, 0x3F, 0xB7, 0x7C, 0x15, 0xA3, 0x34, 0xAA, 0x2F, 0xD1, \
	0x20, 0x62, 0x2E, 0x4B, 0x8A, 0x9A, 0xC3, 0xA3, 0x8B, 0xB5, 0xA1, 0xCE, 0xA3, 0x10, 0x95, 0x16, \
	0xF9, 0xF9, 0x9D, 0x7A, 0x38, 0x92, 0x7D, 0x87, 0x83, 0x06, 0xDD, 0x07, 0xDD, 0x85, 0x87, 0xDC}



//*****************************************************************************
//
// MUST DETERMINE BUFFER SIZE!. ALSO - CAN I REUSE BUFFERS?
// It must be 4KB for full TPM2 spec but since we are using subset of
// commands we might get away with smaller size.
//
//*****************************************************************************

#define TpmOutBuff      TpmInBuff
#define TPMBUF_LEN      _4KB

#define ALLOWED_ERROR   BIT12           // Larger largest TPM2.0 error value.

#define NON_FATAL_802   0x802           //
#define NON_FATAL_801   0x801
#define NON_FATAL_BASE  0x800
#define RETRY_CNT       3

//-----------------------------------------------------------------------------
// MMIO I/F related definitions
//-----------------------------------------------------------------------------



typedef struct _TPM_FIFO {
  UINT8   TPM_ACCESS;                   // OFF 0
  UINT8   R[FILL8(0x14, 0)];
  UINT32  TPM_INTF_CAPABILITY;          // OFF 0x14
  UINT8   TPM_STS;                      // OFF 0x18
  UINT16  TPM_BRST_CNT;                 // OFF 0x19
  UINT8   R2[FILL8(0x24, 0x1A)];
  UINT8   TPM_DATA_FIFO[4];             // OFF 0x24
  UINT8   R3[FILL8(0x30, 0x27)];
  UINT32  TPM_INTERFACE_ID;             // OFF30
  UINT8   R4[FILL8(0x80, 0x33)];
  UINT8   TPM_XDATA_FIFO[64];           // OFF 0x80
  UINT8   R5[FILL8(0xF00, 0xBF)];
  UINT16  TPM_VID;                      // OFF 0xF00
  UINT16  TPM_DID;                      // OFF 0xF02
  UINT8   TPM_RID;                      // OFF 0xF04
} TPM_FIFO;

typedef struct _REG_TPM_INTF_CAPABILITY {
  UINT32  Misc:8;
  UINT32  BurstCountStatic:1;           // 0 - dynamic; 1 - static
  //
  // DataTransferSizeSupport:
  //
  // 00 - legacy only
  // 01 - 8B
  // 10 - 32B
  // 11 - 64B
  UINT32  DataTransferSize:2;
  UINT32  Res:17;
  UINT32  InterfaceVersion:3;           // 000 - 1.21 or before; 001 - 1.3
  UINT32  Res2:1;
} REG_TPM_INTF_CAPABILITY;

//
// Access register definitions.
//
#define TPM_ESTABLISHMENT_MASK    BIT0
#define REQUEST_USE_MASK          BIT1
#define PENDING_REQUEST_MASK      BIT2
#define SEIZE_MASK                BIT3
#define BEEN_SEIZED_MASK          BIT4
#define ACTIVE_LOCALITY_MASK      BIT5
#define TMP_REG_VALID_STS_MASK    BIT7

typedef struct _REG_TPM_ACCESS {
  UINT8 TpmEstablishment:1;
  UINT8 RequestUse:1;
  UINT8 PendingRequest:1;
  UINT8 Seize:1;
  UINT8 BeenSeized:1;
  UINT8 ActiveLocality:1;
  UINT8 Reserved:1;
  UINT8 TmpRegValidSts:1;
} REG_TPM_ACCESS;

//
// Status register definitions.
//
#define RESPONSE_RETRY_MASK   BIT1
#define SELF_TEST_DONE_MASK   BIT2
#define EXPECT_MASK           BIT3
#define DATA_AVAIL_MASK       BIT4
#define TPM_GO_MASK           BIT5
#define COMMAND_READY_MASK    BIT6
#define STS_VALID_MASK        BIT7

typedef struct _REG_TPM_STS {
  UINT8 Reserved:1;
  UINT8 ResponseRetry:1;
  UINT8 SelfTestDone:1;
  UINT8 Expect:1;
  UINT8 DataAvail:1;
  UINT8 TpmGo:1;
  UINT8 CommandReady:1;
  UINT8 StsValid:1;
} REG_TPM_STS;

//
// Defines correspond defnitions of TIS 1.3 in TPM_INTF_CAPABILITY
// register at TPM MMIO offset 0x14[30:28]
//
#define INTF_1_21     0
#define INTF_1_3      2
#define INTF_2_0      3
#define INTF_RES      -1

//-----------------------------------------------------------------------------
// CRB I/F related definitions
//-----------------------------------------------------------------------------

// typedef struct _TPM_CONTROL_AREA {
//   UINT32  Request;
//   UINT32  Status;
//   UINT32  Cancel;
//   UINT32  Start;
//   UINT64  R;
//   UINT32  CmdSize;
//   UINT64  CmdAddr;
//   UINT32  RspSize;
//   UINT64  RspAddr;
// } TPM_CONTROL_AREA;

#define TPMCRBBUF_LEN      3968

typedef struct _TPM_CRB {
  UINT32            TPM_LOC_STATE;      // OFF 0
  UINT32            R1;
  UINT32            TPM_LOC_CTRL;       // OFF 0x8
  UINT32            TPM_LOC_STS;        // OFF 0xC
  UINT8             R2[FILL8(0x30, 0xF)];
  UINT64            TPM_CRB_INTF_ID;    // OFF 0x30
  UINT64            TPM_CRB_CTRL_EXT;   // OFF 0x38
  UINT32            TPM_CRB_CTRL_REQ;   // OFF 0x40
  UINT32            TPM_CRB_CTRL_STS;   // OFF 0x44
  UINT32            TPM_CRB_CTRL_CANCEL; // OFF 0x48
  UINT32            TPM_CRB_CTRL_START; // OFF 0x4C
  UINT64            TPM_CRB_CTRL_INT;   // OFF 0x50
  UINT32            TPM_CRB_CTRL_CMD_SIZE; // OFF 0x58
  UINT32            TPM_CRB_CTRL_CMD_LADDR; // OFF 0x5C
  UINT32            TPM_CRB_CTRL_CMD_HADDR; // OFF 0x60
  UINT32            TPM_CRB_CTRL_RSP_SIZE; // OFF 0x64
  UINT64            TPM_CRB_CTRL_RSP_ADDR; // OFF 0x68
  UINT8             R3[FILL8(0x80, 0x6F)];
  UINT8             TPM_CRB_DATA_BUFFER[TPMCRBBUF_LEN]; // OFF 0x80
} TPM_CRB;

//#define CTRL_AREA_ADDR  (UINT32) (TPM_ACCESS_L3 + offsetof(TPM_CRB, TPM_CRB_CTRL))
#define DATA_BUF_ADDR   (UINT32) (TPM_ACCESS_L3 + offsetof(TPM_CRB, TPM_CRB_DATA_BUFFER))

//
// All register bit fields below have two-fold definitions: as
// structures and as constant values. Despite that structure form
// allows convinient way to access individual bits this is possible for
// read accesses only and is too risky for writes.
// For write accesses of TPM registers it is important to control
// presizely the value which is written and the width of register
// accessed. To prevent compiler to make such decisions for us, we
// write values using constant defines.
//
#define LOC_STATE_TPM_ESTABLISHMENT BIT0
#define LOC_STATE_LOC_ASSIGNED      BIT1
#define LOC_STATE_TPM_REG_VALID_STS BIT7

typedef struct {
  UINT32  TpmEstablishment:1;
  UINT32  LocAssigned:1;
  UINT32  ActiveLocality:3;
  UINT32  R:2;
  UINT32  TpmRegValidSts:1;
  UINT32  R2:24;
} REG_TPM_LOC_STATE;

#define LOC_CTRL_REQUEST_ACCESS       BIT0
#define LOC_CTRL_RELINQUISH           BIT1
#define LOC_CTRL_SEIZE                BIT2
#define LOC_CTRL_RESET_ESTABLISHMENT  BIT3

typedef struct {
  UINT32  RequestAccess:1;
  UINT32  Relinquish:1;
  UINT32  Seize:1;
  UINT32  ResetEstablishmentBit:1;
  UINT32  R2:28;
} REG_TPM_LOC_CTRL;

#define LOC_STS_GRANTED               BIT0
#define LOC_STS_BEEN_SEIZED           BIT1

typedef struct {
  UINT32  Granted:1;
  UINT32  BeenSeized:1;
  UINT32  R:30;
} REG_TPM_LOC_STS;

#define CRB_CTRL_REQ_CMD_READY   BIT0
#define CRB_CTRL_REQ_GO_IDLE     BIT1

typedef struct {
  UINT32  CmdReady:1;
  UINT32  GoIdle:1;
  UINT32  R:30;
} REG_TPM_CRB_CTRL_REQ;

#define CRB_CTRL_STS_TPM_STS     BIT0
#define CRB_CTRL_STS_TPM_IDLE    BIT1

typedef struct {
  UINT32  TpmSts:1;
  UINT32  TpmIdle:1;
  UINT32  R:30;
} REG_TPM_CRB_CTRL_STS;

#define CRB_CTRL_CANCEL             1
#define CRB_CTRL_CANCEL_CLEAR       0
#define CRB_CTRL_START              1
#define CRB_CTRL_CMD_COMPLETE       0

//
// Interface ID register - common for MMIO and CRB
//
#define TPM_INTERFACE_ID_INTERFACE_TYPE_FIFO_0  0
#define TPM_INTERFACE_ID_INTERFACE_TYPE_CRB     1
#define TPM_INTERFACE_ID_INTERFACE_TYPE_FIFO_F  0xF

#define TPM_interface_version


typedef struct {
  UINT32  InterfaceType:4;              // [3:0]
  UINT32  InterfaceVersion:4;           // [7:4]
  UINT32  InterfaceCapLocality:1;       // [8]
  UINT32  Res:4;                        // [12:9]
  UINT32  CapTIS:1;                     // [13]
  UINT32  CapCRB:1;                     // [14]
  UINT32  CapIFRes:2;                   // [16:15]
  UINT32  InterfaceSelector:2;          // [18:17]
  UINT32  IntfSelLock:1;                // [19]
  UINT32  Res2:12;                      // [31:20]
} REG_TPM_INTERFACE_ID;

//
// Defines correspond definitions of PCH specification of LT_VER_FTIF
// register in LTCS offset 0x800[19:16]
//
#define TPM_NOT_PRESENT     0
#define TPM_IF_LPC          1
#define TPM_IF_SPI          5
#define TPM_IF_CRB          7
#define TPM_IF_FTPM         7

//
// Defines corresponding to family - don't follow any spec yet.
//
#define TPM_FAMILY_1_2 0
#define TPM_FAMILY_2_0 1

#define TPM_FAILEDSELFTEST 28

//
// TPM Capabilities
//
#define CAP_TPM_FIFO    BIT0
#define CAP_TPM_CRB     BIT1
#define CAP_TPM_FAM12   BIT2
#define CAP_TPM_FAM20   BIT3

//
// TPM capabiiies make flags
//
#ifndef MKF_CAP_TPM_FAM_20
#define MKF_CAP_TPM_FAM_20 1
#endif

#ifndef MKF_CAP_TPM_FAM_12
#define MKF_CAP_TPM_FAM_12 1
#endif

#ifndef MKF_CAP_TPM_ITF_CRB
#define MKF_CAP_TPM_ITF_CRB 1
#endif

#ifndef MKF_CAP_TPM_ITF_FIFO
#define MKF_CAP_TPM_ITF_FIFO 1
#endif

//
// Context flags
//
#define TPM_SHA256_SUPPORTED BIT0
//
// Policy settings
//
#define FAM20_POLICY_USE_EVENT    0
#define FAM20_POLICY_USE_EXTEND   1

//
// SVN Control field
//
#define TPM_AUX_SVN_ACCESS_PERMISSION 0xABCD9876


//
// Timeouts in msec
//
#define TIMEOUT_A     750
#define TIMEOUT_B     2000
#define TIMEOUT_C     750
#define TIMEOUT_D     750

#define PTP_TIMEOUT_A  750
#define PTP_TIMEOUT_B_ 2000
#define PTP_TIMEOUT_C  200
#define PTP_TIMEOUT_D  30



//
// 2 minutes.
//
#define CMD_DURATION  120000
//
// 3 seconds
//
#define SELFTEST_DURATION 3000

#define RESET_TIMING  30

typedef struct _TPM_CONTEXT {
  //
  // Possible width of data for single transfer: 64, 32, 8, 4, 2, 1
  //
  UINT8                                         DataWidth[6];
  UINT32                                        WidthStartIdx;
  UINT32                                        RetryCnt;
  UINT32                                        DataFifo;

  UINT32                                        TotalTransmitted;
  UINT32                                        CountRemaining;
  UINT32                                        BurstRemaining;

  UINT32                                        DrvCode;
  UINT32                                        TpmCode;
  UINT32                                        Family;
  UINT32                                        AuxIndexSize;
//  UINT32                                        RevocationSize;
  UINT32                                        Interface;
  UINT32                                        InterfaceVersion;
  UINT32                                        Flags;
  UINT32                                        Policy;
  UINT32                                        SvnControl;

  //
  // Family dependent functions.
  //
  VOID_ARG_UINT16                               fpCoreTpmStartup;
  VOID_NO_ARG                                   fpCoreTpmDisable;
  //
  // Wrapper family dependent functions.
  //
  VOID_NO_ARG                                   fpCheckAuxProperties;
  VOID_NO_ARG                                   fpCheckPsProperties;
  VOID_NO_ARG                                   fpCheckPoProperties;
  VOID_NO_ARG                                   fpCheckTpmNvProperties;
  VOID_ARG_UINT32_UINT32_UINT32_PUINT8_UINT32   fpTpmNvReadValue;
  VOID_ARG_UINT32_UINT32_UINT32_PUINT8          fpTpmNvWriteValue;
  VOID_ARG_UINT32_PUINT8                        fpTpmPcrRead;
  VOID_NO_ARG                                   fpHandleLegacyPcrFormat;
  VOID_NO_ARG                                   fpHandleDetailAuthPcr17Format;
  VOID_NO_ARG                                   fpHandleDetailAuthPcr18Format;
  VOID_NO_ARG                                   fpTpmFamilySpecificInit;
  VOID_NO_ARG                                   fpVerifyHashStartPcrValue;
  VOID_NO_ARG                                   fpValidateEventLogHeader;
  VOID_NO_ARG                                   fpRecordLogEventHashStart;
  VOID_NO_ARG                                   fpRecordLogEventPcrMapping;
  VOID_NO_ARG                                   fpRefurbishAuxIndex;
  VOID_NO_ARG                                   fpSetAuxHashAlgId;
  VOID_NO_ARG                                   fpEventLogInit;
  //
  // Interface dependent functions.
  //
  VOID_NO_ARG                                   fpTpmCommand;
  VOID_ARG_UINT32                               fpCoreRequestLocality;
  VOID_ARG_UINT32                               fpCoreRelinquishLocality;
  UINT8_ARG_UINT32                              fpCoreLocalityStatus;
  UINT32_ARG_UINT32                             fpCoreWaitAccessValid;


  VOID_NO_ARG                                   fpBtgHashandExtend;
  VOID_NO_ARG                                   fpBtgScrtmExtend;

  UINT8_ARG_UINT32_UINT32_UINT32                fpCoreTpmNvReadValue;

} TPM_CONTEXT;
//
// Structure describing support of hash algorithms
//
#define HASH_ALG_ID_COUNT 5
#define BPT_MAX_SUPPORTED_TPM_BANKS             3

//
// Indexing of HashAlgIdList table. Order of alg IDs is fixed and is
// used in code - don't change indices.
//
#define TPM_ALG_SHA1_IDX    0
#define TPM_ALG_SM3_256_IDX 1
#define TPM_ALG_SHA256_IDX  2
#define TPM_ALG_SHA384_IDX  3
#define TPM_ALG_SHA512_IDX  4

typedef struct _HASH_ALG_ID_DESCR {
  UINT16  HashAlgId;
  UINT16  TpmSup:1;                     // Alg is supported by TPM
  UINT16  PcrImp:1;                     // PCR bank implemented
  UINT16  AcmSup:1;                     // ACM implemented in SW
  UINT16  LcpSup:1;                     // Alg is selecetd by LCP mask in PS/PO
  UINT16  Re:12;
}HASH_ALG_ID_DESCR;

void Wait1ms                        ();
void __Wait1ms                      ();
UINT64  calibrateTsc64              ();

UINT32 WaitCommandReady             ();
UINT32 WaitSelftestDone             ();
UINT32 WaitStatusValid              ();
UINT32 WaitBurstCountReady          ();
UINT32 WaitCommandComplete          ();
UINT32 WaitActiveLocality           (UINT32);

UINT32 WaitCrbLocalityGranted       (UINT32);
UINT32 WaitCrbLocalityRelinquished  (UINT32);
UINT32 WaitCrbTpmIdle               ();
UINT32 WaitCrbCommandReady          ();
UINT32 WaitCrbCommandComplete       ();


void BiosacTpmInterfaceInit         ();
void ClientBiosacTpmInterfaceInit   ();
void SinitTpmInterfaceInit          ();

#ifndef __ASM__

#if     MCP_PLATFORM_TYPE==CLIENT

#if     MCP_CHIPSET_ACM_TYPE==AC_SINIT
#define HookModuleSpecificTpmInterfaceInit SinitTpmInterfaceInit
#else
#define HookModuleSpecificTpmInterfaceInit ClientBiosacTpmInterfaceInit
#endif

#else

#if     MCP_CHIPSET_ACM_TYPE==AC_SINIT
#define HookModuleSpecificTpmInterfaceInit ServerSinitTpmInterfaceInit
#else
#define HookModuleSpecificTpmInterfaceInit ServerBiosacTpmInterfaceInit
#endif

#endif

#endif


#pragma pack (pop)
#endif
