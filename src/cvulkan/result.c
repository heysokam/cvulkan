//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
#include "./result.h"
#include "./config.h"

cvk_String cvk_result_toString_default (
  VkResult status
) {
  switch (status) {
    case VK_SUCCESS                                            : return "[" cvk_config_Name ".Ok]";
    case VK_NOT_READY                                          : return "[" cvk_config_Name ".Warn](Not Ready)";
    case VK_TIMEOUT                                            : return "[" cvk_config_Name ".Warn](Timeout)";
    case VK_EVENT_SET                                          : return "[" cvk_config_Name ".Warn](Event Set)";
    case VK_EVENT_RESET                                        : return "[" cvk_config_Name ".Warn](Event Reset)";
    case VK_INCOMPLETE                                         : return "[" cvk_config_Name ".Warn](Incomplete)";
    case VK_ERROR_OUT_OF_HOST_MEMORY                           : return "[" cvk_config_Name ".Error](Out of Host Memory)";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY                         : return "[" cvk_config_Name ".Error](Out of Device Memory)";
    case VK_ERROR_INITIALIZATION_FAILED                        : return "[" cvk_config_Name ".Error](Initialization Failed)";
    case VK_ERROR_DEVICE_LOST                                  : return "[" cvk_config_Name ".Error](Device Lost)";
    case VK_ERROR_MEMORY_MAP_FAILED                            : return "[" cvk_config_Name ".Error](Memory Map Failed)";
    case VK_ERROR_LAYER_NOT_PRESENT                            : return "[" cvk_config_Name ".Error](Layer not Present)";
    case VK_ERROR_EXTENSION_NOT_PRESENT                        : return "[" cvk_config_Name ".Error](Extension not Present)";
    case VK_ERROR_FEATURE_NOT_PRESENT                          : return "[" cvk_config_Name ".Error](Feature not Present)";
    case VK_ERROR_INCOMPATIBLE_DRIVER                          : return "[" cvk_config_Name ".Error](Incompatible Driver)";
    case VK_ERROR_TOO_MANY_OBJECTS                             : return "[" cvk_config_Name ".Error](Too Many Objects)";
    case VK_ERROR_FORMAT_NOT_SUPPORTED                         : return "[" cvk_config_Name ".Error](Format not Supported)";
    case VK_ERROR_FRAGMENTED_POOL                              : return "[" cvk_config_Name ".Error](Fragmented Pool)";
    case VK_ERROR_UNKNOWN                                      : return "[" cvk_config_Name ".Error](Unknown)";
    case VK_ERROR_OUT_OF_POOL_MEMORY                           : return "[" cvk_config_Name ".Error](Out of Pool Memory)";
    case VK_ERROR_INVALID_EXTERNAL_HANDLE                      : return "[" cvk_config_Name ".Error](Invalid External Handle)";
    case VK_ERROR_FRAGMENTATION                                : return "[" cvk_config_Name ".Error](Fragmentation)";
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS               : return "[" cvk_config_Name ".Error](Invalid Opaque Capture Address)";
    case VK_PIPELINE_COMPILE_REQUIRED                          : return "[" cvk_config_Name ".Error](Pipeline Compile Required)";
    case VK_ERROR_NOT_PERMITTED                                : return "[" cvk_config_Name ".Error](Not Permitted)";
    case VK_ERROR_SURFACE_LOST_KHR                             : return "[" cvk_config_Name ".Error](Surface Lost)";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR                     : return "[" cvk_config_Name ".Error](Native Window in Use)";
    case VK_SUBOPTIMAL_KHR                                     : return "[" cvk_config_Name ".Error](Suboptimal)";
    case VK_ERROR_OUT_OF_DATE_KHR                              : return "[" cvk_config_Name ".Error](Out of Date)";
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR                     : return "[" cvk_config_Name ".Error](Incompatible Display)";
    case VK_ERROR_VALIDATION_FAILED_EXT                        : return "[" cvk_config_Name ".Error](Validation Failed)";
    case VK_ERROR_INVALID_SHADER_NV                            : return "[" cvk_config_Name ".Error](Invalid Shader)";
    case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR                : return "[" cvk_config_Name ".Error](Image usage not Supported)";
    case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR       : return "[" cvk_config_Name ".Error](Video.Picture Layout Not Supported)";
    case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR    : return "[" cvk_config_Name ".Error](Video.Profile Operation Not Supported)";
    case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR       : return "[" cvk_config_Name ".Error](Video.Profile Format Not Supported)";
    case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR        : return "[" cvk_config_Name ".Error](Video.Profile Codec Not Supported)";
    case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR          : return "[" cvk_config_Name ".Error](Video.STD Version Not Supported)";
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT : return "[" cvk_config_Name ".Error](Invalid DRM Format Modifier Plane Layout)";
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT          : return "[" cvk_config_Name ".Error](Full Screen Exclusive Mode Lost)";
    case VK_THREAD_IDLE_KHR                                    : return "[" cvk_config_Name ".Warn](Thread Idle)";
    case VK_THREAD_DONE_KHR                                    : return "[" cvk_config_Name ".Info](Thread Done)";
    case VK_OPERATION_DEFERRED_KHR                             : return "[" cvk_config_Name ".Warn](Operation Deferred)";
    case VK_OPERATION_NOT_DEFERRED_KHR                         : return "[" cvk_config_Name ".Warn](Operation Not Deferred)";
    case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR             : return "[" cvk_config_Name ".Error](Video.STD Invalid Parameters)";
    case VK_ERROR_COMPRESSION_EXHAUSTED_EXT                    : return "[" cvk_config_Name ".Error](Compression Exhausted)";
    case VK_INCOMPATIBLE_SHADER_BINARY_EXT                     : return "[" cvk_config_Name ".Warn](Incompatible Shader Binary)";
    case VK_PIPELINE_BINARY_MISSING_KHR                        : return "[" cvk_config_Name ".Error](Pipeline Binary Missing)";
    case VK_ERROR_NOT_ENOUGH_SPACE_KHR                         : return "[" cvk_config_Name ".Error](Not Enough Space)";
    case VK_RESULT_MAX_ENUM                                    : /* fall-through */
    default                                                    : return "[" cvk_config_Name ".UnknownStatus]";
  }
}

