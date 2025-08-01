//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
#include "../data.h"


cvk_Pure cvk_image_Data cvk_image_data_create (
  cvk_image_data_create_args const* const arg
) {  // clang-format off
  cvk_image_Data result = (cvk_image_Data){
    .ct  = NULL,
    .cfg = (VkImageCreateInfo){
      .sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
      .pNext                 = NULL,
      .flags                 = (VkImageCreateFlags)arg->flags,
      .imageType             = arg->dimensions,
      .format                = arg->format,
      .extent                = (VkExtent3D){
        .width               = (!arg->width)  ? 1 : arg->width,
        .height              = (!arg->height) ? 1 : arg->height,
        .depth               = (!arg->depth)  ? 1 : arg->depth
      },
      .mipLevels             = (!arg->mip_len)    ? 1 : arg->mip_len,
      .arrayLayers           = (!arg->layers_len) ? 1 : arg->layers_len,
      .samples               = (!arg->samples)    ? 1 : arg->samples,
      .tiling                = arg->tiling,
      .usage                 = arg->usage,
      .sharingMode           = arg->sharing,
      .queueFamilyIndexCount = 0,    // TODO: How to use?
      .pQueueFamilyIndices   = NULL, // TODO: How to use?
      .initialLayout         = arg->layout,
    }};
  cvk_result_check(vkCreateImage(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create an Image.Data context.");
  // clang-format on
  vkGetImageMemoryRequirements(arg->device_logical->ct, result.ct, &result.memory.requirements);
  result.memory.kind = cvk_memory_properties_type(&result.memory, arg->device_physical, arg->memory_flags);

  return result;
}


void cvk_image_data_destroy (
  cvk_image_Data* const           image_data,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
) {
  image_data->cfg                 = (VkImageCreateInfo){ 0 };
  image_data->memory.requirements = (VkMemoryRequirements){ 0 };
  image_data->memory.kind         = cvk_Optional_u32_none;
  if (image_data->ct) vkDestroyImage(device_logical->ct, image_data->ct, allocator->gpu);
}


void cvk_image_data_bind (
  cvk_image_Data const* const           image_data,
  cvk_image_data_bind_args const* const arg
) {  // clang-format off
  cvk_result_check(vkBindImageMemory(arg->device_logical->ct, image_data->ct, arg->memory->ct, arg->offset),
    "Failed to bind a block of Device.Memory to an Image.Data.Memory context.");  // clang-format on
}


void cvk_image_data_transition (
  cvk_image_Data* const                       image_data,
  cvk_image_data_transition_args const* const arg
) {
  image_data->cfg.initialLayout = arg->layout_new;
  vkCmdPipelineBarrier(
    /* commandBuffer            */ arg->command_buffer->ct,
    /* srcStageMask             */ arg->stage_src,  // FIX: Setup depending on the transition
    /* dstStageMask             */ arg->stage_trg,  // FIX: Setup depending on the transition
    /* dependencyFlags          */ 0,               // TODO: Configurable (eg: by_region flag)
    /* memoryBarrierCount       */ 0,
    /* pMemoryBarriers          */ NULL,
    /* bufferMemoryBarrierCount */ 0,
    /* pBufferMemoryBarriers    */ NULL,
    /* imageMemoryBarrierCount  */ 1,  // clang-format off
    /* pImageMemoryBarriers     */ &(VkImageMemoryBarrier){
      .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      .pNext               = NULL,
      .srcAccessMask       = arg->access_src,
      .dstAccessMask       = arg->access_trg,
      .oldLayout           = arg->layout_old,
      .newLayout           = arg->layout_new,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image               = image_data->ct,
      .subresourceRange    = (VkImageSubresourceRange){
        .aspectMask        = VK_IMAGE_ASPECT_COLOR_BIT,  // TODO: Configurable
        .baseMipLevel      = 0,                          // TODO: Configurable
        .levelCount        = 1,                          // TODO: Configurable
        .baseArrayLayer    = 0,                          // TODO: Configurable
        .layerCount        = 1,                          // TODO: Configurable
      }, //:: subresourceRange
    } //:: pImageMemoryBarriers
  );  // clang-format on
}

void cvk_image_data_command_copy_fromBuffer (
  cvk_image_Data const* const           image_data,
  cvk_Buffer const* const               buffer,
  cvk_image_data_copy_args const* const arg
) {  // clang-format off
  vkCmdCopyBufferToImage(
    /* commandBuffer    */ arg->command_buffer->ct,
    /* srcBuffer        */ buffer->ct,
    /* dstImage         */ image_data->ct,
    /* dstImageLayout   */ image_data->cfg.initialLayout,
    /* regionCount      */ 1,
    /* pRegions         */ &(VkBufferImageCopy){
      .bufferOffset      = arg->offset_buffer,
      .bufferRowLength   = 0,  // TODO: Configurable padding between bytes
      .bufferImageHeight = 0,  // TODO: Configurable padding between bytes
      .imageSubresource  = (VkImageSubresourceLayers){
        .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT, // TODO: Configurable
        .mipLevel        = 0,                         // TODO: Configurable
        .baseArrayLayer  = 0,                         // TODO: Configurable
        .layerCount      = 1,                         // TODO: Configurable
      }, //:: imageSubresource
      .imageOffset       = arg->offset_image,
      .imageExtent       = image_data->cfg.extent,
    } //:: pRegions
  );  // clang-format on
}


cvk_Pure cvk_image_View cvk_image_view_create (
  cvk_image_view_create_args const* const arg
) {  // clang-format off
  cvk_image_View result = (cvk_image_View){
    .ct                 = NULL,
    .cfg                = (VkImageViewCreateInfo){
      .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .pNext            = NULL,
      .flags            = (VkImageViewCreateFlags)0, // TODO: Configurable
      .image            = arg->image_data->ct,
      .viewType         = VK_IMAGE_VIEW_TYPE_2D,     // TODO: Configurable
      .format           = arg->image_data->cfg.format,
      .components       = (VkComponentMapping){ .r = 0, .g = 0, .b = 0, .a = 0 }, // 0 == Swizzle.Identity
      .subresourceRange = (VkImageSubresourceRange){
        .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT, // TODO: Configurable
        .baseMipLevel   = 0,                         // TODO: Configurable
        .levelCount     = 1,                         // TODO: Configurable
        .baseArrayLayer = 0,                         // TODO: Configurable
        .layerCount     = 1,                         // TODO: Configurable
      }, //:: subresourceRange
    }, //:: cfg
  };
  cvk_result_check(vkCreateImageView(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create an Image.View for an Image.Data handle.");  // clang-format on
  return result;
}


void cvk_image_view_destroy (
  cvk_image_View* const           image_view,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  image_view->cfg = (VkImageViewCreateInfo){ 0 };
  if (image_view->ct) vkDestroyImageView(device_logical->ct, image_view->ct, allocator->gpu);
}

