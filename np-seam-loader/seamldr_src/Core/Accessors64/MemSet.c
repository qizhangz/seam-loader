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

/*
 * Microsoft compiler can replacement code sections with internal memset so
 * we must provide a replacement. Just call MemFill, but return copy to address
 * per api
 *
 * We can't directly call MemFill, which is treating Val as a dword value
 * instead of a byte. So before calling, initialize a dword's bytes with lsb of
 * value passed to us.
 */
void * memset(void *Start, UINT32 Val, UINT32 Len)
{
  union {
    UINT8 bytes[4];
    UINT32 val;
  } tmp;
  
  UINT8 byte = (UINT8)(Val & 0xFF);
  
  tmp.bytes[0] = byte;
  tmp.bytes[1] = byte;
  tmp.bytes[2] = byte;
  tmp.bytes[3] = byte;

  MemFill(Start,Len,tmp.val);
  return(Start);
}
