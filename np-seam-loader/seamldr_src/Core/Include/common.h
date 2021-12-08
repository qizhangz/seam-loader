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

#ifndef COMMON_H
#define COMMON_H

#pragma pack (push, 1)
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                          USAGE REQUIREMENTS
// Only general-purpose definitions can be placed in this file,
// free from chipset etc. dependencies.
//
// All definitions must be self-contained, not dependant on information from
// other include files.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#pragma data_seg("EDATA32")
#pragma bss_seg("EDATA32")
#pragma const_seg("EDATA32")
#pragma warning ( disable : 4091 )

#define STRINGIFY(X)  #X
#define TOSTRING(X)   STRINGIFY(X)

#define PORT80     0x80
#define PORT81     0x81
//
// Bit definitions
//
#define BIT0				0x01
#define BIT1				0x02
#define BIT2				0x04
#define BIT3				0x08
#define BIT4				0x010
#define BIT5				0x020
#define BIT6				0x040
#define BIT7				0x080
#define BIT8				0x0100
#define BIT9				0x0200
#define BIT10				0x0400
#define BIT11				0x0800
#define BIT12				0x01000
#define BIT13				0x02000
#define BIT14				0x04000
#define BIT15				0x08000
#define BIT16				0x010000
#define BIT17				0x020000
#define BIT18				0x040000
#define BIT19				0x080000
#define BIT20				0x0100000
#define BIT21				0x0200000
#define BIT22				0x0400000
#define BIT23				0x0800000
#define BIT24				0x01000000
#define BIT25				0x02000000
#define BIT26				0x04000000
#define BIT27				0x08000000
#define BIT28				0x010000000
#define BIT29				0x020000000
#define BIT30				0x040000000
#define BIT31				0x080000000
#define BIT32				0x100000000
#define BIT33				0x200000000
#define BIT34				0x400000000
#define BIT35				0x800000000
#define BIT36				0x1000000000
#define BIT37				0x2000000000
#define BIT38				0x4000000000
#define BIT39				0x8000000000
#define BIT40				0x10000000000
#define BIT41				0x20000000000
#define BIT42				0x40000000000
#define BIT43				0x80000000000
#define BIT44				0x100000000000
#define BIT45				0x200000000000
#define BIT46				0x400000000000
#define BIT47				0x800000000000
#define BIT48				0x1000000000000
#define BIT49				0x2000000000000
#define BIT50				0x4000000000000
#define BIT51				0x8000000000000
#define BIT52				0x10000000000000
#define BIT53				0x20000000000000
#define BIT54				0x40000000000000
#define BIT55				0x80000000000000
#define BIT56				0x100000000000000
#define BIT57				0x200000000000000
#define BIT58				0x400000000000000
#define BIT59				0x800000000000000
#define BIT60				0x1000000000000000
#define BIT61				0x2000000000000000
#define BIT62				0x4000000000000000
#define BIT63				0x8000000000000000

#define BITN(N)      (1 << (N))
//-----------------------------------------------------------------------------
// Misc definitions and helper macros.
//-----------------------------------------------------------------------------

#define TRUE  1
#define FALSE 0
#define NULL  0
#define EQUAL     0
#define NOT_EQUAL 1
#define ZEROS     ((UINT32) 0)
#define ONES      ((UINT32) 0xFFFFFFFF)

#define max(A,B)      (((A) > (B)) ? (A) : (B))
#define min(A,B)      (((A) > (B)) ? (B) : (A))

//#define offsetof(T,I) ((UINT32)((UINT8 *)&((T*)0)->I-(UINT8 *)0))
#define lenof(T,I) sizeof(((T*)0)->I)

#define highdword(R)  (*((UINT32 *)&(R) + 1))
#define highword(R)   (*((UINT16 *)&(R) + 1))
#define highbyte(R)   (*((UINT8 *)&(R) + 1))

#define rounded(R, V) ((((R) % (V)) == 0) ? (R) : (((R) / (V)) * (V)) + (V))
#define rounded4KB(R) rounded((R), (_4KB))

#define IN
#define OUT
#define OPTIONAL

//-----------------------------------------------------------------------------
// Common types
//-----------------------------------------------------------------------------
#define INT8  __int8          //char
#define UINT8  unsigned __int8          //char
#define UINT16 unsigned __int16         //short int
#define UINT32 unsigned __int32         //long
#define UINT64 unsigned __int64         //long long
#define UINTN  UINT64
#define INT32  __int32
#define INT64  __int64
typedef UINT8  *PBYTE;
typedef char   *PSBYTE;
typedef UINT32 *PDWORD;
typedef UINT8 BOOL;
typedef UINT32(*PTR_FUNC_NO_ARG) ();

//
// PAE linear address.
// NOTE - "C" structures containing bit fields are allocated in memory
// starting from low order bit. For instance Off bit field in LADRPAE
// DWORD occupies lowest 12 bits.
//
typedef struct _LADRPAE {
    UINT32 Off : 12;                        // 11:0 - offset
    UINT32 PTIdx : 9;                       // 20:12 - page table index
    UINT32 PDIdx : 9;                       // 29:21 - page directory index
    UINT32 PDPIdx : 2;                      // 31:30 - page directory pointer index.
} LADRPAE;

//
// Content of CRASH register
//
typedef struct _CRASH_REGISTER {
    UINT32  AcmType : 4;                    // 3:0   - Type of ACM
    UINT32  Class : 6;                      // 9:4   - Class of error
    UINT32  Major : 5;                      // 14:10 - Major error code
    UINT32  Source : 1;                     // 15    - Source - ACM or MLE
    UINT32  Minor : 12;                     // 27:16 - Minor error code and progress code
    UINT32  Res : 2;                        // 29:28 - Reserved for future
    UINT32  SwGenerated : 1;                // 30 - Register value is SW generated
    UINT32  Valid : 1;                      // 31 - Register value is valid
}CRASH_REGISTER;

