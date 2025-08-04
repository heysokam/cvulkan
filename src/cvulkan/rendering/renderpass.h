//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Vulkan Renderpass: Types & Tools
//_________________________________________________|
#ifndef H_cvulkan_rendering_renderpass
#define H_cvulkan_rendering_renderpass
#include "../base.h"
#include "../types.h"


//______________________________________
// @section RenderPass
//____________________________

typedef struct cvk_renderpass_create_args {
  cvk_device_Logical const* const                 device_logical;
  cvk_Allocator* const                            allocator;
  cvk_Nullable cvk_size const                     attachment_cfg_len;  ///< Will treat `.ptr` as single-item when `.len` is omitted and `.ptr` is not null
  VkAttachmentDescription const* const            attachment_cfg_ptr;
  cvk_Nullable VkAttachmentReference const* const depthStencil;
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
#if defined cvk_Implementation || defined cvk_Implementation_rendering_static
#define cvk_Implementation_renderpass
#endif
#ifdef cvk_Implementation_renderpass
#include "./renderpass.c"
#endif


#endif  // H_cvulkan_rendering_renderpass

