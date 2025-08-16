//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview cvulkan configuration options
//______________________________________________|
#ifndef H_cvk_config
#define H_cvk_config
#include "./base.h"


#ifndef cvk_config_Name
/// @description
/// Default name used by the library for human-readable messages.  (eg: default DebugMessenger callback label)
#define cvk_config_Name "cvulkan"
#endif// cvk_config_Name


#ifndef cvk_config_vulkan_version
/// @description
/// Default Vulkan API version targeted by cvulkan
#define cvk_config_vulkan_version VK_API_VERSION_1_4
#endif  // cvk_config_vulkan_version


#ifndef cvk_config_validation_active
/// @description
/// Whether Validation Layers will be active by default or not.
#define cvk_config_validation_active cvk_true
#endif  // cvk_config_validation_active


#ifndef cvk_config_debug_active
/// @description
/// Whether DebugMessenger functionality will be active by default or not.
#define cvk_config_debug_active cvk_config_validation_active
#endif  // cvk_config_debug_active


#ifndef cvk_config_debug_severity
/// @description
/// Default DebugMessenger severity used by cvulkan.
#define cvk_config_debug_severity \
  VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
#endif  // cvk_config_validation_Debug_severity


#ifndef cvk_config_debug_msgType
/// @description
/// Default DebugMessenger messageType used by cvulkan.
#define cvk_config_debug_msgType \
  VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
#endif  // cvk_config_validation_debug_msgType


#ifndef cvk_config_debug_flags
/// @description
/// Default DebugMessenger initialization flags used by cvulkan.
#define cvk_config_debug_flags (VkDebugUtilsMessengerCreateFlagsEXT)(0)
#endif  // cvk_config_validation_debug_flags


#ifndef cvk_config_debug_callback
/// @description
/// Default DebugMessenger callback function used by cvulkan.
#define cvk_config_debug_callback cvk_validation_debug_callback
#endif  // cvk_config_debug_callback


#ifndef cvk_config_debug_userdata
/// @description
/// Default userdata for the DebugMessenger callback function created by cvulkan.
#define cvk_config_debug_userdata NULL
#endif  // cvk_config_debug_userdata


#endif  // H_cvk_config

