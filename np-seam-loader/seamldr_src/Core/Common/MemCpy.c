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
 * Microsoft compiler can replacement code sections with internal memcpy so
 * we must provide a replacement. Just call copyData, but return copy to address
 * per api
 */
void *memcpy(void *to, const void *from, UINT32 len)
{
  char *d = to;
  char *s = (char *)from;

  while (len / 4) {
    *(UINT32 *) d = * (UINT32 *) s;
    d += 4;
    s += 4;
    len -= 4;
  };
  while (len--)
    *d++ = *s++;
  return to;
}
