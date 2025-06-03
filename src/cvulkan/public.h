//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
//! @fileoverview Cable connector to the public modules of the library.
//_____________________________________________________________________|
#ifndef H_cvulkan_public
#define H_cvulkan_public

//______________________________________
// @section stdlib Dependencies, Aliases and Primitives
//____________________________
#include <stdint.h>
#include <stddef.h>

// clang-format off
#ifndef cvk_bool
#if __STDC_VERSION__ < 202311L
// clang-format on
#define cvk_bool int

#ifndef cvk_false
#define cvk_false (cvk_bool)(0)
#endif  // cvk_false

#ifndef cvk_true
#define cvk_true (cvk_bool)(1)
#endif  // cvk_true

#else   // __STDC_VERSION__ >= C23
#define cvk_bool  bool
#define cvk_true  true
#define cvk_false false
#endif  // __STDC_VERSION__

#endif  // cvk_bool


//______________________________________
// @section Result
//____________________________


//______________________________________
// @section Allocator
//____________________________


//______________________________________
// @section Instance
//____________________________


//______________________________________
// @section Surface
//____________________________


#endif  // H_cvulkan_public
