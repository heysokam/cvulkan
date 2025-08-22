//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
#include "./renderpass.h"


cvk_Pure cvk_Renderpass cvk_renderpass_create (
  cvk_renderpass_create_args const* const arg
) {
  cvk_Renderpass result = (cvk_Renderpass){ 0 };
  // clang-format off
  result.cfg                   = (VkRenderPassCreateInfo){
    .sType                     = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext                     = NULL,
    .flags                     = (VkRenderPassCreateFlags)0,  // TODO: TRANSFORM_BIT_QCOM extension
    .attachmentCount           = (!arg->attachment_cfg_len && arg->attachment_cfg_ptr) ? 1 : (uint32_t )arg->attachment_cfg_len,
    .pAttachments              = arg->attachment_cfg_ptr,
    // TODO: Configurable Subpasses
    // Single subpass for the color buffer attachment of the swapchain images
    .subpassCount              = 1,
    .pSubpasses                = &(VkSubpassDescription){
      .flags                   = (VkSubpassDescriptionFlags)0,  // TODO: Configurable (extensions)
      .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .colorAttachmentCount    = 1,                        // TODO: Configurable. Multi-attachment
      .pColorAttachments       = &(VkAttachmentReference){ // TODO: Configurable. Multi-attachment
        // This attachment's index is directly referenced from the fragment shader
        // with the `layout(location = 0) out vec4 out_Color` directive
        .attachment            = 0,
        .layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      },
      // TODO: Configurable
      .inputAttachmentCount    = 0,
      .pInputAttachments       = NULL, // Read from a shader. Seems mostly for mobile: https://www.saschawillems.de/blog/2018/07/19/vulkan-input-attachments-and-sub-passes/
      .preserveAttachmentCount = 0,
      .pPreserveAttachments    = NULL, // Not used by this subpass, but must be preserved
      .pResolveAttachments     = NULL, // For Multisampling
      .pDepthStencilAttachment = arg->depthStencil, // Depth and Stencil data
    },
    // FIX: Remove hardcoded subpass dependency configuration
    //    : Alternative: Change `waitStages` for `imageAvailable` Semaphore to `VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT`
    .dependencyCount           = 1,
    .pDependencies             = &(VkSubpassDependency){
      .srcSubpass              = VK_SUBPASS_EXTERNAL,
      .dstSubpass              = 0,
      // FIX: Broken. Hardcoded stages/access!
      // TODO: Configurable.
      .srcStageMask            = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .dstStageMask            = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .srcAccessMask           = (VkAccessFlags)0,
      .dstAccessMask           = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      .dependencyFlags         = (VkDependencyFlags)0,
    },
  }; //:: result.cfg
  cvk_result_check(vkCreateRenderPass(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create a Renderpass");  // clang-format on
  return result;
}


void cvk_renderpass_destroy (
  cvk_Renderpass* const           renderpass,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  renderpass->cfg = (VkRenderPassCreateInfo){ 0 };
  vkDestroyRenderPass(device_logical->ct, renderpass->ct, allocator->gpu);
}


void cvk_renderpass_command_begin (
  cvk_Renderpass const* const                    renderpass,
  cvk_renderpass_command_begin_args const* const arg
) {  // clang-format off
  vkCmdBeginRenderPass(arg->command_buffer->ct, &(VkRenderPassBeginInfo){
    .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext           = NULL,
    .renderPass      = renderpass->ct,
    .framebuffer     = arg->framebuffer->ct,
    .renderArea      = { .offset = arg->offset, .extent = arg->extent },
    .clearValueCount = (!arg->clear_len || !arg->clear_ptr) ? 1 : (uint32_t)arg->clear_len,
    .pClearValues    = (arg->clear_ptr) ? arg->clear_ptr : &(VkClearValue){ .color = { .float32 = { [0] = 0.222f, [1] = 0.333f, [2] = 0.444f, [3] = 1.0f } } },
  }, VK_SUBPASS_CONTENTS_INLINE);  // clang-format on
}


inline void cvk_renderpass_command_end (
  cvk_Renderpass const* const     renderpass,
  cvk_command_Buffer const* const command_buffer
) {
  cvk_discard(renderpass);
  vkCmdEndRenderPass(command_buffer->ct);
}

