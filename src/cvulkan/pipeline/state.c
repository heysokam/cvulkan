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


cvk_Pure VkPipelineVertexInputStateCreateInfo cvk_pipeline_state_vertexInput_defaults () {
  return (VkPipelineVertexInputStateCreateInfo){
    .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .pNext                           = NULL,
    .flags                           = /* Vk.Spec -> Reserved for future use */ (VkPipelineVertexInputStateCreateFlags)0,
    .vertexBindingDescriptionCount   = 0,     // Default: No vertex binding descriptions
    .pVertexBindingDescriptions      = NULL,  // Default: No vertex binding descriptions
    .vertexAttributeDescriptionCount = 0,     // Default: No vertex attribute descriptions
    .pVertexAttributeDescriptions    = NULL,  // Default: No vertex attribute descriptions
  };
}


cvk_Pure VkPipelineInputAssemblyStateCreateInfo cvk_pipeline_state_inputAssembly_defaults () {
  return (VkPipelineInputAssemblyStateCreateInfo){
    .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .pNext                  = NULL,
    .flags                  = /* Vk.Spec -> Reserved for future use */ (VkPipelineInputAssemblyStateCreateFlags)0,
    .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,  // Default: Draw using Triangles
    .primitiveRestartEnable = VK_FALSE,                             // Default: Do not enable primitive restart
  };
}


cvk_Pure VkPipelineViewportStateCreateInfo cvk_pipeline_state_viewport_defaults () {
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


cvk_Pure VkPipelineRasterizationStateCreateInfo cvk_pipeline_state_rasterization_defaults () {
  return (VkPipelineRasterizationStateCreateInfo){
    .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .pNext                   = NULL,
    .flags                   = /* Vk.Spec -> Reserved for future use */ (VkPipelineRasterizationStateCreateFlags)0,
    .depthClampEnable        = VK_FALSE,                 // Default: Do not enable depth clamp.  (niche) Requires Device.Feature
    .rasterizerDiscardEnable = VK_FALSE,                 // Default: Do not disable framebuffer output
    .polygonMode             = VK_POLYGON_MODE_FILL,     // Default: Use Fill mode for polygons.  (niche-ish) Requires Device.Feature
    .lineWidth               = 1.0f,                     // Default: Use a line width of 1.0f.   (niche). >1.0 requires Device.Feature for widelines
    .cullMode                = VK_CULL_MODE_BACK_BIT,    // Default: Cull the backface
    .frontFace               = VK_FRONT_FACE_CLOCKWISE,  // Default: Use CW winding order
    .depthBiasEnable         = VK_FALSE,                 // Default: Do not use depth bias.  (niche-ish) Useful for ShadowMapping
    .depthBiasConstantFactor = 0.0f,                     // Default: Do not use depth bias.  (niche-ish) Useful for ShadowMapping
    .depthBiasClamp          = 0.0f,                     // Default: Do not use depth bias.  (niche-ish) Useful for ShadowMapping
    .depthBiasSlopeFactor    = 0.0f,                     // Default: Do not use depth bias.  (niche-ish) Useful for ShadowMapping
  };
}


cvk_Pure VkPipelineMultisampleStateCreateInfo cvk_pipeline_state_multisample_defaults () {
  return (VkPipelineMultisampleStateCreateInfo){
    .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .pNext                 = NULL,
    .flags                 = /* Vk.Spec -> Reserved for future use */ (VkPipelineMultisampleStateCreateFlags)0,
    .sampleShadingEnable   = VK_FALSE,               // Default: Do not enable multisampling features
    .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,  // Default: Do not enable multisampling features
    .minSampleShading      = 1.0f,                   // Default: Do not enable multisampling features
    .pSampleMask           = NULL,                   // Default: Do not enable multisampling features
    .alphaToCoverageEnable = VK_FALSE,               // Default: Do not enable multisampling features
    .alphaToOneEnable      = VK_FALSE,               // Default: Do not enable multisampling features
  };
}

cvk_Pure VkPipelineColorBlendAttachmentState cvk_pipeline_state_colorBlend_attachment_defaults () {
  return (VkPipelineColorBlendAttachmentState){
    .blendEnable         = VK_TRUE,                              // Default: Enable basic blending
    .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,            // Default: Enable basic blending
    .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,  // Default: Enable basic blending
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
  return (VkPipelineColorBlendStateCreateInfo){
    .sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .pNext             = NULL,
    .flags             = (VkPipelineColorBlendStateCreateFlags)0,  // Default: Do not setup RASTERIZATION_ORDER_ATTACHMENT_ACCESS (extension)
    .logicOpEnable     = VK_FALSE,                                 // Default: Do not enable Bitwise combination blending method
    .logicOp           = VK_LOGIC_OP_COPY,                         // Default: Copy operator for Bitwise combination blending
    .attachmentCount   = (arg->attachments_ptr && !arg->attachments_len) ? 1 : (uint32_t)arg->attachments_len,
    .pAttachments      = arg->attachments_ptr,
    .blendConstants[0] = 0.0f,  // Default: Do not enable blending constants
    .blendConstants[1] = 0.0f,  // Default: Do not enable blending constants
    .blendConstants[2] = 0.0f,  // Default: Do not enable blending constants
    .blendConstants[3] = 0.0f,  // Default: Do not enable blending constants
  };
}


inline void cvk_viewport_command_set (
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

