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

#ifndef TRACE_H
#define TRACE_H
#pragma pack (push, 1)

void    TraceWriteString    (char *, UINT32);

void    Trace               (char *, ...);
void    Printf              (const char *, ...);
void    PrintBuffer(UINT8 control, const unsigned char *buffer, UINT32 size, const char *Format, ...);
void    TraceFunction       (UINT8, char *, ...);

void    TraceProgress       (UINT32);
void    TraceValue          (UINT64, UINT32);
void    _Trace(char *Format, ...);

#define TRACE_ENABLED_FLG         BIT0
#define TRACE_SERIALPORT_ENABLED  BIT1
#define TRACE_USBPORT_ENABLED     BIT2

#define TRACE_ERRORHANDLER        BIT0
#define TRACE_MMIO                BIT1
#define TRACE_IO                  BIT2
#define TRACE_MSR                 BIT3
#define TRACE_TPM                 BIT4
#define TRACE_PCI                 BIT5
#define TRACE_GSTATE              BIT6

#define PRINT_HEX_VAL(Description, Value)     ComSerialOut(Description);\
    printHex(Value);\
    ComSerialOut("\n");

#ifndef __ASM__

#if (MKF_ENGINEERING==1)&&(MKF_TRACE==1)

typedef UINT8 (*TRACE_WRITE_STRING) (char *, UINT32 len);

typedef struct _traceInterface {
    UINT32 preTrace;
    UINT32 postTrace;
    UINT32 writeString;
    UINT32 flags;
    UINT32 maxTimeout;
    UINT32 offLine;
    char *func;

} TRACEINTERFACE;

extern TRACEINTERFACE traceRoutines;

  #define TRACE(s, ...) Trace(s, __VA_ARGS__)
  #define PRINTF(s, ...) Printf(s, __VA_ARGS__)
  #define TRACEIT() Printf("%s#%d\n", __FILE__, __LINE__);
  #define TRACEFUNCTION(c,s,...) TraceFunction(c,s,__VA_ARGS__)
  #define TRACE_RANGE(s) TraceRange(s)
  #define TRACE_RANGE_TABLE(s,t) TraceRangeTable(s,t)
  #define TRACE_IF(c) if(c)
  #define TRACE_PROGRESS(c) TraceProgress(c)
  #define SET_NO_TRACE do { \
(traceRoutines.flags &= ~TRACE_ENABLED_FLG); \
traceRoutines.offLine = __LINE__; \
traceRoutines.func = __FUNCTION__; \
  } while (0)
  #define SET_TRACE (traceRoutines.flags |= TRACE_ENABLED_FLG)
  #define IS_TRACE_ENABLED (traceRoutines.flags & TRACE_ENABLED_FLG)
  #define EXEC_NO_TRACE(c) do { \
    UINT8 enabled = IS_TRACE_ENABLED; \
    SET_NO_TRACE; \
    (c); \
    if (enabled) \
      SET_TRACE; \
  } while(0)
  #define TRACE_VALUE(c,w) TraceValue(c,w)
  #define PRINTBUFFER(CONTROL,PTR,SIZE,FMT, ...) PrintBuffer((CONTROL), (PTR), (SIZE), FMT, __VA_ARGS__)
  #define PRINTMULTIHASH(MH,FMT)                 MultiHashPrint(MH,FMT)

#if  TRACE_WITH_FUNCTIONS & TRACE_TPM
  #define TPM_TRANSMIT_DEBUG(R)     tpmTransmitDebug (R)
#else
  #define TPM_TRANSMIT_DEBUG(R)
#endif

#else
  #define TRACE(s, ...)
  #define PRINTF(s, ...)
  #define TRACEIT()
  #define TRACEFUNCTION(c,s,...)
  #define TRACE_RANGE(s)
  #define TRACE_RANGE_TABLE(s,t)
  #define TRACE_IF(c)
  #define TRACE_PROGRESS(c)
  #define SET_NO_TRACE
  #define SET_TRACE
  #define EXEC_NO_TRACE(c) (c)
  #define TRACE_VALUE(c,w)
  #define PRINTBUFFER(CONTROL,PTR,SIZE,FMT, ...)
  #define PRINTMULTIHASH(MH,FMT)
  #define TPM_TRANSMIT_DEBUG(R)
#endif
#endif

//
// Allow benchmarking trace without setting of MKF_TRACE flag in
// engineering mode.
//

// #if (MKF_ENGINEERING==1)
//   #define TRACE_BENCHMARK Trace("CLK = %llx\n", ClockCount)
// #else
#define TRACE_BENCHMARK
// #endif


typedef char *va_list;
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#define va_start(ap,v)  ( ap = (va_list)&v + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )

#define PROGRESS_MARK(code) ProgressMark((UINT8) code); TRACE_PROGRESS(code)
#define FN_PROGRESS_MARK(F,C)  PROGRESS_MARK(((F) << 4) + C)

UINT32  InitSioSmsc1007   ();

void    NumToStr          (UINT8 *, UINT32, UINT64);
void    TraceUnLock       ();
                  
#if (MKF_ENGINEERING==0) || (MKF_TRACE==0)
#define ComSerialOut(...)
#define printHex(...)
#endif

#pragma pack (pop)
#endif
