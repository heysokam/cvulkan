//:________________________________________________________________
//  cvk  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:________________________________________________________________
//! @fileoverview Vulkan Application Info: Types & Tools
//_______________________________________________________|
#ifndef H_cvulkan_application
#define H_cvulkan_application
#include "./base.h"

#define cvk_Application VkApplicationInfo
cvk_Pure cvk_Application cvk_Application_defaults ();


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_application
#endif
#ifdef cvk_Implementation_application
#include "./application.c"
#endif


#endif  // H_cvulkan_application

