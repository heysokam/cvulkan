//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
#include "../device.h"


cvk_Pure cvk_bool cvk_device_physical_isSuitable_default (
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
) {
  // WARN: The .properties field is not yet populated when this function is called
  VkPhysicalDeviceProperties properties = (VkPhysicalDeviceProperties){ 0 };
  vkGetPhysicalDeviceProperties(device->ct, &properties);
  cvk_bool const is_discrete = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;

  // WARN: The .features field is not yet populated when this function is called
  VkPhysicalDeviceFeatures features = (VkPhysicalDeviceFeatures){ 0 };
  vkGetPhysicalDeviceFeatures(device->ct, &features);
  cvk_bool const supports_anisotropy = features.samplerAnisotropy == VK_TRUE;

  cvk_bool const supports_features = supports_anisotropy;

  cvk_QueueFamilies queueFamilies = cvk_device_queue_families_create(device, surface, allocator);
  cvk_bool const    has_graphics  = cvk_Optional_u32_hasValue(queueFamilies.graphics);
  cvk_bool const    has_present   = cvk_Optional_u32_hasValue(queueFamilies.present);
  cvk_device_queue_families_destroy(&queueFamilies, allocator);

  cvk_device_Extensions extensions          = cvk_device_Extensions_default(allocator);
  cvk_bool const        supports_extensions = cvk_device_extensions_supported(device, extensions, allocator);
  allocator->cpu.free(&allocator->cpu, (cvk_Slice*)&extensions);

  cvk_bool supports_swapchain = cvk_false;
  if (supports_extensions) {
    cvk_device_swapchain_Support data = cvk_device_swapchain_support_create(device, surface, allocator);
    supports_swapchain                = data.formats.len > 0 && data.modes.len > 0;
    cvk_device_swapchain_support_destroy(&data, allocator);
  }
  // Find if the device is suitable
  return  // clang-format off
    is_discrete
    && has_graphics && has_present
    && supports_features && supports_extensions && supports_swapchain
    ;  // clang-format on
}  //:: cvk_device_physical_isSuitable_default


cvk_Pure cvk_size cvk_device_physical_getScore_default (
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
) {
  cvk_discard(device);
  cvk_discard(surface);
  cvk_discard(allocator);
  return 1;  // @note Score must be higher than 0 to be ranked. A value of 0 will be ignored.
}  //:: cvk_device_physical_getScore_default


cvk_Pure cvk_device_physical_List cvk_device_physical_getAvailable (
  cvk_Instance* const instance
) {
  cvk_device_physical_List result = (cvk_device_physical_List){ .itemsize = sizeof(VkPhysicalDevice) };
  // clang-format off
  cvk_result_check(vkEnumeratePhysicalDevices(instance->ct, (uint32_t*)&result.len, NULL),
    "Failed when searching for GPUs with Vulkan support.");
  cvk_assert(result.len, "Failed to find any GPUs with Vulkan support.");
  if (result.len) {
    cvk_Slice data = instance->allocator.cpu.allocZ(&instance->allocator.cpu, result.len, result.itemsize);
    cvk_result_check(vkEnumeratePhysicalDevices(instance->ct, (uint32_t*)&data.len, data.ptr), "Failed to retrieve the list of GPUs.");
    result.len = data.len;
    result.ptr = (VkPhysicalDevice*)data.ptr;
  }  // clang-format on
  return result;
}  //:: cvk_device_physical_getAvailable


cvk_Pure cvk_device_Physical cvk_device_physical_create (
  cvk_device_physical_create_args* const arg
) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  // Get the list of available devices
  cvk_device_physical_List available = cvk_device_physical_getAvailable(arg->instance);
  // Get the isSuitable/getScore functions (user or default)
  cvk_Fn_device_physical_isSuitable const isSuitable = (arg->isSuitable) ? arg->isSuitable : cvk_device_physical_isSuitable;
  cvk_Fn_device_physical_getScore const   getScore   = (arg->getScore) ? arg->getScore : cvk_device_physical_getScore;
  // For each available device, call isSuitable and stop when the condition matches
  cvk_device_Physical       result    = (cvk_device_Physical){ .ct = NULL, .id = cvk_Optional_u32_none };
  cvk_device_physical_Score rank_best = 0;
  for (cvk_Optional_u32 id = 0; id < available.len; ++id) {
    cvk_device_Physical const current = (cvk_device_Physical){ .ct = available.ptr[id], .id = id };
    // Continue/stop based on suitability and ranking
    if (arg->forceFirst) { /* clang-format off */ result = current; break; } /* clang-format on */
    if (!isSuitable(&current, arg->surface, &arg->instance->allocator)) continue;
    cvk_size rank_current = getScore(&current, arg->surface, &arg->instance->allocator);
    if (rank_current < rank_best) continue;
    if (rank_current == rank_best) continue;  // @note Tie breaker on equal scores: First found ranks higher.
    result = current;
  }
  // Cleanup the list of devices
  arg->instance->allocator.cpu.free(&arg->instance->allocator.cpu, (cvk_Slice*)&available);
  // Validate
  cvk_assert(cvk_Optional_u32_hasValue(result.id), "Failed to find a Physical Device (GPU) suitable for Vulkan.");
  cvk_assert(result.ct != VK_NULL_HANDLE, "Failed to find a Physical Device (GPU) suitable for Vulkan.");
  // Get the Features, Properties, QueueFamilies and SwapchainSupport of the device
  result.queueFamilies    = cvk_device_queue_families_create(&result, arg->surface, &arg->instance->allocator);
  result.swapchainSupport = cvk_device_swapchain_support_create(&result, arg->surface, &arg->instance->allocator);
  vkGetPhysicalDeviceFeatures(result.ct, &result.features);
  vkGetPhysicalDeviceProperties(result.ct, &result.properties);
  vkGetPhysicalDeviceMemoryProperties(result.ct, &result.memory);
  // Return the result
  return result;
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
}  //:: cvk_device_physical_create


void cvk_device_physical_destroy (
  cvk_device_Physical* const device,
  cvk_Allocator* const       allocator
) {
  cvk_device_swapchain_support_destroy(&device->swapchainSupport, allocator);
  cvk_device_queue_families_destroy(&device->queueFamilies, allocator);
  device->id = cvk_Optional_u32_none;
  device->ct = NULL;
}

