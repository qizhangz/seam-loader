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
#include <pch.h>
#include <uart.h>

#define MAX_SERIAL_TIMEOUT_LOOPS    2

#if (MKF_ENGINEERING==1)&&((MKF_TRACE==1)||(MKF_BENCHMARKING==1))

UINT8 nib[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

extern void ComSerialOut(char *);
void PchUartSerialOut(char *str, UINT32 len);

TRACEINTERFACE traceRoutines = {
    0,
    0,
	(UINT32)&HOOK(Serialout),
    TRACE_ENABLED_FLG,
    MAX_SERIAL_TIMEOUT_LOOPS,
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure:  NumToStr8
//
//  Input:	n BYTE value to convert to string
//
//  Output:	p - pointer to converted string
//
//  Description:	Byte value is converted to ASCIZ string
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void NumToStr8 (UINT8 *p, UINT8 n )
{
  UINT32 i;

  for (i = 0; i < 2; i++) {
    __asm {
      rol n, 4;
    };
    p[i] = nib[n & 0xF];
  }
  p[i] = 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure:  NumToStr
//
//  Input:	w - width in bytes = 1;2;4;8
//          n - number to convert
//
//  Output:	p resultant converted string.
//
//  Description:	BYTE, WORD, DWORD, QWORD number is converted to
//  ASCIZ string.
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void NumToStr (UINT8 *p, UINT32 w, UINT64 n)
{
  UINT32 i;

  for (i = 0; i < w; i++) {
    NumToStr8 (p + i * 2, *(((UINT8 *) &n) + w - i - 1));
  }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure:   TraceWriteString
// 
//  Input:  str - string pointer, len - string length
// 
//  Output: None
// 
//  Description: Prints string via serial i/f
// 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
TraceWriteString(char *str, UINT32 len)
{
  TRACE_WRITE_STRING pFunc = (TRACE_WRITE_STRING)traceRoutines.writeString;
  if (pFunc)
    (*pFunc)(str,len);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure:  __Trace
//
//  Input:      Format string and variable parameters to trace.
//
//  Output:	None
//
//  Description: Supported formats:
//  %c  - unsigned char
//  %hx - unsigned short int = byte
//  %x  - unsigned int = word
//  %lx - unsigned long int = dword
//  %llx - unsigned long long int = qword
//  %s  - string of chars
//  %%  - % char
//  %d1x - SHA1 digest
//  %d2x - SHA256 digest
//
//  All values are in hex - decimals are not supported.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
__Trace (char *Fmt, va_list argptr)
{
  UINT64  arg;
  UINT32  i;
  UINT8   val[97];                      // digest256 + 0 terminator
  char    *pThis;
  char    *pNext;
  char    tmp[256];

  if (!IS_TRACE_ENABLED) {
    return;
  }

  pThis = Fmt;

  for (;;) {

    for (i = 0; pThis[i] != 0; i++) {
      if (pThis[i] == '%')
        break;
    }
    //
    // No format - just out the string.
    //
    if (pThis[i] == 0) {
      TraceWriteString(pThis,0);
      return;
    }

    //
    // Found format specifier. Copy and out string so far
    // If format string is too long, truncate and exit.
    //     
    if (i > sizeof (tmp) - sizeof ("%llx")) {
      pThis[i] = 0;
      TraceWriteString(pThis,0);
      return;
    }
    // 
    // Copy and out string so far
    // 
    //copyData(tmp, pThis, i); // causes corruption in serial log 
	memcpy(tmp, pThis, i); 
    tmp[i]=0;
    TraceWriteString(tmp,0);

    pNext = pThis + i + 1;
    //
    // Analyze format token
    //
    if (pNext[0] == '%') {
      //
      // %% format
      //
      pThis = pNext + 1;
      TraceWriteString("%",0);

    } else if ((pNext[0] == 'c')) {
      //
      // %c format
      //
      arg = va_arg(argptr, UINT8);
      pThis = pNext + 1;
      val[0]=(char) arg;
      val[1]=0;
      TraceWriteString(val,0);

    } else if ((pNext[0] == 'h') && (pNext[1] == 'x')) {
      //
      // %hx format
      //
      arg = va_arg(argptr, UINT8);
      pThis = pNext + 2;
      NumToStr(val, 1, arg);
      TraceWriteString(val,0);

    } else if ((pNext[0] == 'x')) {
      //
      // %x format
      //
      arg = va_arg(argptr, UINT16);
      pThis = pNext + 1;
      NumToStr(val, 2, arg);
      TraceWriteString(val,0);

    } else if ((pNext[0] == 'l') && (pNext[1] == 'x')) {
      //
      // %lx format
      //
      arg = va_arg(argptr, UINT32);
      pThis = pNext + 2;
      NumToStr(val, 4, arg);
      TraceWriteString(val,0);

    } else if ((pNext[0] == 'l') && (pNext[1] == 'l') && (pNext[2] == 'x')) {
      //
      // %llx format
      //
      arg = va_arg(argptr, UINT64);
      pThis = pNext + 3;
      NumToStr(val, 8, arg);
      TraceWriteString(val,0);

    } else if ((pNext[0] == 'd') && (pNext[1] == '1') && (pNext[2] == 'x')) {
      //
      // %d1x format
      //
      arg = va_arg(argptr, UINT32);
      pThis = pNext + 3;
      for (i = 0; i < 20; i++) {
        NumToStr8 (val + i * 3, *((UINT8 *) arg + i));
        *(val + i*3 -1) = 0x20;
      }
      *(val+61) = 0;
      TraceWriteString(val,0);

    } else if ((pNext[0] == 'd') && (pNext[1] == '2') && (pNext[2] == 'x')) {
      //
      // %d1x format
      //
      arg = va_arg(argptr, UINT32);
      pThis = pNext + 3;
      for (i = 0; i < 32; i++) {
        NumToStr8 (val + i * 3, *((UINT8 *) arg + i));
        *(val + i*3 -1) = 0x20;
      }
      //
      // Enter LF after 20th char to start from new line.
      //
      *(val+59) = 0xA;
      // Sighting # 3864549 Start
      *(val+96) = 0;
      // Sighting # 3864549 End
      TraceWriteString(val,0);

    } else if ((pNext[0] == 's')) {
      //
      // %s format
      //
      arg = va_arg(argptr, UINT32);
      pThis = pNext + 1;
      TraceWriteString(val,0);

    } else {
      //
      // Unrecognized
      //
      return;
    }
  }
}

//#if (MKF_TRACE == 1)

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure:
// 
//  Input:  None
// 
//  Output: None
// 
//  Description: 
// 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Same as Trace, without pre/post trace support
void
_Trace( char *Format, ...)
{
  va_list  Marker;
  va_start (Marker, Format);
  __Trace(Format, Marker);
  va_end(Marker);
}


//#if MKF_TRACE == 1 

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure:
// 
//  Input:  None
// 
//  Output: None
// 
//  Description: 
// 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void
Trace( char *Format, ...)
{
  va_list Marker;
  PTR_FUNC_NO_ARG pFunc = (PTR_FUNC_NO_ARG)traceRoutines.preTrace;
  // If ACM has a pre-trace routine, call it.
  if (pFunc) {
    (*pFunc)();
  }
  va_start (Marker, Format);
  __Trace(Format, Marker);
  // If ACM has a post-trace routine, call it
  pFunc = (PTR_FUNC_NO_ARG)traceRoutines.postTrace;
  if (pFunc) {
    (*pFunc)();
  }
  va_end(Marker);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure:    TraceProgress
//
//  Input:	code - progress code to send to UART.
//
//  Output:	None
//
//  Description:  Sends progress code to UART.
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TraceProgress(UINT32 code)
{
  Trace("ACM PROGRESS MARK = %hx\n", code);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure:    TraceValue
//
//  Input:	code - progress code to send to UART.
//
//  Output:	None
//
//  Description:  Sends progress code to UART.
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TraceValue(UINT64 code, UINT32 width)
{
  switch (width) {
    case 8:
      Trace("CODE = %hx\n", code);
      break;
    case 16:
      Trace("CODE = %x\n", code);
      break;
    case 32:
      Trace("CODE = %lx\n", code);
      break;
    case 64:
      Trace("CODE = %llx\n", code);
      break;
  }
}

void PchUartSerialOut(char *str, UINT32 len)
{
	while (*str){
		while (0 == (ReadMmioByte(UART2_BAR + 4 * LINE_STATUS) & TRANS_HOLDING_REG_EMPTY));
		if ((*str) == 0xa){
			WriteMmioByte(UART2_BAR + 4 * TRANSMIT_HOLDING, 0xd);
		}
		WriteMmioByte(UART2_BAR + 4 * TRANSMIT_HOLDING, *str++);
	}
  
}

#endif
