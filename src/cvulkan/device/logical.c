//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
#include "../result.h"
#include "../device.h"


cvk_Pure VkDeviceCreateInfo cvk_device_logical_options_create (
  cvk_device_Queue const* const    queue,
  cvk_device_Features const* const features,
  cvk_device_Extensions const      extensions
) {
  return (VkDeviceCreateInfo){
    .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext                   = NULL,  // FIX: Should have the device features here
    .queueCreateInfoCount    = 1,
    .pQueueCreateInfos       = &queue->cfg,
    .enabledExtensionCount   = (uint32_t)extensions.len,
    .ppEnabledExtensionNames = (cvk_String const*)extensions.ptr,
    .pEnabledFeatures        = &features->v1_0.features,  // FIX: Should be NULL. Features > v1.0 need to go into pNext
    // Not Used
    .flags               = (VkDeviceCreateFlags)0,  // Vk.Spec -> Reserved for future use
    .enabledLayerCount   = 0,                       // Vk.Spec -> enabledLayerCount is deprecated and should not be used
    .ppEnabledLayerNames = NULL,                    // Vk.Spec -> ppEnabledLayerNames is deprecated and should not be used
  };
}


cvk_Pure cvk_device_Logical cvk_device_logical_create (
  cvk_device_logical_create_args* const arg
) {
  // Create the Features and Extensions
  cvk_device_Logical result = (cvk_device_Logical){
    .ct         = NULL,
    .features   = cvk_device_Features_default(),
    .extensions = cvk_device_Extensions_default(arg->allocator),
  };
  // Create the configuration options
  result.cfg = cvk_device_logical_options_create(arg->queue, &result.features, result.extensions);
  // clang-format off
  cvk_result_check(vkCreateDevice(arg->physical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create a Vulkan Logical Device");  // clang-format on
  // Return the result
  return result;
}


void cvk_device_logical_destroy (
  cvk_device_Logical* const device,
  cvk_Allocator* const      allocator
) {
  allocator->cpu.free(&allocator->cpu, (cvk_Slice*)&device->extensions);
  vkDestroyDevice(device->ct, allocator->gpu);
}

