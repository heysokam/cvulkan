//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
#include "../result.h"
#include "../device.h"

cvk_Pure cvk_QueueFamilies cvk_device_queue_families_create (
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
) {
  cvk_QueueFamilies result = (cvk_QueueFamilies){
    .properties = (cvk_queueFamilies_properties_List){ 0 },
    .graphics   = cvk_Optional_u32_none,
    .present    = cvk_Optional_u32_none,
    .transfer   = cvk_Optional_u32_none,
  };
  // Allocate the properties of all families available for the device
  vkGetPhysicalDeviceQueueFamilyProperties(device->ct, (uint32_t*)&result.properties.len, NULL);
  if (result.properties.len) {
    cvk_Slice data = allocator->cpu.allocZ(&allocator->cpu, result.properties.len, sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device->ct, (uint32_t*)&data.len, data.ptr);
    result.properties.len = data.len;
    result.properties.ptr = (VkQueueFamilyProperties*)data.ptr;
  }
  // Populate the Queue IDs
  for (cvk_Optional_u32 id = 0; id < result.properties.len; ++id) {
    // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    VkQueueFamilyProperties const prop = result.properties.ptr[id];
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
    // clang-format on
    if (result.graphics == cvk_Optional_u32_none && prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) result.graphics = id;
    if (result.compute == cvk_Optional_u32_none && prop.queueFlags & VK_QUEUE_COMPUTE_BIT) result.compute = id;
    cvk_bool const transferOnly = prop.queueFlags & VK_QUEUE_TRANSFER_BIT && !(prop.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT));
    if (result.transfer == cvk_Optional_u32_none && transferOnly) result.transfer = id;
    VkBool32 canPresent = 0;
    vkGetPhysicalDeviceSurfaceSupportKHR(device->ct, id, surface, &canPresent);
    if (result.present == cvk_Optional_u32_none && canPresent) result.present = id;
    // TODO: Are these needed? They seem niche.
    //       SparseBinding and Protected seem to be flags, not queue types
    //       Video En/Decode are for vkvideo, not graphics
    //       OpticalFlow is an nvidia extension for video
    //       The user of these might need their own Physical Device search anyway? :think:
    // if (prop.queueFlags & VK_QUEUE_PROTECTED_BIT) result.protected = id;
    // if (prop.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) result.sparseBinding = id;
    // if (prop.queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) result.video_decode = id;
    // if (prop.queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) result.video_encode = id;
    // if (prop.queueFlags & VK_QUEUE_OPTICAL_FLOW_BIT_NV) result.opticalFlow  = id;
  }
  // Return the result
  return result;
}  //:: cvk_device_queue_families_create


void cvk_device_queue_families_destroy (
  cvk_QueueFamilies* const queueFamilies,
  cvk_Allocator* const     allocator
) {
  allocator->cpu.free(&allocator->cpu, (cvk_Slice*)&queueFamilies->properties);
  queueFamilies->graphics = cvk_Optional_u32_none;
  queueFamilies->present  = cvk_Optional_u32_none;
  queueFamilies->transfer = cvk_Optional_u32_none;
  queueFamilies->compute  = cvk_Optional_u32_none;
}  //:: cvk_device_queue_families_destroy


cvk_Pure VkDeviceQueueCreateInfo cvk_device_queue_options_create (
  cvk_QueueID  family,
  uint32_t     count,
  float const* priorities,
  cvk_bool     Protected
) {
  return (VkDeviceQueueCreateInfo){
    .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .pNext            = NULL,
    .flags            = (Protected) ? VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT : (VkDeviceQueueCreateFlags)0,
    .queueFamilyIndex = family,
    .queueCount       = count,
    .pQueuePriorities = priorities,

  };
}


static cvk_Pure cvk_QueuePriority cvk_QueuePriority_clamp (
  cvk_QueuePriority const priority
) {  // clang-format off
  return (priority < 0.0f) ? 0.0f
  : (priority > 1.0f) ? 1.0f
  : priority;
}  // clang-format on


cvk_Pure cvk_device_Queue cvk_device_queue_create_noContext (
  cvk_device_queue_create_args* const arg
) {
  cvk_device_Queue result = (cvk_device_Queue){
    .ct       = NULL,
    .priority = cvk_QueuePriority_clamp(arg->priority),
  };
  result.cfg = cvk_device_queue_options_create(
    /* family     */ arg->id,
    /* count      */ 1,
    /* priorities */ &result.priority,
    /* Protected  */ arg->Protected
  );
  return result;
}


void cvk_device_queue_create_context (
  cvk_device_Queue* const         queue,
  cvk_device_Logical const* const device
) {
  vkGetDeviceQueue(device->ct, queue->cfg.queueFamilyIndex, 0, &queue->ct);
}

