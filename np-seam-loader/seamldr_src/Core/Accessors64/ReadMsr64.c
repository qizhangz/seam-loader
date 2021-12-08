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

/**
  Microsoft Visual Studio 7.1 Function Prototypes for I/O Intrinsics.
**/

#include "common.h"

unsigned __int64 __readmsr (int register);

#pragma intrinsic(__readmsr)

/**
  Read data to MSR.

  @param  Index                Register index of MSR.

  @return Value read from MSR.

**/
UINT64
__readMsr64 (
  IN UINT32  Index
  )
{
  return __readmsr (Index);
}