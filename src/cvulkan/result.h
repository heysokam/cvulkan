//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
//! @fileoverview Vulkan Result: Helpers
//_______________________________________|
#ifndef H_cvk_result
#define H_cvk_result
#include "./base.h"


cvk_String cvk_result_toString_default (VkResult status);
#ifndef cvk_result_toString
#define cvk_result_toString cvk_result_toString_default
#endif  // cvk_result_toString


#ifndef cvk_result_check
#include <stdio.h>  // clang-format off
#define cvk_result_check(status, message)                              \
  do {                                                                 \
    if (status) printf("%s (%s:%d) %s: " message "\n", \
      cvk_result_toString(status), __FILE__, __LINE__, __func__);      \
  } while (0)
#endif  // cvk_result_check
// clang-format on

#ifndef cvk_assert
#include <stdio.h>
#define cvk_assert(condition, message)                                                   \
  do {                                                                                   \
    if (!(condition)) printf("[%s:%d] %s: " message "\n", __FILE__, __LINE__, __func__); \
  } while (0)
#endif  // cvk_assert


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_result
#endif
#ifdef cvk_Implementation_result
#include "./result.c"
#endif


#endif  // H_cvk_result

