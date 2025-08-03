//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../sync.h"
#include "../result.h"
#include "../math.h"
#include "../device.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
cvk_Pure VkSurfaceFormatKHR cvk_device_swapchain_select_format (
  cvk_device_swapchain_Support const* const support
) {
  cvk_assert(support->formats.len, "The swapchain formats list must not be empty.");
  VkSurfaceFormatKHR result = support->formats.ptr[0];
  for (cvk_size id = 0; id < support->formats.len; ++id) {
    result               = support->formats.ptr[id];
    cvk_bool const valid = result.format == VK_FORMAT_B8G8R8A8_SRGB && result.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    if (valid) return result;
  }
  return support->formats.ptr[0];
}
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
cvk_Pure VkPresentModeKHR cvk_device_swapchain_select_mode (
  cvk_device_swapchain_Support const* const support
) {
  cvk_assert(support->modes.len, "The swapchain modes list must not be empty.");
  VkPresentModeKHR result = VK_PRESENT_MODE_FIFO_KHR;
  for (cvk_size id = 0; id < support->formats.len; ++id) {
    result               = support->modes.ptr[id];
    cvk_bool const valid = result == VK_PRESENT_MODE_MAILBOX_KHR;
    if (valid) return result;
  }
  return VK_PRESENT_MODE_FIFO_KHR;  // Guaranteed to exist by spec
}
#pragma GCC diagnostic pop          // -Wunsafe-buffer-usage


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
cvk_Pure VkExtent2D cvk_device_swapchain_select_size (
  cvk_device_swapchain_Support const* const support,
  cvk_Size2D const                          size
) {
  if (support->capabilities.currentExtent.width != UINT32_MAX) return support->capabilities.currentExtent;
  VkExtent2D result = {
    .width  = cvk_clamp_u32(size.width, support->capabilities.minImageExtent.width, support->capabilities.maxImageExtent.width),
    .height = cvk_clamp_u32(size.height, support->capabilities.minImageExtent.height, support->capabilities.maxImageExtent.height),
  };
  return result;
}
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage


cvk_Pure cvk_size cvk_device_swapchain_select_imgMin (
  cvk_device_swapchain_Support const* const support
) {
  cvk_size result = support->capabilities.minImageCount + 1;
  if (support->capabilities.maxImageCount) result = cvk_max_u32((uint32_t)result, support->capabilities.maxImageCount);
  return result;
}


cvk_Pure cvk_device_swapchain_Support cvk_device_swapchain_support_create (
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
) {
  cvk_device_swapchain_Support result = {
    .formats = { .itemsize = sizeof(VkSurfaceFormatKHR) },
    .modes   = { .itemsize = sizeof(VkPresentModeKHR) },
  };

  // clang-format off
  cvk_result_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->ct, surface, &result.capabilities),
    "Failed to retrieve the Swapchain Surface capabilities for the selected device.");
  // clang-format on

  // clang-format off
  cvk_result_check(vkGetPhysicalDeviceSurfaceFormatsKHR(device->ct, surface, (uint32_t*)&result.formats.len, NULL),
    "Failed to retrieve the number of Swapchain Formats for the selected device.");
  if (result.formats.len) {
    cvk_Slice data = allocator->cpu.allocZ(&allocator->cpu, result.formats.len, result.formats.itemsize);
    cvk_result_check(vkGetPhysicalDeviceSurfaceFormatsKHR(device->ct, surface, (uint32_t*)&data.len, data.ptr),
      "Failed to retrieve the list of Swapchain Formats for the selected device.");
    result.formats.len = data.len;
    result.formats.ptr = (VkSurfaceFormatKHR*)data.ptr;
  }  // clang-format on

  // clang-format off
  cvk_result_check(vkGetPhysicalDeviceSurfacePresentModesKHR(device->ct, surface, (uint32_t*)&result.modes.len, NULL),
    "Failed to retrieve the number of Swapchain Present Modes for the selected device.");
  if (result.modes.len) {
    cvk_Slice data = allocator->cpu.allocZ(&allocator->cpu, result.modes.len, result.modes.itemsize);
    cvk_result_check(vkGetPhysicalDeviceSurfacePresentModesKHR(device->ct, surface, (uint32_t*)&data.len, data.ptr),
      "Failed to retrieve the list of Swapchain Present Modes for the selected device.");
    result.modes.len = data.len;
    result.modes.ptr = (VkPresentModeKHR*)data.ptr;
  }  // clang-format on

  return result;
}

