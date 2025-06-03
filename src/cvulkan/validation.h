//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
#ifndef H_cvulkan_validation
#define H_cvulkan_validation
#include "./base.h"


//______________________________________
// @section Validation Data
//____________________________
typedef struct cvk_Validation {
  VkDebugUtilsMessengerEXT           debug_ct;
  VkDebugUtilsMessengerCreateInfoEXT debug_cfg;
  cvk_bool                           debug_active;
  cvk_bool                           layers_active;
} cvk_Validation;


//______________________________________
// @section Validation Options
//____________________________
typedef struct cvk_validation_Options {
  cvk_Readonly cvk_bool                             layers_active;  ///< Will enable validation layers when true
  cvk_Readonly cvk_bool                             debug_active;   ///< Will enable debug messenger when true
  cvk_Readonly VkDebugUtilsMessageSeverityFlagsEXT  debug_severity;
  cvk_Readonly VkDebugUtilsMessageTypeFlagsEXT      debug_msgType;
  cvk_Readonly PFN_vkDebugUtilsMessengerCallbackEXT debug_callback;
  cvk_Readonly cvk_pointer                          debug_userdata;
} cvk_validation_Options;
cvk_Pure cvk_validation_Options cvk_validation_Options_defaults ();


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
