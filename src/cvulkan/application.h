//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
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

