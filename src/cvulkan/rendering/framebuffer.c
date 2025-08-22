//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
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
      .attachmentCount   = (!arg->attachments_len && arg->attachments_ptr) ? 1 : (uint32_t)arg->attachments_len,
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


void cvk_framebuffer_list_destroy (
  cvk_framebuffer_List* const     framebuffers,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  for (cvk_size id = 0; id < framebuffers->len; ++id) cvk_framebuffer_destroy(&framebuffers->ptr[id], device_logical, allocator);
  allocator->cpu.free(/* clang-format off */&allocator->cpu, &(cvk_Slice){
    .ptr= framebuffers->ptr, .len= framebuffers->len, .itemsize= sizeof(*framebuffers->ptr)
  });  // clang-format on
#pragma GCC diagnostic pop                                                                                                // -Wunsafe-buffer-usage
}

