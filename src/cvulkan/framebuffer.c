//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "./result.h"
#include "./framebuffer.h"


cvk_Pure cvk_Framebuffer cvk_framebuffer_create (
  cvk_framebuffer_create_args const* const arg
) {  // clang-format off
  cvk_Framebuffer result = (cvk_Framebuffer){
    .ct                  = NULL,
    .cfg                 = (VkFramebufferCreateInfo){
      .sType             = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
      .pNext             = NULL,
      .flags             = arg->flags,
      .renderPass        = arg->renderpass->ct,
      .width             = arg->size->width,
      .height            = arg->size->height,
      .attachmentCount   = (uint32_t)arg->attachments_len,
      .pAttachments      = arg->attachments_ptr,
      .layers            = 1, // TODO: Does this need configurability ??
    },
  };
  cvk_result_check(vkCreateFramebuffer(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create a Framebuffer context");  // clang-format on
  return result;
}


void cvk_framebuffer_destroy (
  cvk_Framebuffer* const          framebuffer,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  framebuffer->cfg = (VkFramebufferCreateInfo){ 0 };
  vkDestroyFramebuffer(device_logical->ct, framebuffer->ct, allocator->gpu);
}


cvk_Pure cvk_framebuffer_List cvk_device_swapchain_framebuffers_create (
  cvk_device_swapchain_framebuffers_create_args const* const arg
) {
  cvk_framebuffer_List result = (cvk_framebuffer_List){
    .len      = arg->swapchain->images.len,
    .itemsize = sizeof(cvk_Framebuffer),
    .ptr      = NULL,
  };
  cvk_Slice data = arg->allocator->cpu.allocZ(&arg->allocator->cpu, result.len, result.itemsize);
  result.ptr     = (cvk_Framebuffer*)data.ptr;
  for (cvk_size id = 0; id < data.len; ++id) {  // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    result.ptr[id] = cvk_framebuffer_create(&(cvk_framebuffer_create_args){
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
      .device_logical  = arg->device_logical,
      .allocator       = arg->allocator,
      .renderpass      = arg->renderpass,
      .size            = &arg->swapchain->cfg.imageExtent,
      .attachments_len = 1,
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
      .attachments_ptr = &arg->swapchain->images.ptr[id].view,
      #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
    });  // clang-format on
  }
  return result;
}


void cvk_framebuffer_list_destroy (
  cvk_framebuffer_List* const     framebuffers,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  for (cvk_size id = 0; id < framebuffers->len; ++id) cvk_framebuffer_destroy(&framebuffers->ptr[id], device_logical, allocator);
  #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  // clang-format on
  allocator->cpu.free(&allocator->cpu, (cvk_Slice*)framebuffers);
}

