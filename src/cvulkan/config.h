//:________________________________________________________________
//  cvk  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:________________________________________________________________
#ifndef H_cvk_config
#define H_cvk_config
#include "./base.h"

#ifndef cvk_config_vulkan_version
#define cvk_config_vulkan_version VK_API_VERSION_1_4
#endif  // cvk_config_vulkan_version


#ifndef cvk_config_validation_active
#define cvk_config_validation_active true
#endif  // cvk_config_validation_active


#ifndef cvk_config_debug_active
#define cvk_config_debug_active cvk_config_validation_active
#endif  // cvk_config_debug_active


#ifndef cvk_config_debug_severity
#define cvk_config_debug_severity \
  VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
#endif  // cvk_config_validation_Debug_severity


#ifndef cvk_config_debug_msgType
#define cvk_config_debug_msgType \
  VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
#endif  // cvk_config_validation_debug_msgType


#ifndef cvk_config_debug_flags
#define cvk_config_debug_flags (VkDebugUtilsMessengerCreateFlagsEXT)(0)
#endif  // cvk_config_validation_debug_flags


#endif  // H_cvk_config