//-----------------------------------------------------------------------------
// Sizes and masks
//-----------------------------------------------------------------------------
#define _64TB            0x400000000000
#define _256GB           0x4000000000
#define _64GB            0x1000000000
#define _8GB             0x200000000
#define _4GB             0x100000000
#define _3GB             0xC0000000
#define _2GB             0x80000000
#define _2016MB          0x7E000000
#define _1_5GB           0x60000000
#define _1GB             0x40000000
#define _512MB           0x20000000
#define _480MB           0x1E000000
#define _448MB           0x1C000000
#define _416MB           0x1A000000
#define _384MB           0x18000000
#define _352MB           0x16000000
#define _320MB           0x14000000
#define _288MB           0x12000000
#define _256MB           0x10000000
#define _224MB           0xE000000
#define _192MB           0xC000000
#define _160MB           0xA000000
#define _128MB           0x8000000
#define _96MB            0x6000000
#define _64MB            0x4000000
#define _60MB            0x3C00000
#define _56MB            0x3800000
#define _52MB            0x3400000
#define _48MB            0x3000000
#define _44MB            0x2C00000
#define _40MB            0x2800000
#define _36MB            0x2400000
#define _32MB            0x2000000
#define _28MB            0x1C00000
#define _24MB            0x1800000
#define _20MB            0X1400000
#define _16MB            0x1000000
#define _15MB            0xF00000
#define _14MB            0xE00000
#define _12MB            0xC00000
#define _8MB             0x800000
#define _4MB             0x400000
#define _3MB             0x300000
#define _2MB             0x200000
#define _1MB             0x100000
#define _512KB           0x80000
#define _256KB           0x40000
#define _384KB           0x60000
#define _192KB           0x30000
#define _172KB           0x2B000
#define _128KB           0x20000
#define _64KB            0x10000
#define _32KB            0x8000
#define _31KB            0x7C00
#define _20KB            0x5000
#define _16KB            0x4000
#define _12KB            0x3000
#define _8KB             0x2000
#define _6KB             0x1800
#define _4KB             0x1000
#define _2KB             0x800
#define _1KB             0x400
#define _64B             0x40
#define _16B             0x10

#define PAGE4K           0x1000
#define PAGE2M           0x200000
#define PAGE4M           0x400000

#define _4GB_MASK  (~(_4GB -1))
#define _2GB_MASK  (~(_2GB -1))
#define _1GB_MASK  (~(_1GB - 1))
#define _512MB_MASK  (~(_512MB - 1))
#define _256MB_MASK  (~(_256MB - 1))
#define _128MB_MASK  (~(_128MB - 1))
#define _64MB_MASK  (~(_64MB - 1))
#define _16MB_MASK  (~(_16MB - 1))
#define _8MB_MASK  (~(_8MB - 1))
#define _4MB_MASK  (~(_4MB - 1))
#define _2MB_MASK  (~(_2MB - 1))
#define _1MB_MASK  (~(_1MB - 1))

#define _512KB_MASK  (~(_512KB - 1))
#define _256KB_MASK  (~(_256KB - 1))
#define _128KB_MASK  (~(_128KB - 1))
#define _64KB_MASK  (~(_64KB - 1))
#define _32KB_MASK  (~(_32KB - 1))
#define _16KB_MASK  (~(_16KB - 1))
#define _8KB_MASK  (~(_8KB - 1))
#define _4KB_MASK  (~(_4KB - 1))
#define _1KB_MASK  (~(_1KB - 1))
#define _16B_MASK  (~(_16B - 1))


//-----------------------------------------------------------------------------
// Maximal values fpr types
//-----------------------------------------------------------------------------
#define MAX_QWORD ((UINT64) 0xFFFFFFFFFFFFFFFF)
#define MAX_DWORD ((UINT32) 0xFFFFFFFF)
#define MAX_WORD ((UINT16) 0xFFFF)
#define MAX_BYTE ((UINT8) 0xFF)

//-----------------------------------------------------------------------------
// Module types - SINIT or BIOS AC.
//-----------------------------------------------------------------------------
#define AC_BIOSAC        0
#define AC_SINIT         1
#define AC_SINIT_REV     9

//-----------------------------------------------------------------------------
// ACM module header "FLAGS" field control bits.
// BIT15 is set to indicate DEBUG signed module.
// BIT14 is set to indicate NPW module. Flags are intended to be ORed
// if needed.
//-----------------------------------------------------------------------------
#define MF_PRODUCTION         0
#define MF_DEBUG              BIT15
#define MF_PROD_WORTHY        0
#define MF_NOT_PROD_WORTHY    BIT14


//-----------------------------------------------------------------------------
// Leaf value (in eax) calling GETSEC
//-----------------------------------------------------------------------------
#define CAPABILITIES		0
#define ENTERACCS				2
#define EXITAC				  3
#define SENTER				  4
#define SEXIT				    5
#define PARAMETERS			6
#define SMCTRL					7
#define WAKEUP					8


//-----------------------------------------------------------------------------
// Max loop count for Param5 search
//-----------------------------------------------------------------------------
#define MAX_PARAM5_COUNT	10     