void cvk_device_swapchain_support_destroy (
  cvk_device_swapchain_Support* const support,
  cvk_Allocator* const                allocator
) {
  support->capabilities = (VkSurfaceCapabilitiesKHR){ 0 };
  allocator->cpu.free(&allocator->cpu, (cvk_Slice*)&support->formats);
  allocator->cpu.free(&allocator->cpu, (cvk_Slice*)&support->modes);
}


cvk_Pure VkSwapchainCreateInfoKHR cvk_device_swapchain_options_create (
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_size const                   img_min,
  VkSurfaceFormatKHR const         format,
  cvk_Size2D const                 size,
  VkSurfaceCapabilitiesKHR*        capabilities,
  VkPresentModeKHR const           mode,
  cvk_Allocator* const             allocator
) {
  VkSharingMode sharingMode = 0;
  cvk_Slice     families    = cvk_Slice_empty();
  if (device->queueFamilies.graphics == device->queueFamilies.present) {
    sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  } else if (cvk_Optional_u32_hasValue(device->queueFamilies.graphics) && cvk_Optional_u32_hasValue(device->queueFamilies.present)) {
    sharingMode  = VK_SHARING_MODE_CONCURRENT;
    families.len = 2;
    families     = allocator->cpu.allocZ(&allocator->cpu, families.len, sizeof(uint32_t));
    // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    ((uint32_t*)families.ptr)[0] = device->queueFamilies.graphics;
    ((uint32_t*)families.ptr)[1] = device->queueFamilies.present;
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
    // clang-format on
  } else {
    cvk_assert(cvk_false, "Something went wrong when defining the Swapchain Sharing mode.");
  }
  return (VkSwapchainCreateInfoKHR){
    .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext                 = NULL,
    .flags                 = (VkSwapchainCreateFlagsKHR)0,  // TODO: Allow Config
    .surface               = surface,
    .minImageCount         = (uint32_t)img_min,
    .imageFormat           = format.format,
    .imageColorSpace       = format.colorSpace,
    .imageExtent           = size,
    .imageArrayLayers      = 1,
    .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    .imageSharingMode      = sharingMode,
    .queueFamilyIndexCount = (uint32_t)families.len,
    .pQueueFamilyIndices   = (uint32_t*)families.ptr,
    .preTransform          = capabilities->currentTransform,
    .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,  // TODO: Allow Config
    .presentMode           = mode,
    .clipped               = VK_TRUE,                            // TODO: Allow Config
    .oldSwapchain          = NULL,                               // TODO: Allow Config
  };
}


cvk_Pure VkAttachmentDescription cvk_device_swapchain_attachment_cfg (
  cvk_device_Swapchain const* const swapchain
) {  // clang-format off
  return (VkAttachmentDescription){
    .flags          = (VkAttachmentDescriptionFlags)0,  // TODO: MAY_ALIAS
    .format         = swapchain->cfg.imageFormat,
    .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    .samples        = VK_SAMPLE_COUNT_1_BIT,             // TODO: Configurable for multisampling
    .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,       // TODO: Configurable. Current: Clear to black before drawing
    .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,      // TODO: Configurable. Current: Allow reading the rendered contents from memory
    .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,   // TODO: Configurable for stencil setups
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,  // TODO: Configurable for stencil setups
  };  // clang-format on
}


void cvk_device_swapchain_image_list_destroy (
  cvk_device_swapchain_image_List* const images,
  cvk_device_Logical* const              device_logical,
  cvk_Allocator* const                   allocator
) {
  for (cvk_size id = 0; id < images->len; ++id) {  // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    cvk_semaphore_destroy(&images->ptr[id].finished, device_logical, allocator);
    vkDestroyImageView(device_logical->ct, images->ptr[id].view, allocator->gpu);
    images->ptr[id].ct = NULL;
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  }  // clang-format on
  // Allocated in cvk_device_swapchain_image_list_create
  allocator->cpu.free(&allocator->cpu, (cvk_Slice*)images);
}


cvk_Pure VkImageView cvk_device_swapchain_image_view_create (
  cvk_device_Swapchain const* const                        swapchain,
  cvk_device_swapchain_image_view_create_args const* const arg
) {
  VkImageView result = NULL;  // clang-format off
  cvk_result_check(vkCreateImageView(arg->device_logical->ct, &(VkImageViewCreateInfo){
    .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext            = NULL,
    .flags            = (VkImageViewCreateFlags)0,
    .image            = arg->image,
    .viewType         = VK_IMAGE_VIEW_TYPE_2D,
    .format           = swapchain->cfg.imageFormat,
    .components       = (VkComponentMapping){ .r = 0, .g = 0, .b = 0, .a = 0 },
    .subresourceRange = (VkImageSubresourceRange){
      .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
      .baseMipLevel   = 0,
      .levelCount     = 1,
      .baseArrayLayer = 0,
      .layerCount     = 1,
    }}, arg->allocator->gpu, &result),
    "Failed to retrieve one of the ImageViews used by the Swapchain.");  // clang-format on
  return result;
}


