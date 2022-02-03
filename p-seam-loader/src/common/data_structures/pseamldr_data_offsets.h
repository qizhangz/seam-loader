// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file pseamldr_data_offsets.h
 * @brief Offset of fields in PSEAMLDR data used in assembly code
 */
#ifndef __PSEAMLDR_DATA_OFFSETS_H_INCLUDED__
#define __PSEAMLDR_DATA_OFFSETS_H_INCLUDED__

#define PSEAMLDR_DATA_CANARY_OFFSET         40
#define PSEAMLDR_DATA_CANARY_SIZE           8
#define PSEAMLDR_DATA_VMM_GPRS_STATE_OFFSET (PSEAMLDR_DATA_CANARY_OFFSET + PSEAMLDR_DATA_CANARY_SIZE)


#endif // __PSEAMLDR_DATA_OFFSETS_H_INCLUDED__