//-----------------------------------------------------------------------------
// Control registers
//-----------------------------------------------------------------------------
//
// CR0 bits
//
#define CR0_PE     BIT0
#define CR0_EM     BIT2
#define CR0_TS     BIT3
#define CR0_ET     BIT4
#define CR0_NE     BIT5
#define CR0_WP     BIT16
#define CR0_NW     BIT29
#define CR0_CD     BIT30
#define CR0_PG     BIT31

//
// CR4 bits
//
#define CR4_VME       BIT0
#define CR4_PVI       BIT1
#define CR4_TSD       BIT2
#define CR4_DE        BIT3
#define CR4_PSE       BIT4
#define CR4_PAE       BIT5
#define CR4_MSE       BIT6
#define CR4_PGE       BIT7
#define CR4_PCE       BIT8
#define CR4_OSFXSR    BIT9
#define CR4_OSXMMEXCPT BIT10
#define CR4_LA57      BIT12
#define CR4_VMXE      BIT13
#define CR4_SMXE      BIT14
#define CR4_FSGSBASE  BIT16
#define CR4_OSXSAVE   BIT18
#define CR4_SMEP      BIT20
#define CR4_SMAP      BIT21
#define CR4_CET       BIT23

//-----------------------------------------------------------------------------
// CPUID family/model/stepping masks
//-----------------------------------------------------------------------------

#define CPUID_XTENDED_FAMILY    (0xFF << 20)
#define CPUID_XTENDED_MODEL     (0xF << 16)
#define CPUID_TYPE              (0x3 << 12)
#define CPUID_FAMILY            (0xF << 8)
#define CPUID_MODEL             (0xF << 4)
#define CPUID_STEPPING          (0xF << 0)
#define CPUID_FMS_MASK          (CPUID_XTENDED_FAMILY + CPUID_XTENDED_MODEL + CPUID_FAMILY + CPUID_MODEL)

//-----------------------------------------------------------------------------
// SHAXXX hash
//-----------------------------------------------------------------------------

#define H_LEN			20
#define H256_LEN  32
#define HASH_ALG_SHA1    0x0
#define HASH_ALG_SHA256  0x1

//-----------------------------------------------------------------------------
// MSRs. Place only architectural, not CPU specific MSRs here
//-----------------------------------------------------------------------------

#define MSR_IA32_PLATFORM_ID     0x017
#define MSR_IA32_APIC_BASE       0x01B
#define MSR_PLATFORM_INFO        0xCE
//
// Local APIC register offsets.
//
#define SPURIOUS_INTERRUPT_VECTOR_REG 0xF0
#define LVT_CMCI_REG                  0x2F0
#define LVT_ICR_LO                    0x300
#define LVT_ICR_HI                    0x310
#define LVT_TIMER_REG                 0x320
#define LVT_THERMAL_MONITOR_REG       0x330
#define LVT_LINT0_REG                 0x350
#define LVT_LINT1_REG                 0x360
#define LVT_ERROR_REG                 0x370

typedef struct _LAPIC_REG_SAVE {
    UINT32 RegIdx;
    UINT32 RegVal;
} LAPIC_REG_SAVE;

#define MSR_CORE_THREAD_COUNT    0x035
#define MSR_IA32_FEATURE_CONTROL 0x03A
#define     FEATURE_CONTROL_LOCK BIT0
#define     VMXON_IN_SMX_ENABLE  BIT1
#define     VMXON_OUT_SMX_ENABLE BIT2
#define     SENTER_ENABLE        0xFF00
#define     SGX_ENABLE           (BIT18 + BIT0)
#define MSR_IA32_BIOS_SIGN_ID    0x08B
#define MSR_IA32_SMM_MONITOR_CTL 0x09B
#define MSR_IA32_MTRRCAP         0x0FE
#define VCNT_MASK              0xFF

#define MSR_BTG_BOOT_POLICY      0x130
#define MSR_BTG_BP_KEY0          0x131
#define MSR_BTG_BP_KEY1          0x132
#define MSR_BTG_BP_KEY2          0x133
#define MSR_BTG_BP_KEY3          0x134
#define MSR_BTG_BP_KEY4          0x135
#define MSR_BTG_BP_KEY5          0x136
#define MSR_BTG_BP_RSVD3         0x137
#define MSR_BTG_BOOT_STATUS      0x138
#define MSR_BTG_SACM_INFO        0x13A
#define LT_SX_FUSE             BIT34
#define BTG_FUSE               BIT32
#define NEM_ENABLED            BIT0

#define SMRR_0  0
#define SMRR_2  2

#define MSR_SGX_DEBUG_MODE       0x503
#define MSR_IA32_MISC_ENABLES	 0x1A0
#define MSR_SMRR_PHYBASE         0x1F2
#define MSR_SMRR_PHYMASK         0x1F3
#define MSR_SMRR2_PHYBASE        0x1F6
#define MSR_SMRR2_PHYMASK        0x1F7
#define MSR_IA32_MTRR_PHYSBASE0  0x200
#define MSR_IA32_MTRR_PHYSMASK0  0x201
#define MSR_IA32_MTRR_PHYSBASE1  0x202
#define MSR_IA32_MTRR_PHYSMASK1  0x203
#define MSR_IA32_MTRR_PHYSBASE2  0x204
#define MSR_IA32_MTRR_PHYSMASK2  0x205
#define MSR_IA32_MTRR_PHYSBASE8  0x210
#define MSR_IA32_MTRR_PHYSMASK8  0x211
#define MSR_IA32_MTRR_PHYSBASE9  0x212
#define MSR_IA32_MTRR_PHYSMASK9  0x213
#define LO_PHYS_BASE_MASK      0xFFFFF000
#define PHYS_BASE_TYPE_MASK    0xFF
#define PHYS_MASK_VALID        BIT11
#define LO_PHYS_MASK_MASK      0xFFFFF000

