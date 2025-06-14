//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
#include "../result.h"
#include "../device.h"


cvk_Pure cvk_bool cvk_device_physical_isSuitable_default (
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
) {
  cvk_bool result = cvk_false;
  // Allocate the list of QueueFamilies of this device
  cvk_QueueFamilies queueFamilies = cvk_queue_families_create(device, surface, allocator);
  // Find if the device is suitable
  if (cvk_Optional_u32_hasValue(queueFamilies.graphics)) result = cvk_true;
  // Cleanup the QueueFamilies list we just allocated
  cvk_queue_families_destroy(&queueFamilies, allocator);
  return result;
}  //:: cvk_device_physical_isSuitable_default


cvk_Pure cvk_Slice /* VkPhysicalDevice[] */ cvk_device_physical_getAvailable (
  cvk_Instance* const instance
) {
  cvk_Slice result = cvk_Slice_empty();
  cvk_result_check(vkEnumeratePhysicalDevices(instance->ct, (uint32_t*)&result.len, NULL), "Failed when searching for GPUs with Vulkan support.");
  cvk_assert(result.len, "Failed to find any GPUs with Vulkan support.");
  result = /* VkPhysicalDevice[] */ instance->allocator.cpu.allocZ(&instance->allocator.cpu, result.len, sizeof(VkPhysicalDevice));
  cvk_result_check(vkEnumeratePhysicalDevices(instance->ct, (uint32_t*)&result.len, result.ptr), "Failed to retrieve the list of GPUs.");
  return result;
}  //:: cvk_device_physical_getAvailable


cvk_Pure cvk_device_Physical cvk_device_physical_create (
  cvk_device_physical_create_args* const arg
) {
  // Get the list of available devices
  cvk_Slice available = cvk_device_physical_getAvailable(arg->instance);
  // Get the isSuitable function (user or default)
  cvk_Fn_device_physical_isSuitable const isSuitable = (!arg->isSuitable) ? arg->isSuitable : cvk_device_physical_isSuitable;
  // For each available device, call isSuitable and stop when the condition matches
  cvk_device_Physical result = (cvk_device_Physical){ .ct = NULL, .id = cvk_Optional_u32_none };
  for (cvk_Optional_u32 id = 0; id < available.len; ++id) {  // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    result = (cvk_device_Physical){ .ct = ((VkPhysicalDevice*)available.ptr)[id], .id = id };
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
    if (arg->forceFirst) break;  // clang-format on
    // Continue/stop based on suitability
    if (isSuitable(&result, arg->surface, &arg->instance->allocator)) {
      result.queueFamilies = cvk_queue_families_create(&result, arg->surface, &arg->instance->allocator);
      break;
    }
  }
  // Cleanup the list of devices
  arg->instance->allocator.cpu.free(&arg->instance->allocator.cpu, available);
  // Validate and Return the result
  cvk_assert(cvk_Optional_u32_hasValue(result.id), "Failed to find a Physical Device (GPU) suitable for Vulkan.");
  cvk_assert(result.ct != VK_NULL_HANDLE, "Failed to find a Physical Device (GPU) suitable for Vulkan.");
  return result;
}  //:: cvk_device_physical_create


void cvk_device_physical_destroy (
  cvk_device_Physical* const device,
  cvk_Allocator* const       allocator
) {
  cvk_queue_families_destroy(&device->queueFamilies, allocator);
  device->ct = NULL;
  device->id = cvk_Optional_u32_none;
}

