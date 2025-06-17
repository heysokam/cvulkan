//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
#include "./application.h"
#include "./config.h"

cvk_Pure cvk_Application cvk_Application_defaults () {
  return (cvk_Application){
    .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext              = NULL,
    .applicationVersion = VK_MAKE_VERSION(0, 0, 0),
    .pApplicationName   = "cvk | Generic Application",
    .engineVersion      = VK_MAKE_VERSION(0, 0, 0),
    .pEngineName        = "cvk | Generic Engine",
    .apiVersion         = cvk_config_vulkan_version,
  };
}