cvk_Pure cvk_device_swapchain_image_List cvk_device_swapchain_image_list_create (
  cvk_device_Swapchain const* const swapchain,
  cvk_device_Logical* const         device_logical,
  cvk_Allocator* const              allocator
) {
  cvk_device_swapchain_image_List result = (cvk_device_swapchain_image_List){ .itemsize = sizeof(cvk_device_swapchain_Image) };
  // clang-format off
  cvk_result_check(vkGetSwapchainImagesKHR(device_logical->ct, swapchain->ct, (uint32_t*)&result.len, NULL),
    "Failed to retrieve the number of Images used by the device's Swapchain.");  // clang-format on
  // Temporary Slice that holds our allocated list. Will transfer to the result at the end
  cvk_Slice data = allocator->cpu.allocZ(&allocator->cpu, result.len, result.itemsize);

  // Get the list of Images
  cvk_Slice images = allocator->cpu.allocZ(&allocator->cpu, data.len, sizeof(VkImage));
  // clang-format off
  cvk_result_check(vkGetSwapchainImagesKHR(device_logical->ct, swapchain->ct, (uint32_t*)&images.len, images.ptr), 
    "Failed to retrieve the list of Images used by the device's Swapchain.");  // clang-format on
  for (cvk_size id = 0; id < images.len; ++id) {
    // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    // Assign the .ct field of our allocated image list (cvk_Slice data)
    ((cvk_device_swapchain_Image*)data.ptr)[id].ct = ((VkImage*)images.ptr)[id];
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
    // clang-format on
  }

  // Get the list of ImageViews
  cvk_Slice views = allocator->cpu.allocZ(&allocator->cpu, data.len, sizeof(VkImageView));
  for (cvk_size id = 0; id < data.len; ++id) {
    // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    ((VkImageView*)views.ptr)[id] = cvk_device_swapchain_image_view_create(swapchain, &(cvk_device_swapchain_image_view_create_args){
      .device_logical = device_logical,
      .image          = ((VkImage*)images.ptr)[id],
      .allocator      = allocator,
    });
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
    // clang-format on
  }

  // Create the Image Semaphores
  for (cvk_size id = 0; id < data.len; ++id) {
    // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    ((cvk_device_swapchain_Image*)data.ptr)[id].finished = cvk_semaphore_create(device_logical, allocator);
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
    // clang-format on
  }

  // Assign to the result
  for (cvk_size id = 0; id < views.len; ++id) {
    // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    // Assign the .ct field of our allocated image list (cvk_Slice data)
    ((cvk_device_swapchain_Image*)data.ptr)[id].view = ((VkImageView*)views.ptr)[id];
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
    // clang-format on
  }

  // Cleanup the temporary lists
  allocator->cpu.free(&allocator->cpu, &images);
  allocator->cpu.free(&allocator->cpu, &views);

  // Assign the allocated image list (cvk_Slice data) to the result.images list
  result.itemsize = sizeof(cvk_device_swapchain_Image);
  result.len      = data.len;
  result.ptr      = (cvk_device_swapchain_Image*)data.ptr;
  return result;
}