#define MTRR_FIX_64K_00000       0x250
#define MTRR_FIX_16K_80000       0x258
#define MTRR_FIX_16K_A0000       0x259
#define MTRR_FIX_4K_C0000        0x268
#define MTRR_FIX_4K_C8000        0x269
#define MTRR_FIX_4K_D0000        0x26A
#define MTRR_FIX_4K_D8000        0x26B
#define MTRR_FIX_4K_E0000        0x26C
#define MTRR_FIX_4K_E8000        0x26D
#define MTRR_FIX_4K_F0000        0x26E
#define MTRR_FIX_4K_F8000        0x26F

#define BUS_WIDTH_36             0
#define BUS_WIDTH_ACTUAL         1

#define MSR_NO_EVICT_MODE        0x2E0

#define MSR_LTCTRLSTS            0x2E7
#define CONFIGLOCK             BIT0
#define SCHECK_PASS            BIT4
#define CONFIGUNLOC            BIT16
#define SCHECK_PASS_CLR        BIT20

#define MSR_EDRAM_ACM            0x2E8
#define EDRAM_EXISTS           BIT2
#define EDRAM_IS_DISABLED      BIT1
#define FORCE_EDRAM_DISABLED   BIT0

#define MSR_IA32_MTRR_DEF_TYPE   0x2FF
#define DEF_MEM_TYPE_MASK      0x7
#define DEF_FIX_MTRR_ENABLE    BIT10
#define DEF_VAR_MTRR_ENABLE    BIT11

#define MSR_BUS_NUM	         0x300

#define MSR_IA32_VMX_BASIC       0x480
#define MSR_IA32_VMX_MISC        0x485

//
// PRMRR_BASE location changed to 0x2A0
//
#define MSR_PRMRR_BASE           0x2A0
#define MSR_PRMRR_MASK           0x1F5

#define MSR_BIOS_DONE            0x151

#define MSR_FLEX_RATIO           0x194
#define B_OVERCLOCKING_LOCK    BIT20
#define B_OVERCLOCKING_BINS    (0x7 << 17)

#define MSR_BIOS_SE_SVN          0x302

#define MSR_MKTME_ACTIVATE       0x982

//
// BIOS_SE_SVN_MSR (0x302) structure
//
typedef union {
    struct {
        UINT8 reserved0;                    // bits [7:0]
        UINT8 reserved1;                    // bits [15:8]
        UINT8 sinit_svn;                    // bits [23:16]
        UINT8 sclean_svn;                   // bits [31:24]
        UINT8 anc_svn;                      // bits [39:32]
        UINT8 pfat_svn;                     // bits [47:40]
        UINT8 reserved6;                    // bits [55:48]
        UINT8 seamldr_svn;                  // bits [63:56]
    };
    UINT64 raw;
} bios_se_svn_msr_u;

//   #define SINIT_SE_SVN_MASK      (~0xFF0000)
//   #define BIOSAC_SE_SVN_MASK     (~0xFF000000)
//   #define BTG_SE_SVN_MASK        (~0xFF00000000)
//   #define SINIT_SE_SVN_SHIFT     16
//   #define BIOSAC_SE_SVN_SHIFT    24

//
// FLOOR and CEILING definitions. CEILING use requires 64 bit output
// values. This is because output value may become over 4GB even if
// enties are below 4GB.
// Example:
//      UINT64 OutVar;   
//      UINT32 InVar;
// 			OutVar = CEILING(InVar,_4KB);


#define FLOOR(addr,interval) ((((UINT32)addr))&(~((UINT32)((interval)-1))))
#define CEILING(addr,interval) (FLOOR((addr),(interval)) == addr ? addr : ((UINT64)FLOOR((addr),(interval)) + (UINT64)interval))


#define MTRR_MEMORY_TYPE_UC      00
#define MTRR_MEMORY_TYPE_WC      01
#define MTRR_MEMORY_TYPE_WT      04
#define MTRR_MEMORY_TYPE_WP      05
#define MTRR_MEMORY_TYPE_WB      06

#define MTRR_MIN_RANGE           0x1000

#define MSR_DEBUG_INTERFACE      0xC80
#define DEBUG_HAS_OCCURED      BIT31
#define DEBUG_LOCK             BIT30
#define DEBUG_ENABLE           BIT0

#define IA32_EFER_MSR            0xC0000080
#define N_IA32_EFER_SCE        BIT0
#define LME                    BIT8
#define N_IA32_EFER_LMA        BIT10
#define N_IA32_EFER_NXE        BIT11



//-----------------------------------------------------------------------------
// TXT register space
//-----------------------------------------------------------------------------

#define LT_PUB_BASE		0xFED30000
#define LT_PRV_BASE	  0xFED20000

#define FILL64(H,L) ((((H)-(L))/8)-1)
#define FILL8(H,L) (((H)-(L))-1)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

