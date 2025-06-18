//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
//! @fileoverview Graphics Pipeline: Implementation
//__________________________________________________|
#include "../result.h"
#include "../pipeline.h"

cvk_Pure cvk_pipeline_Graphics cvk_pipeline_graphics_create (
  cvk_pipeline_graphics_create_args const* const arg
) {
  cvk_pipeline_Graphics result = (cvk_pipeline_Graphics){
    .ct         = NULL,
    .layout     = cvk_pipeline_layout_create(arg->device_logical, arg->allocator),
    .renderpass = (arg->renderpass) ? *arg->renderpass : (cvk_Renderpass){ .ct = NULL },
  };
  result.cfg = (VkGraphicsPipelineCreateInfo){
    .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .pNext               = NULL,
    .flags               = arg->flags,  // TODO: DERIVATIVE
    .stageCount          = (uint32_t)arg->stages->len,
    .pStages             = arg->stages->ptr,
    .pVertexInputState   = arg->state_vertexInput,
    .pInputAssemblyState = arg->state_inputAssembly,
    .pViewportState      = arg->state_viewport,
    .pRasterizationState = arg->state_rasterization,
    .pMultisampleState   = arg->state_multisample,
    .pDepthStencilState  = arg->state_depthStencil,
    .pColorBlendState    = arg->state_colorBlend,
    .pDynamicState       = arg->state_dynamic,
    .pTessellationState  = arg->state_tessellation,
    .layout              = result.layout.ct,
    .renderPass          = result.renderpass.ct,
    .subpass             = 0,     // TODO: Does this need to be configurable ??
    .basePipelineHandle  = NULL,  // TODO: Configurable. Derivative Pipeline creation
    .basePipelineIndex   = -1,    // TODO: Configurable. Derivative Pipeline creation
  };  // clang-format off
  cvk_result_check(vkCreateGraphicsPipelines(
    /* device          */ arg->device_logical->ct,
    /* pipelineCache   */ VK_NULL_HANDLE,
    /* createInfoCount */ 1,
    /* pCreateInfos    */ &result.cfg,
    /* pAllocator      */ arg->allocator->gpu,
    /* pPipelines      */ &result.ct
    ), "Failed to create a Graphics Pipeline");
  // clang-format on
  return result;
}


void cvk_pipeline_graphics_destroy (
  cvk_pipeline_Graphics* const    pipeline,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  cvk_pipeline_layout_destroy(&pipeline->layout, device_logical, allocator);
  pipeline->cfg = (VkGraphicsPipelineCreateInfo){ 0 };
  vkDestroyPipeline(device_logical->ct, pipeline->ct, allocator->gpu);
}


