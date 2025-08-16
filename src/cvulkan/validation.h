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

/// @description
/// Returns a valid `cvk_validation_Options` object already configured to use all of the `cvulkan` defaults.
/// The default values are taken from the (overridable) `cvk_config_validation_*` and `cvk_config_debug_*` comptime defined values.
cvk_Pure cvk_validation_Options cvk_validation_options_defaults ();


//______________________________________
// @section Debug Messenger: Options
//____________________________

/// @description
/// Returns a valid `VkDebugUtilsMessengerCreateInfoEXT` object already configured to use all of the `cvulkan` defaults.
/// The default values are taken from the (overridable) `cvk_config_debug_*` comptime defined values.
cvk_Pure VkDebugUtilsMessengerCreateInfoEXT cvk_validation_debug_options_defaults ();

/// @description
/// Returns a valid `VkDebugUtilsMessengerCreateInfoEXT` configured to use the given arguments.
/// Shorthand to avoid passing the correct `sType` enum value on creation.
cvk_Pure VkDebugUtilsMessengerCreateInfoEXT cvk_validation_debug_options_create (
  VkDebugUtilsMessengerCreateFlagsEXT const  flags,
  VkDebugUtilsMessageSeverityFlagsEXT const  severity,
  VkDebugUtilsMessageTypeFlagsEXT const      msgType,
  PFN_vkDebugUtilsMessengerCallbackEXT const callback,
  void* const                                userdata
);

/// @description
/// Function used by cvulkan as the default DebugMessenger callback.
/// Overridable at comptime by the `cvk_config_debug_callback` define.
VKAPI_ATTR VkBool32 VKAPI_CALL cvk_validation_debug_callback (
  VkDebugUtilsMessageSeverityFlagBitsEXT const      severity,
  VkDebugUtilsMessageTypeFlagsEXT const             types,
  VkDebugUtilsMessengerCallbackDataEXT const* const cbdata,
  void* const                                       userdata
);


//______________________________________
// @section Debug Messenger: Create/Destroy
//____________________________

/// @description
/// Creates the debug messenger context using `vkCreateDebugUtilsMessengerEXT`
/// @note This helper first requests the `vkCreateDebugUtilsMessengerEXT` function handle, and then calls it with the correct arguments
cvk_Pure VkDebugUtilsMessengerEXT cvk_validation_debug_context_create (  // clang-format off
  VkInstance const                          instance,
  VkDebugUtilsMessengerCreateInfoEXT* const cfg,
  VkAllocationCallbacks* const              allocator
);  // clang-format on


/// @description
/// Destroys the debug messenger context using `vkDestroyDebugUtilsMessengerEXT`
/// @note This helper first requests the `vkDestroyDebugUtilsMessengerEXT` function handle, and then calls it with the correct arguments
void cvk_validation_debug_context_destroy ( // clang-format off
  VkInstance const             instance,
  VkDebugUtilsMessengerEXT     debug_ct,
  VkAllocationCallbacks* const allocator
); // clang-format on


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