typedef struct _TXT {
    UINT64 LT_STS;                        // 0000h
    UINT64 LT_ESTS;                       // 0008h
    UINT64 LT_THREADS_EXISTS;             // 0010h
    UINT64 R1_0;                          // 0018h
    UINT64 LT_THREADS_JOIN;               // 0020h
    UINT64 R1_1;                          // 0028h
    UINT64 LT_CRASH;                      // 0030h
    UINT64 LT_CMD_SYS_RESET;              // 0038h
    UINT64 LT_CMD_OPEN_PRIVATE;           // 0040h
    UINT64 LT_CMD_CLOSE_PRIVATE;          // 0048h
    UINT64 LT_CRASH2;                     // 0050h

    UINT64 R2_0[FILL64(0xA0, 0x50)];
    UINT64 LT_SPAD;                       // 00A0h

    UINT64 R3_0[FILL64(0x100, 0xA0)];
    UINT64 LT_VER_FSBIF;                  // 0100h
    UINT64 R3_1;                          // 0108h
    UINT64 LT_DIDVID;                     // 0110h
    UINT64 LT_EID;                        // 0118h

    UINT64 R4_0[FILL64(0x200, 0x118)];
    UINT64 LT_VER_EMIF;                   // 0200h
    UINT64 R4_1;                          // 0208h
    UINT64 LT_CMD_LOCKMEM_CONFIG;         // 0210h
    UINT64 LT_CMD_UNLOCK_MEM_CONFIG;      // 0218h
    UINT64 LT_CMD_UNLOCK_MEMORY;          // 0220h
    UINT64 R4_2;                          // 0228h
    UINT64 LT_LOCK_BASE;                  // 0230h
    UINT64 LT_UNLOCK_BASE;                // 0238h
    UINT64 R4_3;                          // 0240h
    UINT64 R4_4;                          // 0248h
    UINT64 LT_CMD_CACHE_INVALIDATE;       // 0250h
    UINT64 LT_CMD_FLUSH_WB;               // 0258h
    UINT64 LT_NODMA_BASE;                 // 0260h
    UINT64 LT_NODMA_SIZE;                 // 0268h
    UINT64 LT_SINIT_BASE;                 // 0270h
    UINT64 LT_SINIT_SIZE;                 // 0278h
    UINT64 LT_CMD_LOCK_PMRC;              // 0280h
    UINT64 LT_CMD_UNLOCK_PMRC;            // 0288h
    UINT64 LT_MLE_JOIN;                   // 0290h
    UINT64 R4_5;                          // 0298h
    UINT64 LT_BLOCKMAP_CAP;               // 02A0h
    UINT64 LT_BLOCKMAP_CNF;               // 02A8h
    UINT64 LT_BLOCKMAP_POINTER;           // 02B0h
    UINT64 R4_6;                          // 02B8h
    UINT64 LT_CMD_BLOCKMAP_EN;            // 02C0h
    UINT64 LT_CMD_BLOCKMAP_DIS;           // 02C8h
    UINT64 LT_CMD_NODMA_CACHE_EN;         // 02D0h
    UINT64 LT_CMD_NODMA_CACHE_DIS;        // 02D8h
    UINT64 LT_CMD_NODMA_TABLE_PROTECT_EN; // 02E0h
    UINT64 LT_CMD_NODMA_TABLE_PROTECT_DIS; // 02E8h
    UINT64 LT_CMD_MEM_CONFIG_CHECKED;     // 02F0h
    UINT64 R4_7;                          // 02F8h
    UINT64 LT_HEAP_BASE;                  // 0300h
    UINT64 LT_HEAP_SIZE;                  // 0308h
    UINT64 LT_MSEG_BASE;                  // 0310h
    UINT64 LT_MSEG_SIZE;                  // 0318h
    UINT64 LT_SCRATCHPAD_0;               // 0320h
    UINT32 LT_BIOSACMCode;                // 0328h
    UINT32 LT_ACM_BIOS_POLICY;            // 032ch
    UINT64 LT_DPR;                        // 0330h
    UINT64 R4_8;                          // 0338h
    UINT64 LT_FIT_STATUS;                 // 0340h
    UINT64 R4_9;                          // 0348h
    UINT64 LT_INCREMENT;                  // 0350h
    UINT64 LT_SPAD_3;                     // 0358h
    UINT64 LT_SCRATCHPAD_4;               // 0360h
    UINT64 LT_SCRATCHPAD_5;               // 0368h
    UINT64 LT_INCREMENT_2;                // 0370h
    UINT64 LT_SCRATCHPAD_6;               // 0378h
    UINT64 LT_CMD_OPEN_LOCALITY1;         // 0380h
    UINT64 LT_CMD_CLOSE_LOCALITY1;        // 0388h
    UINT64 LT_CMD_OPEN_LOCALITY2;         // 0390h
    UINT64 LT_CMD_CLOSE_LOCALITY2;        // 0398h
    UINT64 LT_CMD_OPEN_LOCALITY3;         // 03A0h
    UINT64 LT_CMD_CLOSE_LOCALITY3;        // 03A8h

    UINT64 R5_0[FILL64(0x400, 0x3A8)];
    UINT64 LT_PUBLIC_KEY;                 // 0400h

    UINT64 R5_1[FILL64(0x608, 0x400)];
    UINT64 LT_ESTS_SET;                   // 0608h
    UINT64 LT_EXISTS_SET;                 // 0610h
    UINT64 R5_2;                          // 0618h
    UINT64 LT_JOINS_SET;                  // 0620h

    UINT64 R6_0[FILL64(0x670, 0x620)];
    UINT64 LT_SCLEAN_SET;                 // 0670h

    UINT64 R7_0[FILL64(0x6A0, 0x670)];
    UINT64 LT_SPAD_SET;                   // 06A0h

    UINT64 R8_0[FILL64(0x710, 0x6A0)];
    UINT64 LT_EXISTS_CLEAR;               // 0710h
    UINT64 R8_1;                          // 0718h
    UINT64 LT_JOINS_CLEAR;                // 0720h

    UINT64 R9_0[FILL64(0x770, 0x720)];
    UINT64 LT_SCLEAN_CLEAR;               // 0770h

    UINT64 R10_0[FILL64(0x7A0, 0x770)];
    UINT64 LT_SPAD_CLEAR;                 // 07A0h

    UINT64 R11_0[FILL64(0x800, 0x7A0)];
    UINT64 LT_VER_FTIF;                   // 0800h
    UINT64 R11_1;                         // 0808h
    UINT64 LT_PCH_DIDVID;                 // 0810h

    UINT64 R12_0[FILL64(0x880, 0x810)];
    UINT64 LT_UCS;                        // 880h

    UINT64 R13_0[FILL64(0x8E0, 0x880)];
    UINT64 LT_CMD_SECRETS;                // 08E0h
    UINT64 LT_CMD_NO_SECRETS;             // 08E8h
    UINT64 LT_E2STS;                      // 08F0h
    UINT64 R13_1;                         // 08F8h
    UINT32 LT_FT_REGS1;                   // 0900h
    UINT32 LT_FT_REGS2;                   // 0904h

    UINT64 R14_0[FILL64(0xD80, 0x900)];
    UINT64 LT_SEQ_START;                  // 0D80h
    UINT64 R14_2;                         // 0D88h
    UINT64 LT_SEQ_DONE;                   // 0D90h
                                          //
                                          // Added with Sunrise Point and Lewisburg
                                          // These all behave like INCREMENT2 / SPAD6 pair
                                          //
    UINT64 LT_INCREMENT_3;                // 0D98h
    UINT64 LT_SCRATCHPAD_7;               // 0DA0h
    UINT64 LT_INCREMENT_4;                // 0DA8h
    UINT64 LT_SCRATCHPAD_8;               // 0DB0h
    UINT64 LT_INCREMENT_5;                // 0DB8h
    UINT64 LT_SCRATCHPAD_9;               // 0DC0h
    UINT64 LT_INCREMENT_6;                // 0DC8h
    UINT64 LT_SCRATCHPAD_10;              // 0DD0h
    UINT64 LT_INCREMENT_7;                // 0DD8h
    UINT64 LT_SCRATCHPAD_11;              // 0DE0h
    UINT64 LT_INCREMENT_8;                // 0DE8h
    UINT64 LT_SCRATCHPAD_12;              // 0DF0h
} TXT;

