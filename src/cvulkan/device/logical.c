//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
#include "../device.h"


cvk_Pure VkDeviceCreateInfo cvk_device_logical_options_create (
  cvk_device_Queue const* const    queue,
  cvk_device_Features const* const features,
  cvk_device_Extensions const      extensions
) {
  return (VkDeviceCreateInfo) /* clang-format off */ {
    .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    #ifndef VK_VERSION_1_1
    .pNext                   = NULL,
    #else // >= VK_VERSION_1_1
    .pNext                   = &features->list,
    #endif// >= VK_VERSION_1_1
    .queueCreateInfoCount    = 1,
    .pQueueCreateInfos       = &queue->cfg,
    .enabledExtensionCount   = (uint32_t)extensions.len,
    .ppEnabledExtensionNames = (cvk_String const*)extensions.ptr,
    #ifndef VK_VERSION_1_1
    .pEnabledFeatures        = &features->list,
    #else // >= VK_VERSION_1_1
    .pEnabledFeatures        = NULL,
    #endif// >= VK_VERSION_1_1
    // Not Used
    .flags                   = (VkDeviceCreateFlags)0,  // Vk.Spec -> Reserved for future use
    .enabledLayerCount       = 0,                       // Vk.Spec -> enabledLayerCount is deprecated and should not be used
    .ppEnabledLayerNames     = NULL,                    // Vk.Spec -> ppEnabledLayerNames is deprecated and should not be used
  }; // clang-format on
}


cvk_Pure cvk_device_Logical cvk_device_logical_create (
  cvk_device_logical_create_args* const arg
) {
  cvk_device_Logical result = (cvk_device_Logical){ 0 };
  // Create the Extensions
  result.extensions = /* clang-format off */ cvk_device_extensions_create(arg->extensions, &(cvk_device_extensions_create_args){
    .device    = arg->physical,
    .allocator = arg->allocator,
  });
  // Create the Features
  result.features = cvk_device_features_merge(arg->features);
  // Create the configuration options
  result.cfg = cvk_device_logical_options_create(arg->queue, &result.features, result.extensions);
  // Create the context/handle
  cvk_result_check(/* clang-format off */vkCreateDevice(arg->physical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create a Vulkan Logical Device");  // clang-format on
  // Return the result
  return result;
}


void cvk_device_logical_destroy (
  cvk_device_Logical* const device,
  cvk_Allocator* const      allocator
) {
  cvk_device_features_clear(&device->features);
  cvk_device_extensions_destroy(&device->extensions, allocator);
  device->cfg = (VkDeviceCreateInfo){ 0 };
  vkDestroyDevice(device->ct, allocator->gpu);
}


inline void cvk_device_logical_wait (
  cvk_device_Logical const* const device_logical
) {
  vkDeviceWaitIdle(device_logical->ct);
}

