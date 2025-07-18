//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Vulkan Debug & Validation Layers: Types & Tools
//________________________________________________________________|
#ifndef H_cvulkan_validation
#define H_cvulkan_validation
#include "./base.h"
#include "./types.h"



/// @description
/// Validation Layer Name, missing from the spec.
/// Should be VK_KHR_VALIDATION_LAYER_NAME or similar
#define cvk_validation_LayerName "VK_LAYER_KHRONOS_validation"


//______________________________________
// @section Validation: Options
//____________________________

cvk_Pure cvk_validation_Options cvk_validation_options_defaults ();


//______________________________________
// @section Debug Messenger: Options
//____________________________

cvk_Pure VkDebugUtilsMessengerCreateInfoEXT cvk_validation_debug_options_defaults ();
cvk_Pure VkDebugUtilsMessengerCreateInfoEXT cvk_validation_debug_options_create (
  VkDebugUtilsMessengerCreateFlagsEXT const  flags,
  VkDebugUtilsMessageSeverityFlagsEXT const  severity,
  VkDebugUtilsMessageTypeFlagsEXT const      msgType,
  PFN_vkDebugUtilsMessengerCallbackEXT const callback,
  void* const                                userdata
);

VkBool32 cvk_validation_debug_callback (
  VkDebugUtilsMessageSeverityFlagBitsEXT const      severity,
  VkDebugUtilsMessageTypeFlagsEXT const             types,
  VkDebugUtilsMessengerCallbackDataEXT const* const cbdata,
  void* const                                       userdata
);


//______________________________________
// @section Debug Messenger: Create/Destroy
//____________________________

/// @description
/// Creates the debug messenger context with `vkCreateDebugUtilsMessengerEXT`
/// @note `vkCreateDebugUtilsMessengerEXT` is not provided by default,
/// This helper first requests the function, and then calls with the correct arguments
cvk_Pure VkDebugUtilsMessengerEXT cvk_validation_debug_context_create (  // clang-format off
  VkInstance const                          instance,
  VkDebugUtilsMessengerCreateInfoEXT* const cfg,
  VkAllocationCallbacks* const              allocator
);  // clang-format on


/// @description
/// Destroys the debug messenger context with `vkDestroyDebugUtilsMessengerEXT`
/// @note `vkDestroyDebugUtilsMessengerEXT` is not provided by default,
/// This helper first requests the function, and then calls with the correct arguments
void cvk_validation_debug_context_destroy (VkInstance const instance, VkDebugUtilsMessengerEXT debug_ct, VkAllocationCallbacks* const allocator);


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_validation
#endif
#ifdef cvk_Implementation_validation
#include "./validation.c"
#endif


#endif  // H_cvulkan_validation

