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
void
__ud2 (
  void
  );
#pragma intrinsic(__ud2)

/**
  Generates an undefined instruction.
**/
void
_ud2 (
  void
  )
{
  __ud2 ();
}
