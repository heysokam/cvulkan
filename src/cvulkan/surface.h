//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
//! @fileoverview Vulkan Surface: Types & Tools
//______________________________________________|
#ifndef H_cvulkan_surface
#define H_cvulkan_surface
#include "./base.h"

/// @important
/// Surface requesting features would force this library to depend on a specific windowing library.
/// As such, cvulkan expects you to send the VkSurfaceKHR handle as input instead.
/// How you request that handle is up to you. cvulkan doesn't need to know about it.
///
/// @description
/// Alias to unify the naming convention of cvulkan types
typedef VkSurfaceKHR cvk_Surface;

#endif  // H_cvulkan_surface
