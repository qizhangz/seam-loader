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

#include "common.h"

#if (MKF_ENGINEERING==1) &&(MKF_TRACE==1)
void ComSerialOut(unsigned char *str) {
    //static uint8_t initialized=0;
    unsigned short     comPort = 0x3f8;

    while (*str) {
        while (0 == (_inp(comPort + 5) & 0x20));
        if ((*str) == 0xa) {
            _outp(comPort, 0xd);
            while (0 == (_inp(comPort + 5) & 0x20));
        }
        _outp(comPort, *str++);
    }
}

const static char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7',
'8', '9' ,'A', 'B', 'C', 'D', 'E', 'F' };

void printHex(UINT64 num)
{
    int i = 0, k = 0;
    static char buffer[17];    
    for (i = 0; i<16; i++) {
        buffer[15 - i] = hex[num & 0xF];
        num = num >> 4;
    }
    buffer[16] = 0;

    ComSerialOut(buffer);
}
#endif