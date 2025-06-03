//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
#include "./application.h"
#include "./result.h"
#include "./instance.h"
#include <vulkan/vk_platform.h>


//______________________________________
// @section Instance: Options
//____________________________

cvk_Pure VkInstanceCreateInfo cvk_instance_options_create (
  cvk_Application const* const application,
  VkInstanceCreateFlags const  flags,
  cvk_Slice const              layers,
  cvk_Slice const              extensions
) {
  return (VkInstanceCreateInfo){
    .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext                   = NULL,
    .flags                   = flags,
    .pApplicationInfo        = application,
    .enabledLayerCount       = (uint32_t)layers.len,
    .ppEnabledLayerNames     = layers.ptr,
    .enabledExtensionCount   = (uint32_t)extensions.len,
    .ppEnabledExtensionNames = extensions.ptr,
  };
}


//______________________________________
// @section Instance: Initialize/Allocate/Create
//____________________________
cvk_Pure cvk_Instance cvk_instance_create (
  cvk_instance_create_args const* const arg
) {
  cvk_Instance result                   = (cvk_Instance){ 0 };
  result.allocator                      = (arg->allocator) ? *arg->allocator : cvk_Allocator_defaults();
  result.application                    = cvk_Application_defaults();
  result.application.pApplicationName   = arg->app_name;
  result.application.applicationVersion = arg->app_version;
  result.application.pEngineName        = arg->engine_name;
  result.application.engineVersion      = arg->engine_version;

  cvk_Slice layers     = cvk_Slice_empty();
  cvk_Slice extensions = cvk_Slice_empty();
  result.cfg           = cvk_instance_options_create(&result.application, 0, layers, extensions);
  // result.cfg.pNext     = (void*)&result.validation.debug_cfg;
  cvk_result_check(vkCreateInstance(&result.cfg, result.allocator.gpu, &result.ct), "Failed to create the Vulkan Instance.");
  return result;
}


//______________________________________
// @section Instance: Terminate/Deallocate/Destroy
//____________________________

void cvk_instance_destroy (
  cvk_Instance* const instance
) {
  // TODO: Deallocate the extensions
  vkDestroyInstance(instance->ct, instance->allocator.gpu);
}

