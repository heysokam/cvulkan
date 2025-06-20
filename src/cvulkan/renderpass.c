//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
#include "./result.h"
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
    // TODO: Configurable Color Attachments
    // Single color buffer attachment for one of the images of the Swapchain.
    .attachmentCount           = 1,
    .pAttachments              = &(VkAttachmentDescription){
      .flags                   = (VkAttachmentDescriptionFlags)0,  // TODO: MAY_ALIAS
      .format                  = arg->swapchain->cfg.imageFormat,
      .initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
      .samples                 = VK_SAMPLE_COUNT_1_BIT,             // TODO: Configurable for multisampling
      .loadOp                  = VK_ATTACHMENT_LOAD_OP_CLEAR,       // TODO: Configurable. Current: Clear to black before drawing
      .storeOp                 = VK_ATTACHMENT_STORE_OP_STORE,      // TODO: Configurable. Current: Allow reading the rendered contents from memory
      .stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE,   // TODO: Configurable for stencil setups
      .stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE,  // TODO: Configurable for stencil setups
    },
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
      .pInputAttachments       = NULL, // Read from a shader
      .preserveAttachmentCount = 0,
      .pPreserveAttachments    = NULL, // Not used by this subpass, but must be preserved
      .pResolveAttachments     = NULL, // For Multisampling
      .pDepthStencilAttachment = NULL, // Depth and Stencil data
    },
    // FIX: Remove hardcoded subpass dependency configuration
    //    : Alternative: Change `waitStages` for `imageAvailable` Semaphore to `VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT`
    .dependencyCount           = 1,
    .pDependencies             = &(VkSubpassDependency){
      .srcSubpass              = VK_SUBPASS_EXTERNAL,
      .dstSubpass              = 0,
      .srcStageMask            = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask            = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .srcAccessMask           = (VkAccessFlags)0,
      .dstAccessMask           = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
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
    .clearValueCount = 1,
    .pClearValues    = &(VkClearValue){ .color = { .float32 = { [0] = 0.222f, [1] = 0.333f, [2] = 0.444f, [3] = 1.0f } } },
  }, VK_SUBPASS_CONTENTS_INLINE);  // clang-format on
}


void cvk_renderpass_command_end (
  cvk_Renderpass const* const     renderpass,
  cvk_command_Buffer const* const command_buffer
) {
  cvk_discard(renderpass);
  vkCmdEndRenderPass(command_buffer->ct);
}

