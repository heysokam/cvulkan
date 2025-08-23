//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Graphics Pipeline: Implementation
//__________________________________________________|
#include "../result.h"
#include "../pipeline.h"


cvk_Pure cvk_pipeline_Graphics cvk_pipeline_graphics_create (
  cvk_pipeline_graphics_create_args const* const arg
) {
  cvk_pipeline_Graphics result = (cvk_pipeline_Graphics){
    .ct                 = NULL,
    .layout             = cvk_pipeline_layout_create(arg->layout ? arg->layout : &(cvk_pipeline_layout_create_args){
      .device_logical   = arg->device_logical,
      .allocator        = arg->allocator,
    }),
  };
  result.cfg = (VkGraphicsPipelineCreateInfo){
    .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .pNext               = NULL,
    .flags               = arg->flags,
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
    .renderPass          = (arg->renderpass) ? arg->renderpass->ct : NULL,
    .subpass             = arg->subpass,
    .basePipelineHandle  = NULL,
    .basePipelineIndex   = -1,
  };
  if (arg->derivative) {  // Derivative Pipeline creation
    result.cfg.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    result.cfg.basePipelineHandle = arg->derivative->handle;
    result.cfg.basePipelineIndex  = arg->derivative->index;
  }
  cvk_result_check(/* clang-format off */vkCreateGraphicsPipelines(
    /* device          */ arg->device_logical->ct,
    /* pipelineCache   */ VK_NULL_HANDLE,
    /* createInfoCount */ 1,
    /* pCreateInfos    */ &result.cfg,
    /* pAllocator      */ arg->allocator->gpu,
    /* pPipelines      */ &result.ct
    ), "Failed to create a Graphics Pipeline");  // clang-format on
  return result;
}  //:: cvk_pipeline_graphics_create


void cvk_pipeline_graphics_destroy (
  cvk_pipeline_Graphics* const    pipeline,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  cvk_pipeline_layout_destroy(&pipeline->layout, device_logical, allocator);
  pipeline->cfg = (VkGraphicsPipelineCreateInfo){ 0 };
  if (pipeline->ct) vkDestroyPipeline(device_logical->ct, pipeline->ct, allocator->gpu);
}  //:: cvk_pipeline_graphics_destroy


inline void cvk_pipeline_graphics_command_bind (
  cvk_pipeline_Graphics const* const pipeline,
  cvk_command_Buffer const* const    command_buffer
) {
  vkCmdBindPipeline(command_buffer->ct, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->ct);
}


inline void cvk_command_draw (
  cvk_command_Buffer const* const    command_buffer,
  cvk_command_draw_args const* const arg
) {
  vkCmdDraw(
    /* commandBuffer */ command_buffer->ct,
    /* vertexCount   */ (!arg->vertex_len) ? 3 : arg->vertex_len,
    /* instanceCount */ (!arg->instance_len) ? 1 : arg->instance_len,
    /* firstVertex   */ arg->vertex_first,
    /* firstInstance */ arg->instance_first
  );
}


inline void cvk_command_draw_indexed (
  cvk_command_Buffer const* const            command_buffer,
  cvk_command_draw_indexed_args const* const arg
) {
  vkCmdDrawIndexed(
    /* commandBuffer */ command_buffer->ct,
    /* indexCount    */ (!arg->indices_len) ? 3 : arg->indices_len,
    /* instanceCount */ (!arg->instance_len) ? 1 : arg->instance_len,
    /* firstIndex    */ arg->indices_first,
    /* vertexOffset  */ arg->vertex_offset,
    /* firstInstance */ arg->instance_first
  );
}

