//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
#include "../result.h"
#include "../device.h"


cvk_Pure cvk_bool cvk_device_physical_isSuitable_default (
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
) {
  VkPhysicalDeviceProperties properties = (VkPhysicalDeviceProperties){ 0 };
  vkGetPhysicalDeviceProperties(device->ct, &properties);
  cvk_bool const is_discrete = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;

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
    && supports_extensions && supports_swapchain
    ;  // clang-format on
}  //:: cvk_device_physical_isSuitable_default


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
  // Get the list of available devices
  cvk_device_physical_List available = cvk_device_physical_getAvailable(arg->instance);
  // Get the isSuitable function (user or default)
  cvk_Fn_device_physical_isSuitable const isSuitable = (!arg->isSuitable) ? arg->isSuitable : cvk_device_physical_isSuitable;
  // For each available device, call isSuitable and stop when the condition matches
  cvk_device_Physical result = (cvk_device_Physical){ .ct = NULL, .id = cvk_Optional_u32_none };
  for (cvk_Optional_u32 id = 0; id < available.len; ++id) {
    // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    result = (cvk_device_Physical){ .ct = available.ptr[id], .id = id };
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
    // clang-format on
    // Continue/stop based on suitability
    if (arg->forceFirst || isSuitable(&result, arg->surface, &arg->instance->allocator)) {
      result.queueFamilies    = cvk_device_queue_families_create(&result, arg->surface, &arg->instance->allocator);
      result.swapchainSupport = cvk_device_swapchain_support_create(&result, arg->surface, &arg->instance->allocator);
      break;
    }
  }
  // Cleanup the list of devices
  arg->instance->allocator.cpu.free(&arg->instance->allocator.cpu, (cvk_Slice*)&available);
  // Validate and Return the result
  cvk_assert(cvk_Optional_u32_hasValue(result.id), "Failed to find a Physical Device (GPU) suitable for Vulkan.");
  cvk_assert(result.ct != VK_NULL_HANDLE, "Failed to find a Physical Device (GPU) suitable for Vulkan.");
  return result;
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

