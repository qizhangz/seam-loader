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

#include <common.h>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure:  GetMaxPhyAddr
//
//  Input:      None
//
//  Output:     MAXPHYADDR value.
//
//  Description: Uses CPUID function in order to retrieve MAXPHYADDR
//  value
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
UINT32 GetMaxPhyAddr ()
{
  UINT32 CpuInfo[4];                    // 0-eax, 1-ebx, 2-ecx, 3-edx

  //
  // Obtain MAXPHYADDR.
  //
  __cpuid(CpuInfo, 0x80000000);

  if (CpuInfo[0] >= 0x80000008)
    __cpuid(CpuInfo, 0x80000008);
  else
    CpuInfo[0] = 36;

  return (CpuInfo[0] & 0xFF);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure:  GetPhyAddrMask
//
//  Input:      None
//
//  Output:     MAXPHYADDR value.
//
//  Description: Converts MAXPHYADDR to mask value. i.e  for 36 bits
//  returns 0xFFFFFFFFF 
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
UINT64 GetPhyAddrMask ()
{
  return shiftLeft64 (1,  GetMaxPhyAddr ()) - 1;     // Sighting #3865410
}

