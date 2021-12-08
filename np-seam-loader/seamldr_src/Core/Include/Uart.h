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

#ifndef UART_H
#define UART_H


#define COM1_BASE                   0x3f8
#define COM2_BASE                   0x2f8
#define COM3_BASE                   0x3e8
#define COM4_BASE                   0x2e8

#define MAX_SERIAL_TIMEOUT_LOOPS    2

#define COM_PORT                    COM1_BASE

#define RECEIVER_BUFFER             0x00
#define TRANSMIT_HOLDING            0x00
#define DIVISOR_LATCH_LOW           0x00    // 0/1 - 16-bit

#define DIVISOR_LATCH_HIGH          0x01
#define INTERRUPT_ENABLE            0x01
#   define DISABLE_INTERRUPTS       0x00

#define INTERRUPT_IDENTIFICATION    0x02
#define FIFO_CONTROL                0x02
#define     FIFO_DISABLE_MASK       0xfe

#define LINE_CONTROL                0x03
#   define DIVISOR_LATCH_ACCESS     0x80
#   define NO_STOP                  0x00
#   define NO_PARITY                0x00
#   define BITS8                    0x03

#define MODEM_CONTROL               0x04
#define LINE_STATUS                 0x05
#   define TRANS_HOLDING_REG_EMPTY  0x20
#   define DATA_READY               0x01

#define MODEM_STATUS                0x06
#define SCRATCH                     0x07


#endif UART_H