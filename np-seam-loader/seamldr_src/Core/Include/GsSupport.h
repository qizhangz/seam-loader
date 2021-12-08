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

#ifndef GS_SUPPORT_H
#define GS_SUPPORT_H

#include <common.h>

//
// Default value used for the global /GS security cookie.
// Only (partly) useful in the case there was an overflow prior to the call to 
// __security_init_cookie()
//
#define DEFAULT_SECURITY_COOKIE 0xFF0A001B

//
// Cookie
//
extern UINT32 __security_cookie;

void __security_init_cookie(void);
void __fastcall  __security_check_cookie(UINT32 _StackCookie);

#endif