//
// Selected register bit structures.
//

typedef struct {
    UINT32  SenterDone_Sts : 1;
    UINT32  SexitDone_Sts : 1;
    UINT32  Reserved : 2;
    UINT32  MemUnlock_Sts : 1;
    UINT32  BaseLocked_Sts : 1;
    UINT32  Reserved2 : 1;
    UINT32  PrivateOpen_Sts : 1;
    UINT32  Reserved3 : 3;
    UINT32  MemConfigOk_Sts : 1;
    UINT32  Reserved4 : 2;
    UINT32  Locality3Open_Sts : 1;
    UINT32  Locality1Open_Sts : 1;
    UINT32  Locality2Open_Sts : 1;
    UINT32  SeqInProgress : 1;
    UINT32  Reserved5 : 14;
} REG_STS;

typedef struct {
    UINT32  Poison_Sts : 1;
    UINT32  Rogue_Sts : 1;
    UINT32  Memory_Attack : 1;
    UINT32  Reserved : 2;
    UINT32  Aliasi_Fault : 1;
    UINT32  Wakeerror_Sts : 1;
    UINT32  Reserved2 : 25;
} REG_ESTS;

typedef struct {
    struct {
        UINT32  Sleep_Entry_Error_Sts : 1;
        UINT32  Secrets_Sts : 1;
        UINT32  Block_Mem_Sts : 1;
        UINT32  Reset_Sts : 1;
        UINT32  Reserved : 28;
    } LowWrd;
    struct {
        UINT32  Reset_Policy : 1;
        UINT32  Reserved2 : 31;
    } HighWrd;
} REG_E2STS;





//-----------------------------------------------------------------------------
// ACM - PE2BIN communication area
//-----------------------------------------------------------------------------
typedef struct _COM_DATA {
    UINT32 Data64Start;                   // Offset of 64-bit data start (and Code32End)
    UINT32 Code64Start;                   // Offset of 64-bit code start
    UINT32 Code64End;                     // Offset of 64-bit code end
    UINT32 Code64Entry;                   // Offset of 64-bit code entry point
    UINT32 StkStart;                      // Offset of stack start
    UINT32 Code32Start;                   // Offset of code segment start.
} COM_DATA;

//-----------------------------------------------------------------------------
// LTSX / Client TXT origin
//-----------------------------------------------------------------------------
#define LTSX_FUSED            BIT0
#define CLIENT_TXT_FUSED       0



//-----------------------------------------------------------------------------
// Error handling related defines.
//-----------------------------------------------------------------------------

//
// Crash register bit masks. Sometimes use of CRASH_REGISTER structure
// defined above is not convinient. Here are equivalent bit definitions.
//

#define CRASH_CODE_VALID 0x80000000
#define CRASH_CODE_SW_GENERATED 0x40000000

#define CPU_AUTH_ERR 0x7
#define CRASH_CODE_CPU_AUTH_ERR CRASH_CODE_VALID + CPU_AUTH_ERR

//
//  Error context variable.
//
typedef struct _ERR_CONTEXT {
    UINT32 Class;
    UINT32 Major;
    UINT32 Minor;
} ERR_CONTEXT;

//
// Value passed to error handler to indicate that it has to retrieve
// respective error/class code from error context variable.
//
#define VOID -1

