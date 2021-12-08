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

void __writemsr (unsigned long Register, unsigned __int64 Value);

#pragma intrinsic(__writemsr)

/**
  Write data to MSR.

  @param  Index                The register index of MSR.
  @param  Value                Data wants to be written.

  @return Value written to MSR.

**/
void
__writeMsr64 (
  IN UINT32  Index,
  IN UINT64  Value
  )
{
  __writemsr (Index, Value);
}
