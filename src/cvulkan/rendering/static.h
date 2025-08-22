//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Vulkan Static Rendering Types & Tools
//!
//! @important
//!  cvulkan prioritizes Dynamic Rendering and vulkan 1.3+,
//!  which makes this section completely unnecessary for normal use.
//!  This API is here for completion (and supporting tile-based GPUs)
//!  In short:
//!  If you find something missing in this section that you need,
//!  please reach out and open an issue on GitHub
//____________________________________________________________________|
#ifndef H_cvulkan_rendering_static
#define H_cvulkan_rendering_static

//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_rendering_static
#endif

//______________________________________
// @section Cable Connector
//____________________________
#include "./renderpass.h"
#include "./framebuffer.h"


#endif  // H_cvulkan_rendering_static

