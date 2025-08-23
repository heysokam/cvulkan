//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
#include "../math.h"
#include "../data.h"


cvk_Pure cvk_image_Data cvk_image_data_create (
  cvk_image_data_create_args const* const arg
) {
  cvk_image_Data result = (cvk_image_Data) /* clang-format off */ {
    .ct                      = NULL,
    .cfg                     = (VkImageCreateInfo){
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
      .queueFamilyIndexCount = (arg->queueFamilies_ptr && !arg->queueFamilies_len) ? 1 : (uint32_t)arg->queueFamilies_len,
      .pQueueFamilyIndices   = arg->queueFamilies_ptr,
      .initialLayout         = arg->layout,
  }};                                                            // clang-format on
  cvk_result_check(/* clang-format off */vkCreateImage(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create an Image.Data context.");  // clang-format on
  vkGetImageMemoryRequirements(arg->device_logical->ct, result.ct, &result.memory.requirements);
  result.memory.kind = cvk_memory_properties_type(&result.memory, arg->device_physical, arg->memory_flags);
  return result;
}  //:: cvk_image_data_create


void cvk_image_data_destroy (
  cvk_image_Data* const           image_data,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
) {
  image_data->cfg                 = (VkImageCreateInfo){ 0 };
  image_data->memory.requirements = (VkMemoryRequirements){ 0 };
  image_data->memory.kind         = cvk_Optional_u32_none;
  if (image_data->ct) vkDestroyImage(device_logical->ct, image_data->ct, allocator->gpu);
}  //:: cvk_image_data_destroy


void cvk_image_data_bind (
  cvk_image_Data const* const           image_data,
  cvk_image_data_bind_args const* const arg
) {
  cvk_result_check(/* clang-format off */vkBindImageMemory(arg->device_logical->ct, image_data->ct, arg->memory->ct, arg->offset),
    "Failed to bind a block of Device.Memory to an Image.Data.Memory context.");  // clang-format on
}  //:: cvk_image_data_bind


void cvk_image_data_command_transition (
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
        .aspectMask        = (!arg->aspect) ? VK_IMAGE_ASPECT_COLOR_BIT : arg->aspect,
        .baseMipLevel      = 0,  // TODO: Configurable
        .levelCount        = 1,  // TODO: Configurable
        .baseArrayLayer    = 0,  // TODO: Configurable
        .layerCount        = 1,  // TODO: Configurable
      }, //:: subresourceRange
    } //:: pImageMemoryBarriers
  );  // clang-format on
}  //:: cvk_image_data_command_transition


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
}  //:: cvk_image_data_command_copy_fromBuffer


cvk_Pure cvk_image_View cvk_image_view_create (
  cvk_image_view_create_args const* const arg
) {  // clang-format off
  cvk_image_View result = (cvk_image_View){
    .ct                 = NULL,
    .cfg                = (VkImageViewCreateInfo){
      .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .pNext            = NULL,
      .flags            = (VkImageViewCreateFlags)0, // TODO: Configurable. Only for extensions.
      .image            = arg->image_data->ct,
      .viewType         = VK_IMAGE_VIEW_TYPE_2D,     // TODO: Configurable
      .format           = arg->image_data->cfg.format,
      .components       = (VkComponentMapping){ .r = 0, .g = 0, .b = 0, .a = 0 }, // 0 == Swizzle.Identity
      .subresourceRange = (VkImageSubresourceRange){
        .aspectMask     = (arg->aspect) ? arg->aspect : VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel   = 0,                         // TODO: Configurable
        .levelCount     = 1,                         // TODO: Configurable
        .baseArrayLayer = 0,                         // TODO: Configurable
        .layerCount     = 1,                         // TODO: Configurable
      }, //:: cfg.subresourceRange
    }, //:: cfg
  };
  cvk_result_check(/* clang-format off */vkCreateImageView(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create an Image.View for an Image.Data handle.");  // clang-format on
  return result;
}  //:: cvk_image_view_create


void cvk_image_view_destroy (
  cvk_image_View* const           image_view,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  image_view->cfg = (VkImageViewCreateInfo){ 0 };
  if (image_view->ct) vkDestroyImageView(device_logical->ct, image_view->ct, allocator->gpu);
}  //:: cvk_image_view_destroy


cvk_Pure cvk_image_Sampler cvk_image_sampler_create (
  cvk_image_sampler_create_args const* const arg
) {  // clang-format off
  cvk_image_Sampler result = (cvk_image_Sampler){
    .ct                        = NULL,
    .cfg                       = (VkSamplerCreateInfo){
      .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
      .pNext                   = NULL,
      .flags                   = (VkSamplerCreateFlags)arg->flags,
      .magFilter               = arg->filter_mag,
      .minFilter               = arg->filter_min,
      .mipmapMode              = arg->mip_mode,
      .addressModeU            = arg->address_U,
      .addressModeV            = arg->address_V,
      .addressModeW            = arg->address_W,
      .mipLodBias              = arg->mip_lodBias,
      .anisotropyEnable        = arg->anisotropy_enabled ? VK_TRUE : VK_FALSE,
      .maxAnisotropy           = (arg->anisotropy_enabled && arg->anisotropy_max == 0.0f)
        ? arg->device_physical->properties.limits.maxSamplerAnisotropy
        : cvk_clamp_f32(arg->anisotropy_max, 1.0f, arg->device_physical->properties.limits.maxSamplerAnisotropy),
      .compareEnable           = arg->compare_enabled ? VK_TRUE : VK_FALSE,
      .compareOp               = arg->compare_op,
      .minLod                  = arg->lod_min,
      .maxLod                  = arg->lod_max,
      .borderColor             = arg->border_color,
      .unnormalizedCoordinates = arg->unnormalized ? VK_TRUE : VK_FALSE,
    },
  };
  cvk_result_check(vkCreateSampler(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create an Image.Sampler context.");  // clang-format on
  return result;
}  //:: cvk_image_sampler_create


void cvk_image_sampler_destroy (
  cvk_image_Sampler* const        image_sampler,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  image_sampler->cfg = (VkSamplerCreateInfo){ 0 };
  if (image_sampler->ct) vkDestroySampler(device_logical->ct, image_sampler->ct, allocator->gpu);
}  //:: cvk_image_sampler_destroy

