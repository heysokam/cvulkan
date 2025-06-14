//:________________________________________________________________
//  cvk  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:________________________________________________________________
//! @fileoverview Vulkan Result: Helpers
//_______________________________________|
#ifndef H_cvk_result
#define H_cvk_result


#ifndef cvk_result_check
#include <stdio.h>
#define cvk_result_check(status, message)                                                      \
  do {                                                                                         \
    if (status) printf("[%s:%d][%d] %s: " message "\n", __FILE__, __LINE__, status, __func__); \
  } while (0)
#endif  // cvk_result_check


#ifndef cvk_assert
#include <stdio.h>
#define cvk_assert(condition, message)                                                \
  do {                                                                                \
    if (!(condition)) printf("[%s:%d] %s: " message "\n", __FILE__, __LINE__, __func__); \
  } while (0)
#endif  // cvk_assert


#endif  // H_cvk_result