//
// Behavior of TPM access functions when error is detected
//
#define RESET_ON_ERROR                  0
#define NO_RESET_ON_ERROR               1

//
// Trampoline code related
//
#define GENERIC_CALL                    0
#define TRAMPOLINE_CALL                 1


#define RFA_VALID           BIT15      
#define LBL_ACM               BIT0

//-----------------------------------------------------------------------------
// Include makeflag here to use it in preprocessing of
// platform-specific selections.
// Late placement of makeflag.h files allows use in it of common
// definitions.
//-----------------------------------------------------------------------------

#ifndef __ASM__

#define TRAPLOOP(X) { volatile int foo = (X); while (foo) ; }

#ifdef MKF_USE_MAKEFLAG_H
#include <makeflag.auto>
#include <Benchmark.h>
//#include <Trace.h>
#include <Mmio.h>
#include <Pci.h>
#endif

#endif

//-----------------------------------------------------------------------------
// Interface
//-----------------------------------------------------------------------------

typedef UINT32(*UINT32_NO_ARG) ();
typedef UINT16(*UINT16_NO_ARG) ();
typedef UINT8(*UINT8_ARG_UINT32) (UINT32);
typedef UINT8* (*UINT8_ARG_UINT32_UINT32_UINT32) (UINT32, UINT32, UINT32);
typedef UINT32(*UINT32_ARG_UINT32) (UINT32);
typedef void(*VOID_NO_ARG) ();
typedef void(*VOID_ARG_UINT32) (UINT32);
typedef void(*VOID_ARG_UINT16) (UINT16);
typedef void(*VOID_ARG_PUINT8_UINT32) (UINT8 *, UINT32);
typedef void(*VOID_ARG_UINT32_PUINT8) (UINT32, UINT8 *);
typedef void(*VOID_ARG_UINT32_UINT32) (UINT32, UINT32);
typedef void(*VOID_ARG_UINT32_UINT32_UINT32) (UINT32, UINT32, UINT32);
typedef void(*VOID_ARG_UINT32_UINT32_UINT32_PUINT8_UINT32) (UINT32, UINT32, UINT32, UINT8 *, UINT32);
typedef void(*VOID_ARG_UINT32_UINT32_UINT32_PUINT8) (UINT32, UINT32, UINT32, UINT8 *);
typedef void(*VOID_ARG_UINT32_UINT32_PVOID_UINT32_PUINT8) (UINT32, UINT32, void *, UINT32, UINT8 *);

typedef void* (*COPY_DATA_FUNC)(void *, const void *, UINT32);

void    commonAcmEntryPoint               ();
void    ProjectAcmEntryPoint               ();
void    AcmEntryPoint                      ();
void   *memcpy                             (void *, const void *, UINT32);

void   *FastCopyDataWithWidths(void *, const void *, UINT32, UINT32 *);
void   *FastCopyData(void *, const void *, UINT32);
void    FastChunkTransfer(UINT8 *, UINT8 *, UINT32);

UINT32  copyDataChkBoundsWithFunc(void *, const void *, UINT32, UINT32, UINT32, COPY_DATA_FUNC);
#define copyDataChkBounds(TO,FROM,LEN,MAX,KIND) copyDataChkBoundsWithFunc((TO),(FROM),(LEN),(MAX),(KIND),FastCopyData)
#define copyData(TO,FR,LEN)                FastCopyData((TO),(FR),(LEN))


UINT32  compareHashX(UINT8 *, UINT8 *, UINT16);
UINT32  compareData(UINT8 *, UINT8 *, UINT32);
UINT32  isZeroHashX(UINT8 *, UINT16);
void    MemFill(UINT8 *, UINT32, UINT32);
void    Movdir64B(const void *, void *);
void    CoreBuildPml5Tables(UINT32);
void    CoreBuildPml4Tables(UINT32);
void    CoreBuildPml4TablesShort(UINT32);
void    CoreBuildPseTables(UINT32);
void    EnablePaging();
void    CoreAcmErrorHandler(UINT32, UINT32);
void    ProgressMark(UINT8);
void    LtReset();
void    __ErrorHandler(UINT32, UINT32);
void    cpyAndZap(UINT8*, UINT32, UINT8 *, UINT32);
void    ChipsetCpuPresenceTest();
void    __readMsr(UINT32, UINT32 *, UINT32 *);
UINT64  __readMsr64(UINT32);
UINT64  ReadMsr64WithInput(UINT32 msr_addr, UINT32 edx_in, UINT32 eax_in);
void    __writeMsr(UINT32, UINT32, UINT32);
void    __writeMsr64(UINT32, UINT64);
void    _ud2(void);
UINT64  readTsc64();
void    CheckMTRR(UINT32);
void    CheckLtCpu();
void    VerifyMicrocode(UINT32);
void    SinitExitPoint();
void    BiosacExitPoint();
void    StartupExitPoint();
void    SinitLtsxExitPoint();
void    SinitRtcCheck();

#ifndef __ASM__

#if TRACE_WITH_FUNCTIONS & TRACE_MSR

#define readMsr(A,B,C) do { \
  UINT32 REG1,REG2; \
   __readMsr((A),(B),(C)); \
   REG1 = REG2 = 0; \
  if ((B)) \
      REG1 = *((UINT32 *)(B)); \
  if ((C)) \
    REG2 = *((UINT32 *)(C)); \
  TRACEFUNCTION(TRACE_MSR,"MSRRD(%s#%d): %04x=%08x%08x\n", __FUNCTION__, __LINE__, (A),REG1,REG2); \
} while(0)

