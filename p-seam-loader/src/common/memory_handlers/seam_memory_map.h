// Intel Proprietary
// 
// Copyright 2021 Intel Corporation All Rights Reserved.
// 
// Your use of this software is governed by the TDX Source Code LIMITED USE LICENSE.
// 
// The Materials are provided “as is,” without any express or implied warranty of any kind including warranties
// of merchantability, non-infringement, title, or fitness for a particular purpose.
/**
 * @file seamldr_memory_map.h
 * @brief
 */

#ifndef SRC_COMMON_MEMORY_HANDLERS_SEAM_MEMORY_MAP_H_
#define SRC_COMMON_MEMORY_HANDLERS_SEAM_MEMORY_MAP_H_

#include "data_structures/pseamldr_data_types.h"

/**
 * @brief Maps the SEAM module memory in the SEAM range page table
 *
 * @return Returns error if mapping failed due to insufficient memory
 */
api_error_type seam_module_memory_map(pseamldr_data_t* pseamldr_data, memory_constants_t* mem_consts);

#endif /* SRC_COMMON_MEMORY_HANDLERS_SEAM_MEMORY_MAP_H_ */
