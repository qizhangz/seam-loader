// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file serial_port.c
 * @brief Serial port configurations and usage
 */

#include "tdx_debug.h"
#include "accessors/ia32_accessors.h"

#ifdef DEBUGFEATURE_TDX_DBG_TRACE

void tdx_outport80(uint16_t id)
{
    ia32_out16(0x80, id);
}

#define TIMEOUT_IN_TSC              200000000
#define DEBUG_SERIAL_PORT_NUMBER    0x3F8

void init_com_port(debug_control_t* debug_control)
{
    tdx_outport80(0xFD00);

    uint16_t com_port = DEBUG_SERIAL_PORT_NUMBER;

    debug_control->com_port_addr = com_port;

    // UART info taken from http://www.beyondlogic.org/serial/serial.htm

    // disabling access to Divisor Latch register
    ia32_out8((uint16_t)(com_port + 3), (uint8_t)(ia32_in8((uint16_t)(com_port + 3)) & 0x7f));
    // IER - disable interupts
    ia32_out8((uint16_t)(com_port + 1), (uint8_t)(ia32_in8((uint16_t)(com_port + 1)) & 0xf0));
    // Disable FIFO
    ia32_out8((uint16_t)(com_port + 2), 0x00);
    // config MCR (turn off:  loop back, aux output)
    ia32_out8((uint16_t)(com_port + 4), (uint8_t)(ia32_in8((uint16_t)(com_port + 4)) & 0xe3));
    // write 0 in the scratch register
    ia32_out8((uint16_t)(com_port + 7), 0x00);
    // enabling access to Divisor Latch
    ia32_out8((uint16_t)(com_port + 3), (uint8_t)(ia32_in8((uint16_t)(com_port + 3)) | 0x80));
    // setting Divisor Latch low byte to 1 - set to 115,200 baud
    ia32_out8((uint16_t)(com_port + 0), 0x01);
    // setting Divisor Latch high byte to 0
    ia32_out8((uint16_t)(com_port + 1), 0x00);
    // setting 8 bits word
    ia32_out8((uint16_t)(com_port + 3), 0x03);
    // config MCR (turn off: loop back, Force request to Send, Force Data Terminal Ready)
    ia32_out8((uint16_t)(com_port + 4), (uint8_t)(ia32_in8((uint16_t)(com_port + 4)) & 0xec));
    // Clear and enable FIFO
    ia32_out8((uint16_t)(com_port + 2), 0x07);

    tdx_outport80(0xFD02);
}

void log_to_com_port(const char* buff, uint32_t length, uint16_t print_port)
{
    uint32_t i = 0;
    uint8_t lsr = 0;

    for (i = 0; buff[i] != 0 && i < length; ++i)
    {
        uint64_t tsc_endtime = ia32_set_timeout( TIMEOUT_IN_TSC );
        // waiting for Empty Transmitter Holding Register bit to be set in the LSR
        for (lsr = 0x00; (lsr & 0x20) == 0x00; lsr = ia32_in8((uint16_t)(print_port + 5)))
        {
            ia32_pause();
            if (ia32_is_timeout_expired(tsc_endtime))
            {
                break;
            }
        }
        ia32_out8(print_port, (uint8_t)buff[i]);
    }
}

#endif
