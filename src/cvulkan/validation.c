//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
#include "./config.h"
#include "./result.h"
#include <vulkan/vulkan_core.h>
#include "./validation.h"


cvk_Pure cvk_validation_Options cvk_validation_options_defaults () {
  return (cvk_validation_Options){
    .layers_active  = cvk_config_validation_active,
    .debug_active   = cvk_config_debug_active,
    .debug_severity = cvk_config_debug_severity,
    .debug_msgType  = cvk_config_debug_msgType,
    .debug_callback = cvk_config_debug_callback,
    .debug_userdata = cvk_config_debug_userdata,
  };
}


cvk_Pure VkDebugUtilsMessengerCreateInfoEXT cvk_validation_debug_options_create (
  VkDebugUtilsMessengerCreateFlagsEXT const  flags,
  VkDebugUtilsMessageSeverityFlagsEXT const  severity,
  VkDebugUtilsMessageTypeFlagsEXT const      msgType,
  PFN_vkDebugUtilsMessengerCallbackEXT const callback,
  void* const                                userdata
) {
  return (VkDebugUtilsMessengerCreateInfoEXT){
    .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    .pNext           = NULL,
    .flags           = flags,
    .messageSeverity = severity,
    .messageType     = msgType,
    .pfnUserCallback = callback,
    .pUserData       = userdata,
  };
}


VkBool32 cvk_validation_debug_callback (
  VkDebugUtilsMessageSeverityFlagBitsEXT const      severity,
  VkDebugUtilsMessageTypeFlagsEXT const             types,
  VkDebugUtilsMessengerCallbackDataEXT const* const cbdata,
  void* const                                       userdata
) {
  cvk_discard(userdata);
  cvk_print("[cvulkan.Validation] (%u %u) : %s\n", types, severity, cbdata->pMessage);
  return VK_FALSE;
}


cvk_Pure VkDebugUtilsMessengerCreateInfoEXT cvk_validation_debug_options_defaults () {
  return cvk_validation_debug_options_create(
    /* flags    */ cvk_config_debug_flags,
    /* severity */ cvk_config_debug_severity,
    /* msgType  */ cvk_config_debug_msgType,
    /* callback */ cvk_config_debug_callback,
    /* userdata */ cvk_config_debug_userdata
  );
}


/// @internal
/// @description
/// Returns the Vulkan Debug Messenger create function
static PFN_vkCreateDebugUtilsMessengerEXT cvk_Fn_get_createDebug (
  VkInstance const instance
) {  // clang-format off
  #pragma clang diagnostic push
  // Allow soft typecasting Vulkan function pointers from void*
  #pragma clang diagnostic ignored "-Wcast-function-type-strict"
  PFN_vkCreateDebugUtilsMessengerEXT const result = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  #pragma clang diagnostic pop
  cvk_assert(result != NULL, "Failed to get the Vulkan Debug Messenger create function");
  return result;
}  // clang-format on


cvk_Pure VkDebugUtilsMessengerEXT cvk_validation_debug_context_create (
  VkInstance const                          instance,
  VkDebugUtilsMessengerCreateInfoEXT* const cfg,
  VkAllocationCallbacks* const              allocator
) {
  PFN_vkCreateDebugUtilsMessengerEXT const create = cvk_Fn_get_createDebug(instance);
  VkDebugUtilsMessengerEXT                 result = NULL;
  cvk_result_check(create(instance, cfg, allocator, &result), "Failed to create the Vulkan Debug Messenger Context");
  return result;
}


/// @internal
/// @description
/// Returns the Vulkan Debug Messenger destroy function
static PFN_vkDestroyDebugUtilsMessengerEXT cvk_Fn_get_destroyDebug (
  VkInstance const instance
) {  // clang-format off
  #pragma clang diagnostic push
  // Allow soft typecasting Vulkan function pointers from void*
  #pragma clang diagnostic ignored "-Wcast-function-type-strict"
  PFN_vkDestroyDebugUtilsMessengerEXT const result = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  #pragma clang diagnostic pop
  cvk_assert(result != NULL, "Failed to get the Vulkan Debug Messenger destroy function");
  return result;
}  // clang-format on


void cvk_validation_debug_context_destroy (
  VkInstance const             instance,
  VkDebugUtilsMessengerEXT     debug_ct,
  VkAllocationCallbacks* const allocator
) {
  PFN_vkDestroyDebugUtilsMessengerEXT destroy = cvk_Fn_get_destroyDebug(instance);
  destroy(instance, debug_ct, allocator);
  // TODO:
  cvk_discard(instance);
  cvk_discard(cvk_Fn_get_destroyDebug);
}

