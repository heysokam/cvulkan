//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
//! @fileoverview Vulkan Renderpass: Types & Tools
//!
//! @important
//!  cvulkan prioritizes Dynamic Rendering and vulkan 1.3+,
//!  which makes this section completely unnecessary for normal use.
//!  This API is here for completion (and supporting tile-based GPUs)
//!  In short:
//!  Please reach out and open an issue on GitHub
//!  if there is something missing in this section that you need.
//____________________________________________________________________|
#ifndef H_cvulkan_renderpass
#define H_cvulkan_renderpass
#include "./base.h"


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_renderpass
#endif
#ifdef cvk_Implementation_renderpass
#include "./renderpass/pass.c"
#include "./renderpass/subpass.c"
#include "./renderpass/framebuffer.c"
#endif


#endif  // H_cvulkan_renderpass

