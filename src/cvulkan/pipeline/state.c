//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../pipeline.h"


cvk_Pure VkPipelineDynamicStateCreateInfo cvk_pipeline_state_dynamic_setup (
  cvk_size const       states_len,
  VkDynamicState const states_ptr[]
) {
  return (VkPipelineDynamicStateCreateInfo){
    .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    .pNext             = NULL,
    .flags             = (VkPipelineDynamicStateCreateFlags)0,  // Vk.Spec -> Reserved for future use
    .dynamicStateCount = (uint32_t)states_len,
    .pDynamicStates    = states_ptr,
  };
}


cvk_Pure VkPipelineVertexInputStateCreateInfo cvk_pipeline_state_vertexInput_setup () {
  return (VkPipelineVertexInputStateCreateInfo){
    .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .pNext                           = NULL,
    .flags                           = /* Vk.Spec -> Reserved for future use */ (VkPipelineVertexInputStateCreateFlags)0,
    .vertexBindingDescriptionCount   = 0,     // TODO: Configurable
    .pVertexBindingDescriptions      = NULL,  // TODO: Configurable
    .vertexAttributeDescriptionCount = 0,     // TODO: Configurable
    .pVertexAttributeDescriptions    = NULL,  // TODO: Configurable
  };
}


cvk_Pure VkPipelineInputAssemblyStateCreateInfo cvk_pipeline_state_inputAssembly_setup () {
  return (VkPipelineInputAssemblyStateCreateInfo){
    .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .pNext                  = NULL,
    .flags                  = /* Vk.Spec -> Reserved for future use */ (VkPipelineInputAssemblyStateCreateFlags)0,
    .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,  // TODO: Configurable
    .primitiveRestartEnable = VK_FALSE,                             // TODO: Configurable
  };
}


cvk_Pure VkPipelineViewportStateCreateInfo cvk_pipeline_state_viewport_setup () {
  return (VkPipelineViewportStateCreateInfo){
    .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .pNext         = NULL,
    .flags         = /* Vk.Spec -> Reserved for future use */ (VkPipelineViewportStateCreateFlags)0,
    .viewportCount = 1,     // TODO: Configurable.   Current: Viewport is Dynamic State
    .pViewports    = NULL,  // TODO: NULL because Viewport is Dynamic State
    .scissorCount  = 1,     // TODO: Configurable.   Current: Viewport.Scissor is Dynamic State
    .pScissors     = NULL,  // TODO: NULL because Viewport.Scissor is Dynamic State
  };
}


cvk_Pure VkPipelineRasterizationStateCreateInfo cvk_pipeline_state_rasterization_setup () {
  return (VkPipelineRasterizationStateCreateInfo){
    .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .pNext                   = NULL,
    .flags                   = /* Vk.Spec -> Reserved for future use */ (VkPipelineRasterizationStateCreateFlags)0,
    .depthClampEnable        = VK_FALSE,                 // TODO: Configurable (niche). Requires Device.Feature
    .rasterizerDiscardEnable = VK_FALSE,                 // TODO: Does this need to be configurable at all ?? Basically disables framebuffer output
    .polygonMode             = VK_POLYGON_MODE_FILL,     // TODO: Configurable (niche-ish). Requires Device.Feature
    .lineWidth               = 1.0f,                     // TODO: Configurable (niche). >1.0 requires Device.Feature for widelines
    .cullMode                = VK_CULL_MODE_BACK_BIT,    // TODO: Configurable
    .frontFace               = VK_FRONT_FACE_CLOCKWISE,  // TODO: Configurable
    .depthBiasEnable         = VK_FALSE,                 // TODO: Configurable (niche). Useful for ShadowMapping
    .depthBiasConstantFactor = 0.0f,                     // TODO: Configurable (niche). Useful for ShadowMapping
    .depthBiasClamp          = 0.0f,                     // TODO: Configurable (niche). Useful for ShadowMapping
    .depthBiasSlopeFactor    = 0.0f,                     // TODO: Configurable (niche). Useful for ShadowMapping
  };
}


cvk_Pure VkPipelineMultisampleStateCreateInfo cvk_pipeline_state_multisample_setup () {
  return (VkPipelineMultisampleStateCreateInfo){
    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = /* Vk.Spec -> Reserved for future use */ (VkPipelineMultisampleStateCreateFlags)0,
    // TODO: Configurable
    //     : Set to disabled for now.
    .sampleShadingEnable   = VK_FALSE,
    .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
    .minSampleShading      = 1.0f,
    .pSampleMask           = NULL,
    .alphaToCoverageEnable = VK_FALSE,
    .alphaToOneEnable      = VK_FALSE,
  };
}

cvk_Pure VkPipelineColorBlendAttachmentState cvk_pipeline_state_colorBlend_attachment_setup () {
  return (VkPipelineColorBlendAttachmentState){
    .blendEnable         = VK_TRUE,                              // TODO: Configurable
    .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,            // TODO: Configurable
    .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,  // TODO: Configurable
    .colorBlendOp        = VK_BLEND_OP_ADD,
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
    .alphaBlendOp        = VK_BLEND_OP_ADD,
    .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
  };
}

cvk_Pure VkPipelineColorBlendStateCreateInfo cvk_pipeline_state_colorBlend_setup (
  cvk_pipeline_state_colorBlend_setup_args const* const arg
) {
  // clang-format off
  return (VkPipelineColorBlendStateCreateInfo){
    .sType                   = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .pNext                   = NULL,
    .flags                   = (VkPipelineColorBlendStateCreateFlags)0,  // TODO: RASTERIZATION_ORDER_ATTACHMENT_ACCESS extension
      // TODO: Configurable. true for Bitwise combination blending method
    .logicOpEnable         = VK_FALSE,
    .logicOp               = VK_LOGIC_OP_COPY,  // TODO: Configurable. Bitwise Operator for blending
    .attachmentCount       = (arg->attachments_ptr && !arg->attachments_len) ? 1 : (uint32_t)arg->attachments_len,
    .pAttachments          = arg->attachments_ptr,
    // TODO: Do these need to be configurable ?
    .blendConstants[0]       = 0.0f,
    .blendConstants[1]       = 0.0f,
    .blendConstants[2]       = 0.0f,
    .blendConstants[3]       = 0.0f,
  };  // clang-format on
}


void cvk_viewport_command_set (
  VkViewport const* const         viewport,
  cvk_command_Buffer const* const command_buffer
) {
  vkCmdSetViewport(command_buffer->ct, 0, 1, viewport);
}


void cvk_scissor_command_set (
  VkRect2D const* const           scissor,
  cvk_command_Buffer const* const command_buffer
) {
  vkCmdSetScissor(command_buffer->ct, 0, 1, scissor);
}

