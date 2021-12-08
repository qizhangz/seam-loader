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

#ifndef _BENCHMARK_H
#define _BENCHMARK_H

#pragma pack (push, 1)
                
#if MKF_BENCHMARKING
#define INIT_BENCHMARK init_benchmark()
#define SAVE_BENCHMARK(tn, tag) save_benchmark(tn, tag)
#define FLUSH_BENCHMARK flush_benchmark ()
#define ADD_SINIT_HEAP_ELEMENT(p) build_sinit_heap_benchmark_element(p)
#else
#define INIT_BENCHMARK
#define SAVE_BENCHMARK
#define FLUSH_BENCHMARK
#define ADD_SINIT_HEAP_ELEMENT
#endif        
                                              
// 
// MTRR8 base for Startup ACM benchmarking log
//                           
#if !defined (MTRR8_BASE)
#define MTRR8_BASE 0xFF000000 
#endif

//
// Maximum number of performance records
//
#if MCP_CHIPSET_ACM_TYPE==AC_SINIT
#define PERF_MAX_RECORDS  (0x60)  
#else
#define PERF_MAX_RECORDS  (0x20)
#endif


typedef struct {
  UINT16      TaskNumber;
  UINT16      Tag;
  UINT64      TimeStamp;
} PERF_RECORD;

typedef struct {
  UINT32          Type;
  UINT32          Size;                 // Size of element in bytes
  UINT32          Uuid[4];
  UINT32          ModuleID;
  UINT32          Count;
  PERF_RECORD     Record[PERF_MAX_RECORDS];
} PERF_ELEMENT;

//
// Performance data UUID
//
// {06A9C77B-850B-476B-B3D0-C9B47BCB8A85}
#define PERF_DATA_UUID1 0x06A9C77B
#define PERF_DATA_UUID2 0x850B476B
#define PERF_DATA_UUID3 0xB3D0C9B4
#define PERF_DATA_UUID4 0x7BCB8A85

//
// Common benchmark tags across all types of ACM's( i.e. S-ACM, SINIT etc)
// Range : 0x1000 - 0x1FFF
//
#define TAG_ACM_BEGIN                    0x1000     // at ACM  launch
#define TAG_ACM_EXIT                     0x1001     // before ACM transfers control back

#define TAG_PCR_EXTEND_START             0x1002     // before extending PCR
#define TAG_PCR_EXTEND_END               0x1003     // PCR extend done


//
// SINIT specific benchmark tags
// Range : 0x2000 - 0x2FFF
//
#define SINIT_TAG_MLE_EVALUATION_START    0x2000     // MLE processing start
#define SINIT_TAG_MLE_EVALUATION_END      0x2001     // MLE processing done

#define SINIT_TAG_PROCESS_LCP_START       0x2002     // LCP processing start
#define SINIT_TAG_PROCESS_LCP_END         0x2003     // LCP processing done

#define SINIT_TAG_PROCESS_PCR_START       0x2004     // PCR processing start
#define SINIT_TAG_PROCESS_PCR_END         0x2005     // PCR processing done


//
// S-ACM specific benchmark tags
// Range : 0x3000 - 0x3FFF
//

#pragma pack (pop)

#endif