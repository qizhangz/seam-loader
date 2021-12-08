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
#include "Trace.h"

#define VZEROUPPER        __asm _emit 0xC5 \
                          __asm _emit 0xF8 \
                          __asm _emit 0x77

// movq mm0, QWORD PTR [eax]     // 8 bytes, MMX
// 0f 6f 00

// movq QWORD PTR [eax], mm0     // 8 bytes, MMX
// 0f 7f 00

// movdqu xmm0, XMMWORD PTR [eax]       // 16 bytes, SSE2
// f3 0f 6f 00

#define MOV_EAX_TO_XMM0   __asm _emit 0xF3 \
                          __asm _emit 0x0F \
                          __asm _emit 0x6F \
                          __asm _emit 0x00

// movdqu XMMWORD PTR [eax], xmm0       // 16 bytes, SSE2
// f3 0f 7f 00

#define MOV_XMM0_TO_EAX  __asm _emit 0xF3 \
                         __asm _emit 0x0F \
                         __asm _emit 0x7F \
                         __asm _emit 0x00


// vmovdqu ymm0, YMMWORD PTR [eax]      // 32 bytes, AVX
// c5 fe 6f 00

#define MOV_EAX_TO_YMM0   __asm _emit 0xC5 \
                          __asm _emit 0xFE \
                          __asm _emit 0x6F \
                          __asm _emit 0x00

// vmovdqu YMMWORD PTR [eax], ymm0      // 32 bytes, AVX
// c5 fe 7f 00

#define MOV_YMM0_TO_EAX  __asm _emit 0xC5 \
                         __asm _emit 0xFE \
                         __asm _emit 0x7F \
                         __asm _emit 0x00

// vmovdqu64 zmm0, ZMMWORD PTR [eax]    // 64 bytes, AVX-512     
// 62 f1 fe 48 6f 00

#define MOV_EAX_TO_ZMM0   __asm _emit 0x62 \
                          __asm _emit 0xF1 \
                          __asm _emit 0xFE \
                          __asm _emit 0x48 \
                          __asm _emit 0x6F \
                          __asm _emit 0x00

// vmovdqu64 ZMMWORD PTR [eax], zmm0    // 64 bytes, AVX-512
// 62 f1 fe 48 7f 00

#define MOV_ZMM0_TO_EAX   __asm _emit 0x62 \
                          __asm _emit 0xF1 \
                          __asm _emit 0xFE \
                          __asm _emit 0x48 \
                          __asm _emit 0x7F \
                          __asm _emit 0x00

#define XMMSAVE_ALIGNMENT 16

#define ALIGN(x,a)              __ALIGN_MASK(x,(a)-1)
#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))

#define XCRO_SSE      BIT1
#define XCRO_AVX      BIT2
#define XCRO_OPMASK   BIT5
#define XCRO_ZMM_LO   BIT6
#define XCRO_ZMM_HI   BIT7
#define CPUID_F1_ECX_XSAVE  BIT26

#define CPUID_F1_ECX_AVX    BIT28

#define CPUID_F1_EDX_MMX    BIT23 
#define CPUID_F1_EDX_SSE    BIT25
#define CPUID_F1_EDX_SSE2   BIT26

#define AVX512_SUPPORT_MASK (XCRO_OPMASK | XCRO_ZMM_LO | XCRO_ZMM_HI)

static UINT8   xmmsavearea[512+XMMSAVE_ALIGNMENT];
static UINT32  xmmsavearea_ptr;
static UINT8   avxUsedLast = 0;


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure: Vzeroupper
// 
//  Input:  None
// 
//  Output: None
// 
//  Description: Clears upper parts of YMM registers thus restoring AVX
//  state.
// 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static void Vzeroupper()
{
  __asm
  {
    VZEROUPPER
  }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure: x86_copy8b
// 
//  Input:  source and destination memory address pointers
// 
//  Output: None
// 
//  Description: Copies 8 bytes from source to destination via MM0
//  register. MMX instruction set.
// 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void x86_copy8b(UINT8 *pDest, UINT8 *pSrc)
{

  if (avxUsedLast)
    Vzeroupper();

  __asm
  {
    mov eax, pSrc 
    movq  mm0, QWORD PTR [eax] 
    mov eax, pDest
    movq QWORD PTR [eax], mm0
    emms
  }
  avxUsedLast = 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure: x86_copy16b
// 
//  Input:  source and destination memory address pointers
// 
//  Output: None
// 
//  Description: Copies 16 bytes from source to destination via XMM0
//  register. SSE2 instruction set.
// 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void x86_copy16b(UINT8 *pDest, UINT8 *pSrc)
{


  __asm
  {
    mov eax, pSrc
    MOV_EAX_TO_XMM0
    mov eax, pDest
    MOV_XMM0_TO_EAX
  }
  avxUsedLast = 0;

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure: x86_copy32b
// 
//  Input:  source and destination memory address pointers
// 
//  Output: None
// 
//  Description: Copies 32 bytes from source to destination via YMM0
//  register. AVX instruction set.
// 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void x86_copy32b (UINT8 *pDest, UINT8 *pSrc)
{

  __asm
  {
    mov eax, pSrc
    MOV_EAX_TO_YMM0
    mov eax, pDest
    MOV_YMM0_TO_EAX
  }
  avxUsedLast = 1;

}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure: x86_copy64b
// 
//  Input:  source and destination memory address pointers
// 
//  Output: None
// 
//  Description: Copies 64 bytes from source to destination via ZMM0
//  register. AVX-512 instruction set.
// 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void x86_copy64b(UINT8 *pDest, UINT8 *pSrc)
{

  __asm
  {
    mov eax, pSrc
    MOV_EAX_TO_ZMM0
    mov eax, pDest
    MOV_ZMM0_TO_EAX
  }
  avxUsedLast = 1;

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure: GetXCRregister
// 
//  Input:  Extended control register number
// 
//  Output: None
// 
//  Description: Reads value of extended control register
// 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
UINT32 GetXCRregister(UINT32 reg)
{
  UINT32 val = 0;
   __asm {
       mov     ecx, reg
       xgetbv
       mov     val, eax
    }
  return val;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure: SetXCRregister
// 
//  Input:  Extended control register number and vaue to be written into
//  it
// 
//  Output: None
// 
//  Description: Writes value into extended control register.
// 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void SetXCRregister(UINT32 reg, UINT32 val)
{
   __asm {
       mov     ecx, reg
       mov     eax, val
       xor     edx, edx
       xsetbv
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure: SetupCpuFeatures
// 
//  Input:  None
// 
//  Output: None
// 
//  Description: Detects CPU instruction sets support and enables respective
//  features in XCR0. 
// 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void SetupCpuFeatures()
{

  
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure: GetSgxFusingInfo
// 
//  Input:  None
// 
//  Output: None
// 
//  Description: Returns value of SGX fuse
// 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
UINT32 GetSgxFusingInfo ()
{  
  UINT32 val = 0;
  
  __asm {
    mov     eax, 7                      // Leaf 7
    mov     ecx, 0                      // Sub-leaf 2
    cpuid 
    mov val, ebx
  }             
  val >>=2;
  val &= 1;
  return val;
}
