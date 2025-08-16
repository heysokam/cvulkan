//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
#include "../device.h"


cvk_Pure cvk_QueueFamilies cvk_device_queue_families_create (
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  cvk_QueueFamilies result = (cvk_QueueFamilies){
    .properties = (cvk_queueFamilies_properties_List){ .itemsize = sizeof(VkQueueFamilyProperties) },
    .graphics   = cvk_Optional_u32_none,
    .present    = cvk_Optional_u32_none,
    .transfer   = cvk_Optional_u32_none,
    .compute    = cvk_Optional_u32_none,
  };
  // Allocate the properties of all families available for the device
  vkGetPhysicalDeviceQueueFamilyProperties(device->ct, (uint32_t*)&result.properties.len, NULL);
  if (result.properties.len) {
    cvk_Slice data = allocator->cpu.allocZ(&allocator->cpu, result.properties.len, result.properties.itemsize);
    vkGetPhysicalDeviceQueueFamilyProperties(device->ct, (uint32_t*)&data.len, data.ptr);
    result.properties.len = data.len;
    result.properties.ptr = (VkQueueFamilyProperties*)data.ptr;
  }
  // Populate the Queue IDs
  for (cvk_Optional_u32 id = 0; id < result.properties.len; ++id) {
    VkQueueFamilyProperties const prop = result.properties.ptr[id];
    if (result.graphics == cvk_Optional_u32_none && prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) result.graphics = id;
    cvk_bool const transfer_only = prop.queueFlags & VK_QUEUE_TRANSFER_BIT && !(prop.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT));
    if (result.transfer == cvk_Optional_u32_none && transfer_only) result.transfer = id;
    cvk_bool const compute_only = (prop.queueFlags & VK_QUEUE_COMPUTE_BIT) && !(prop.queueFlags & VK_QUEUE_GRAPHICS_BIT);
    if (result.compute == cvk_Optional_u32_none && compute_only) result.compute = id;
    VkBool32 canPresent = 0;
    vkGetPhysicalDeviceSurfaceSupportKHR(device->ct, id, surface, &canPresent);
    if (result.present == cvk_Optional_u32_none && canPresent) result.present = id;
  }
  // Return the result
  return result;
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
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


void cvk_device_queue_submit (
  cvk_device_Queue const* const             queue,
  cvk_device_queue_submit_args const* const arg
) {  // clang-format off
  cvk_result_check(vkQueueSubmit(queue->ct, 1, &(VkSubmitInfo){
    .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext                = NULL,
    .waitSemaphoreCount   = arg->semaphore_wait ? 1 : 0,
    .pWaitSemaphores      = arg->semaphore_wait ? &arg->semaphore_wait->ct : NULL,
    .pWaitDstStageMask    = &(VkPipelineStageFlags){VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
    .commandBufferCount   = 1,
    .pCommandBuffers      = &arg->command_buffer->ct,
    .signalSemaphoreCount = arg->semaphore_signal ? 1 : 0,
    .pSignalSemaphores    = arg->semaphore_signal ? &arg->semaphore_signal->ct : NULL,
    }, /* fence */ arg->fence ? arg->fence->ct : NULL
    ), "Failed to submit a Command Buffer to the given Device.Queue");
}  // clang-format on


void cvk_device_queue_wait (
  cvk_device_Queue const* const queue
) {  // clang-format off
  cvk_result_check(vkQueueWaitIdle(queue->ct),
    "Failed while waiting for a Queue to finish.");  // clang-format on
}

