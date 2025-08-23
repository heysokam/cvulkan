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

/// @description
/// Configuration options for `cvk_renderpass_create`.
typedef struct cvk_renderpass_create_args {
  cvk_device_Logical const* const                 device_logical;
  cvk_Allocator* const                            allocator;
  cvk_Nullable cvk_size const                     attachment_cfg_len;  ///< Will treat `.ptr` as single-item when `.len` is omitted and `.ptr` is not null
  VkAttachmentDescription const* const            attachment_cfg_ptr;
  cvk_Nullable VkAttachmentReference const* const depthStencil;
} cvk_renderpass_create_args;

/// @description
/// Creates a `Renderpass` object and its required configuration, using the given `arg` configuration options.
///
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_renderpass_destroy` using the same `allocator`.
///
///................................
/// @important
/// cvulkan focuses primarily on Dynamic Rendering and v1.3+ features/extensions.
/// This function is currently very under-developed.
/// and only supports the setup created by [vulkan-tutorial.org].
/// The Renderpass+Framebuffer helpers are here for completion,
/// and need to be properly extended to support Tile-Based GPUs (low priority before cvulkan v1.0).
///................................
cvk_Pure cvk_Renderpass cvk_renderpass_create (  // clang-format off
  cvk_renderpass_create_args const* const arg
);  // clang-format on

/// @description
/// Releases any memory and handles created by `cvk_renderpass_create` using the same `allocator`.
void cvk_renderpass_destroy ( // clang-format off
  cvk_Renderpass* const           renderpass,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on

/// @description
/// Configuration options for `cvk_renderpass_command_begin`.
typedef struct cvk_renderpass_command_begin_args {
  cvk_command_Buffer const* const        command_buffer;
  cvk_Framebuffer const* const           framebuffer;
  VkOffset2D const                       offset;
  cvk_Size2D const                       extent;
  cvk_Nullable cvk_size const            clear_len;  ///< Default `1` when omitted or `.clear_ptr` is null
  cvk_Nullable VkClearValue const* const clear_ptr;  ///< Default `{0.222, 0.333, 0.444, 1}` when omitted
} cvk_renderpass_command_begin_args;

/// @description
/// Orders Vulkan to start recording draw commands using the given `renderpass`.
/// The caller is responsible for calling `cvk_renderpass_command_end` when the recording process is done.
/// @note Thin wrapper for `vkCmdBeginRenderPass` using the cvulkan API.
void cvk_renderpass_command_begin ( // clang-format off
  cvk_Renderpass const* const                    renderpass,
  cvk_renderpass_command_begin_args const* const arg
); // clang-format on

/// @description
/// Stops the command recording process started by `cvk_renderpass_command_begin` using the same `renderpass`.
/// @note Thin inline wrapper for `vkCmdEndRenderPass` using the cvulkan API.
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