cvk_Pure cvk_device_Swapchain cvk_device_swapchain_create (
  cvk_device_swapchain_create_args const* const arg
) {
  cvk_device_Swapchain result = (cvk_device_Swapchain){
    .ct               = NULL,
    .cfg              = cvk_device_swapchain_options_create(  // clang-format off
      /* device       */ arg->device_physical,
      /* surface      */ arg->surface,
      /* img_min      */ cvk_device_swapchain_select_imgMin(&arg->device_physical->swapchainSupport),
      /* format       */ cvk_device_swapchain_select_format(&arg->device_physical->swapchainSupport),
      /* size         */ cvk_device_swapchain_select_size(&arg->device_physical->swapchainSupport, arg->size),
      /* capabilities */ &arg->device_physical->swapchainSupport.capabilities,
      /* mode         */ cvk_device_swapchain_select_mode(&arg->device_physical->swapchainSupport),
      /* allocator    */ arg->allocator  // clang-format on
    ),
    .images = (cvk_device_swapchain_image_List){ 0 },
  };  // clang-format off
  result.attachment_cfg = cvk_device_swapchain_attachment_cfg(&result);
  cvk_result_check(vkCreateSwapchainKHR(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create the device's Swapchain context.");  // clang-format on
  // Get the swapchain images
  result.images = cvk_device_swapchain_image_list_create(&result, arg->device_logical, arg->allocator);
  // Cleanup and Return the result
  return result;
}


void cvk_device_swapchain_destroy (
  cvk_device_Swapchain* const swapchain,
  cvk_device_Logical* const   device_logical,
  cvk_Allocator* const        allocator
) {  // clang-format off
  cvk_device_swapchain_image_list_destroy(&swapchain->images, device_logical, allocator);
  if (swapchain->cfg.pQueueFamilyIndices) allocator->cpu.free(&allocator->cpu, &(cvk_Slice){
    // Allocated in cvk_device_swapchain_options_create
    .len = swapchain->cfg.queueFamilyIndexCount,
     #pragma GCC diagnostic push
     #pragma GCC diagnostic ignored "-Wcast-qual"
    .ptr = (void*)swapchain->cfg.pQueueFamilyIndices,
     #pragma GCC diagnostic pop  // -Wcast-qual
  });  // clang-format on
  swapchain->cfg = (VkSwapchainCreateInfoKHR){ 0 };
  vkDestroySwapchainKHR(device_logical->ct, swapchain->ct, allocator->gpu);
}


//______________________________________
// FIX:
// How to recreate correctly?
// Currently gives validation errors on unreachable semaphore.
// https://vulkan-tutorial.com/en/Drawing_a_triangle/Swap_chain_recreation
//______________________________________
void cvk_device_swapchain_recreate (
  cvk_device_Swapchain* const                     swapchain,
  cvk_device_swapchain_recreate_args const* const arg
) {
  cvk_device_logical_wait(arg->device_logical);

  // Store the old handle & Destroy the old images
  swapchain->cfg.oldSwapchain = swapchain->ct;

  // Destroy the Swapchain's ImageViews
  for (cvk_size id = 0; id < swapchain->images.len; ++id) {  // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    vkDestroyImageView(arg->device_logical->ct, swapchain->images.ptr[id].view, arg->allocator->gpu);
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  }  // clang-format on

  // Create a new Swapchain handle and request its list of images
  // clang-format off
  cvk_result_check(vkCreateSwapchainKHR(arg->device_logical->ct, &swapchain->cfg, arg->allocator->gpu, &swapchain->ct),
    "Failed to re-create the device's Swapchain context."
  );  // clang-format on

  // Re-Create the Swapchain's ImageViews
  for (cvk_size id = 0; id < swapchain->images.len; ++id) {  // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    swapchain->images.ptr[id].view = cvk_device_swapchain_image_view_create(swapchain, &(cvk_device_swapchain_image_view_create_args){
      .device_logical = arg->device_logical,
      .image          = swapchain->images.ptr[id].ct,
      .allocator      = arg->allocator,
    });
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  }  // clang-format on

  // Destroy the old handle
  vkDestroySwapchainKHR(arg->device_logical->ct, swapchain->cfg.oldSwapchain, arg->allocator->gpu);
}


cvk_Pure cvk_size cvk_device_swapchain_nextImageID (
  cvk_device_swapchain_nextImageID_args const* const arg
) {  // clang-format off
  uint32_t result = UINT32_MAX;
  VkResult status = VK_RESULT_MAX_ENUM;
  status = vkAcquireNextImageKHR(
    /* device      */ arg->device_logical->ct,
    /* swapchain   */ arg->swapchain->ct,
    /* timeout     */ UINT64_MAX,
    /* semaphore   */ (arg->semaphore) ? arg->semaphore->ct : VK_NULL_HANDLE,
    /* fence       */ (arg->fence    ) ? arg->fence->ct     : VK_NULL_HANDLE,
    /* pImageIndex */ &result
  );  // clang-format on
  if (!arg->log_disable && status) cvk_print("Failed to request the next ID from the Device.Swapchain's list of images.\n");
  if (arg->status) *arg->status = status;
  return result;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
void cvk_device_swapchain_present (
  cvk_device_Swapchain const* const swapchain,
  cvk_size const                    imageID,
  cvk_device_Queue const* const     queue
) {
  // clang-format off
  cvk_result_check(vkQueuePresentKHR(queue->ct, &(VkPresentInfoKHR){
    .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .pNext              = NULL,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores    = &swapchain->images.ptr[imageID].finished.ct,
    .swapchainCount     = 1,
    .pSwapchains        = &swapchain->ct,
    .pImageIndices      = (uint32_t const*)&imageID,
    .pResults           = NULL, // todo: How to deal with multiple swapchains. This returns their VkResult[]
  }), "Failed when presenting the Graphics Commands with the given Queue");
}                           // clang-format on
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage

