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

#include <GsSupport.h>
#include <common.h>

// Init cookies with some default values, in case there is an overflow before 
// it is initialized
UINT32 __security_cookie = DEFAULT_SECURITY_COOKIE;

__declspec(noreturn)
void
__security_deadloop(void)
{
DeadLoop:
  _ud2();
  goto  DeadLoop;
}

UINT32
__security_cookie_rdrand(void)
{
    UINT8 num_retries = 10; // number of max retries
    UINT32 status = 0, value = 0;
    while(num_retries != 0){
        if (RdSeed32(&value)){
            status = 1;
            break;
        }
        num_retries--;
        _mm_pause();
    }
    if (status == 0)
        __security_deadloop();
    return value;
}

void 
__security_init_cookie(void)
{
    UINT32 Cookie;
    Cookie = __security_cookie_rdrand();
    if (Cookie == 0){       
      __security_deadloop();
    }
  __security_cookie = Cookie;
}

void 
__report_gsfailure(void)
{
  __security_deadloop();
}

void 
__fastcall  __security_check_cookie(UINT32 _StackCookie)
{
  if (_StackCookie == __security_cookie) {
    return;
  }
  __report_gsfailure();
}

#if (MKF_ENGINEERING==1) &&(MKF_TRACE==1)
__declspec(noreturn)
void
__cdecl __report_rangecheckfailure(void)
{
  __report_gsfailure();
}
#endif