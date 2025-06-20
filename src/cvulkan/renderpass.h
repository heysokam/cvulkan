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
#include "./types.h"


//______________________________________
// @section RenderPass
//____________________________

typedef struct cvk_renderpass_create_args {
  cvk_device_Logical const* const   device_logical;
  cvk_device_Swapchain const* const swapchain;
  cvk_Allocator* const              allocator;
} cvk_renderpass_create_args;

cvk_Pure cvk_Renderpass cvk_renderpass_create (  // clang-format off
  cvk_renderpass_create_args const* const arg
);  // clang-format on

void cvk_renderpass_destroy ( // clang-format off
  cvk_Renderpass* const           renderpass,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on

typedef struct cvk_renderpass_command_begin_args {
  cvk_command_Buffer const* const command_buffer;
  cvk_Framebuffer const* const    framebuffer;
  VkOffset2D const                offset;
  cvk_Size2D const                extent;
} cvk_renderpass_command_begin_args;
void cvk_renderpass_command_begin ( // clang-format off
  cvk_Renderpass const* const                    renderpass,
  cvk_renderpass_command_begin_args const* const arg
); // clang-format on

void cvk_renderpass_command_end ( // clang-format off
  cvk_Renderpass const* const     renderpass,
  cvk_command_Buffer const* const command_buffer
); // clang-format on


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_renderpass
#endif
#ifdef cvk_Implementation_renderpass
#include "./renderpass.c"
#endif


#endif  // H_cvulkan_renderpass