static __inline UINT64 _readMsr64(UINT32 addr, const char *name, UINT32 lineNumber)
{
    UINT64 dummy;
    dummy = __readMsr64(addr);
    TRACEFUNCTION(TRACE_MSR, "MSRRD(%s#%d): %04x=%L\n", name, lineNumber, addr, dummy);
    return dummy;
}

#define readMsr64(A)                        _readMsr64(A,__FUNCTION__,__LINE__)

#define writeMsr(A,B,C) do { \
  TRACEFUNCTION(TRACE_MSR,"MSRWR(%s#%d): %04x=%08x%08x\n", __FUNCTION__, __LINE__, (A),(B),(C)); \
  __writeMsr((A),(B),(C)); \
  } while(0)

#else
#define readMsr(A,B,C)                    __readMsr((A),(B),(C))
#define readMsr64(A)                      __readMsr64((A))
#define writeMsr(A,B,C)                   __writeMsr((A),(B),(C))
#define writeMsr64(A, B)                      __writeMsr64((A), (B))
#endif

#if TRACE_WITH_FUNCTIONS & TRACE_ERRORHANDLER
#define ErrorHandler(A,B) do { \
  TRACEFUNCTION(TRACE_ERRORHANDLER,"ERROR-HANDLER(%s#%d)\n", __FUNCTION__, __LINE__); \
  __ErrorHandler((A),(B)); \
  } while(0)
#else
#define ErrorHandler(A,B)                 __ErrorHandler((A),(B))
#endif

#endif

UINT64  shiftLeft64(UINT64, UINT32);

UINT32  BitScanForward64(UINT32 *, UINT64);
UINT32  BitScanReverse64(UINT32 *, UINT64);
UINT32  BitScanForward64AndReset(UINT32 *, UINT64 *);
UINT32  BitScanReverse64AndReset(UINT32 *, UINT64 *);
UINT32  BitScanForwardAndReset(UINT32 *, UINT32 *);
UINT32  BitScanReverseAndReset(UINT32 *, UINT32 *);
void    SetupCpuFeatures();
void    RestoreCpuFeatures();
UINT32  GetSgxFusingInfo();
void    SgxProtection();
void    Update_CRx();

void    x86_copy8b(UINT8 *, UINT8 *);
void    x86_copy32b(UINT8 *, UINT8 *);
void    x86_copy64b(UINT8 *, UINT8 *);


typedef UINT32(*GET_DATA)                (UINT32, UINT32 *, UINT32 *);
//void    getPcrValue                       (UINT32, UINT8 *);
UINT32  GetSmBase();
UINT32  strLen(char *);

void    SaveLapicRegisters(UINT32);
void    RestoreLapicRegisters(UINT32);
UINT32  GetMaxPhyAddr();
UINT64  GetPhyAddrMask();
UINT32  GetCpuFms();
UINT32  Swap(void *, UINT32);

#ifndef __BYTESWAP__
UINT32  _byteswap_ulong(UINT32);
UINT64  _byteswap_uint64(UINT64);
#define __BYTESWAP__
#endif

#define HOOK(A) Hook##A

#ifndef __ASM__
#define CLIENT 1
#define SERVER 2
#endif

#ifdef MKF_LPSS_UART_SUPPORT
#define HookSerialout PchUartSerialOut
#else
#define HookSerialout ComSerialOut
#endif

UINT32  getsecParameters(UINT32 *, UINT32 *, UINT32 *, UINT32);
void    LongJump(void *);
UINT32  RdrandProc();
void    GetRandomDwords(UINT32 *buf, UINT32 num_dwords);

// Macro for old usage
#define GetRandom(A)                      GetRandomDwords((UINT32 *)(A), (H256_LEN/4))

void    WaitFixedTime(UINT32);
#define Wait100ms() WaitFixedTime(100)
#define Wait1s()    WaitFixedTime(1000)

UINT32  TinyWaitRegisterValue(UINT32, UINT32, UINT32, UINT32, UINT32, UINT32);
#define In10USEC  0
#define In1MSEC   1

UINT32  BaseWaitMsrValue(UINT32, UINT64, UINT32, UINT64, UINT32, UINT32);
#define WaitMsrValue(A,B,C,D,E)     BaseWaitMsrValue(A,B,C,D,E,In1MSEC)
#define TinyWaitMsrValue(A,B,C,D,E) BaseWaitMsrValue(A,B,C,D,E,In10USEC)

UINT32  __WaitRegisterBitSet(UINT32, UINT32);
UINT32  __WaitRegisterBitClear(UINT32, UINT32);
UINT32  SafeAdd(UINT32, void *, void *, void *);

#ifndef HI_SMRR_MASK
#define HI_SMRR_MASK                0
#endif
#ifndef HI_SMRR2_MASK
#define HI_SMRR2_MASK               0
#endif

extern ERR_CONTEXT ErrCtx;
extern UINT8  stackStart[];
extern UINT32 CodeSegmentStart;
extern UINT32 PageDirectoryBase;
extern UINT32 ChipsetAcmType;
extern UINT32 NotProductionWorthy;
extern UINT8  MleDigest[H_LEN];
extern UINT32 TxtErrorRegister;
extern UINT32 Ltsx_ClientTxt;
extern UINT64 LtCtlSts;
extern UINT64 ClockCount;
extern UINT32 CrashCode;
extern UINT32 MaxSupportedBlockWidthIndex;

//
// Definition help catch error at build time.
//
#define C_ASSERT(e)  typedef char ___C_ASSERT___[e?1:-1]

//#define SIZE_OF_FIELD(TYPE, Field) (sizeof(((TYPE *)0)->Field))

#pragma pack (pop)
#endif

