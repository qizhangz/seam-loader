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
#include <../Server/Include/common64.h>
#include <Header.h>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure:  Init64bitComArea
//
//  Input:      None
//
//  Output:     None
//
//  Description: All fields of Init64bitComArea communication area with
//  64-bit code are initialized.
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Init64bitComArea()
{
    //
    // Initialize 32-64 bit communication data
    //

    SeamldrCom64Data.OriginalIDTRLimit = OriginalIDTRLimit; // Take only the limit from the IDTR
    TRACE("OriginalIDTR Limit: 0x%x\n", SeamldrCom64Data.OriginalIDTRLimit);

    MemFill(SeamldrCom64Data.OriginalGdtr, sizeof(SeamldrCom64Data.OriginalGdtr), 0);
    *(UINT16 *)SeamldrCom64Data.OriginalGdtr = (UINT16)(OriginalECX >> 16);
}