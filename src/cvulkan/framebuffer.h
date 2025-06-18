//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
//! @fileoverview Vulkan Framebuffer: Types & Tools
//!
//! @important
//!  cvulkan prioritizes Dynamic Rendering and vulkan 1.3+,
//!  which makes this section completely unnecessary for normal use.
//!  This API is here for completion (and supporting tile-based GPUs)
//!  In short:
//!  Please reach out and open an issue on GitHub
//!  if there is something missing in this section that you need.
//____________________________________________________________________|
#ifndef H_cvulkan_framebuffer
#define H_cvulkan_framebuffer
#include "./base.h"
#include "./device.h"
#include "./renderpass.h"


//______________________________________
// @section Framebuffer: Context
//____________________________

typedef struct cvk_Framebuffer {
  VkFramebuffer           ct;
  VkFramebufferCreateInfo cfg;
} cvk_Framebuffer;

typedef struct cvk_framebuffer_List {
  cvk_size              len;
  cvk_Readonly cvk_size itemsize;
  cvk_Framebuffer*      ptr;
} cvk_framebuffer_List;

void cvk_framebuffer_list_destroy ( // clang-format off
  cvk_framebuffer_List* const framebuffers,
  cvk_device_Logical const* const              device_logical,
  cvk_Allocator* const                         allocator
); // clang-format on

typedef struct cvk_framebuffer_create_args {
  cvk_device_Logical const* const             device_logical;
  cvk_Renderpass const* const                 renderpass;
  cvk_Size2D const* const                     size;
  cvk_size const                              attachments_len;
  VkImageView const* const                    attachments_ptr;
  cvk_Nullable VkFramebufferCreateFlags const flags;
  cvk_Allocator* const                        allocator;
} cvk_framebuffer_create_args;

cvk_Pure cvk_Framebuffer cvk_framebuffer_create (  // clang-format off
  cvk_framebuffer_create_args const* const arg
);  // clang-format on

void cvk_framebuffer_destroy ( // clang-format off
  cvk_Framebuffer* const          framebuffer,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on


//______________________________________
// @section Framebuffer: Swapchain Framebuffer List
//____________________________

typedef struct cvk_device_swapchain_framebuffers_create_args {
  cvk_device_Swapchain const* const swapchain;
  cvk_device_Logical const* const   device_logical;
  cvk_Renderpass const* const       renderpass;
  cvk_Allocator* const              allocator;
} cvk_device_swapchain_framebuffers_create_args;

cvk_Pure cvk_framebuffer_List cvk_device_swapchain_framebuffers_create (  // clang-format off
  cvk_device_swapchain_framebuffers_create_args const* const arg
);  // clang-format on


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_framebuffer
#endif
#ifdef cvk_Implementation_framebuffer
#include "./framebuffer.c"
#endif


#endif  // H_cvulkan_framebuffer

