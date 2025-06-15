//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
//! @fileoverview Minimal Math tools used by cvulkan
//___________________________________________________|
#ifndef H_cvulkan_math
#define H_cvulkan_math
#include "./base.h"


//______________________________________
// @section u32 helpers
//____________________________
cvk_Pure uint32_t cvk_min_u32 (uint32_t const val, uint32_t const m);
cvk_Pure uint32_t cvk_max_u32 (uint32_t const val, uint32_t const M);
cvk_Pure uint32_t cvk_clamp_u32 (uint32_t const val, uint32_t const m, uint32_t const M);


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_math
#endif
#ifdef cvk_Implementation_math
#include "./math.c"
#endif


#endif  // H_cvulkan_math

