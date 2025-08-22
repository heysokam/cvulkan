//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Vulkan Framebuffer: Types & Tools
//__________________________________________________|
#ifndef H_cvulkan_rendering_framebuffer
#define H_cvulkan_rendering_framebuffer
#include "../base.h"
#include "../types.h"


//______________________________________
// @section Framebuffer
//____________________________

/// @description
/// Configuration options for `cvk_framebuffer_create`.
typedef struct cvk_framebuffer_create_args {
  cvk_device_Logical const* const             device_logical;
  cvk_Renderpass const* const                 renderpass;
  cvk_Size2D const* const                     size;
  cvk_Nullable cvk_size const                 attachments_len;  ///< When omittied (aka. `0`), will treat `.attachments_ptr` as a pointer to one
  VkImageView const* const                    attachments_ptr;
  cvk_Nullable VkFramebufferCreateFlags const flags;
  char                                        priv_pad[4];
  cvk_Allocator* const                        allocator;
} cvk_framebuffer_create_args;

/// @description
/// Creates a `Framebuffer` object and its required configuration, using the given `arg` configuration options.
///
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_framebuffer_create` using the same `allocator`.
cvk_Pure cvk_Framebuffer cvk_framebuffer_create (  // clang-format off
  cvk_framebuffer_create_args const* const arg
);  // clang-format on

/// @description
/// Releases any memory and handles created by `cvk_framebuffer_create` using the same `allocator`.
void cvk_framebuffer_destroy ( // clang-format off
  cvk_Framebuffer* const          framebuffer,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on

/// @description
/// Releases any memory and handles stored on the given `framebuffers` list, using the same `allocator` that created them.
/// @note Thin wrapper around calling `cvk_framebuffer_destroy` on every item in the `framebuffers` list.
void cvk_framebuffer_list_destroy ( // clang-format off
  cvk_framebuffer_List* const     framebuffers,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on


//______________________________________
// @section Single Header Support
//____________________________
#if defined cvk_Implementation || defined cvk_Implementation_rendering_static
#define cvk_Implementation_framebuffer
#endif
#ifdef cvk_Implementation_framebuffer
#include "./framebuffer.c"
#endif


#endif  // H_cvulkan_rendering_framebuffer

