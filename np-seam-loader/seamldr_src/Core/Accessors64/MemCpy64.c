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
#include <Header.h>
#include <NpSeamldrApi.h>

/*
 * Microsoft compiler can replacement code sections with internal memcpy so
 * we must provide a replacement. Just call copyData, but return copy to address
 * per api
 */
void *memcpy(void *to, const void *from, UINT32 len)
{
  UINT8 *d = (UINT8 *)to;
  UINT8 *s = (UINT8 *)from;

  #if 1
  while (len / 8) {
    *(UINT64 *) d = * (UINT64 *) s;
    d += 8;
    s += 8;
    len -= 8;
  };
#else
  __movsq(to, from, len / 8);
  len %= 8;
#endif
  while (len--)
    *d++ = *s++;
  return to;
}


/*
 * Implement MSVC memcpy_s interface 
 */
UINT64 memcpy_s(
    void* dest,
    UINTN destSize,
    const void* src,
    UINTN count
) {
  UINT64 Status = NP_SEAMLDR_PARAMS_STATUS_SUCCESS;
  UINT8 *d = (UINT8 *)dest;
  UINT8 *s = (UINT8 *)src;

  UINT64 dst_addr = (UINT64)d;
  UINT64 src_addr = (UINT64)s;

  const UINT64 dst_first_address = dst_addr;
  const UINT64 dst_last_address = dst_addr + count - 1;
  const UINT64 src_first_address = src_addr;
  const UINT64 src_last_address = src_addr + count - 1;

  if ((destSize < count) ||
      (dst_addr > MAX_QWORD - count - 1) ||      
      (src_addr + count - 1 < src_addr))
  {
      Status = NP_SEAMLDR_PARAMS_STATUS_EBADPARAM;
      goto EXIT;
  }

  // check if we have an overlap between source and destination
  if (((dst_first_address > src_last_address) || (dst_last_address < src_first_address)) == FALSE)
  {
      Status = NP_SEAMLDR_PARAMS_STATUS_EBADPARAM;
      goto EXIT;
  }
  
#if 1
  while (count / 8) {
    *(UINT64 *) d = * (UINT64 *) s;
    d += 8;
    s += 8;
    count -= 8;
  };
#else
  __movsq(to, from, len / 8);
  len %= 8;
#endif
  while (count--)
    *d++ = *s++;
  
  EXIT:
  return Status;
}


