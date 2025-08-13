//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "./application.h"
#include "./config.h"

cvk_Pure cvk_Application cvk_application_defaults () {
  return (cvk_Application){
    .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext              = NULL,
    .applicationVersion = VK_MAKE_VERSION(0, 0, 0),
    .pApplicationName   = "cvulkan | Generic Application",
    .engineVersion      = VK_MAKE_VERSION(0, 0, 0),
    .pEngineName        = "cvulkan | Generic Engine",
    .apiVersion         = cvk_config_vulkan_version,
  };
}

