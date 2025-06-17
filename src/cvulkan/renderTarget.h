//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
//! @fileoverview Vulkan RenderTarget: Types & Tools
//!
//! @note
//!  RenderTarget is a very thin wrapper/abstraction over:
//!  RenderPass + FrameBuffers + Subpasses
//!  cvk.RenderTarget -> vk.Renderpass
//!  cvk.RenderPass   -> vk.Subpass
//!  cvk.Framebuffer  -> vk.Framebuffer
//!
//! @important
//!  cvulkan prioritizes Dynamic Rendering and vulkan 1.3+,
//!  which makes this section completely unnecessary for normal use.
//!  This API is here for completion (and supporting tile-based GPUs)
//!  In short:
//!  Please reach out and open an issue on GitHub
//!  if there is something missing in this section that you need.
//____________________________________________________________________|
#ifndef H_cvulkan_renderTarget
#define H_cvulkan_renderTarget
#include "./base.h"


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_renderTarget
#endif
#ifdef cvk_Implementation_renderTarget
#include "./renderTarget/renderpass.c"
#include "./renderTarget/framebuffer.c"
#endif


#endif  // H_cvulkan_renderTarget

