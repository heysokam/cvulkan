//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Vulkan Pipeline: Types & Tools
//________________________________________________|
#ifndef H_cvulkan_pipeline
#define H_cvulkan_pipeline
#include "./base.h"
#include "./types.h"


//______________________________________
// @section Pipeline: State
//____________________________

cvk_Pure VkPipelineDynamicStateCreateInfo cvk_pipeline_state_dynamic_setup ( // clang-format off
  cvk_size const       states_len,
  VkDynamicState const states_ptr[]
); // clang-format on
cvk_Pure VkPipelineVertexInputStateCreateInfo   cvk_pipeline_state_vertexInput_setup ();
cvk_Pure VkPipelineInputAssemblyStateCreateInfo cvk_pipeline_state_inputAssembly_setup ();
cvk_Pure VkPipelineViewportStateCreateInfo      cvk_pipeline_state_viewport_setup ();
cvk_Pure VkPipelineRasterizationStateCreateInfo cvk_pipeline_state_rasterization_setup ();
cvk_Pure VkPipelineMultisampleStateCreateInfo   cvk_pipeline_state_multisample_setup ();

cvk_Pure VkPipelineColorBlendAttachmentState cvk_pipeline_state_colorBlend_attachment_setup ();

typedef struct cvk_pipeline_state_colorBlend_setup_args {
  cvk_Nullable cvk_size const                                   attachments_len;
  cvk_Nullable VkPipelineColorBlendAttachmentState const* const attachments_ptr;
} cvk_pipeline_state_colorBlend_setup_args;
cvk_Pure VkPipelineColorBlendStateCreateInfo cvk_pipeline_state_colorBlend_setup (  // clang-format off
  cvk_pipeline_state_colorBlend_setup_args const* const arg
);  // clang-format on

// TODO: There is probably a better header/location for this
void cvk_viewport_command_set ( // clang-format off
  VkViewport const* const         viewport,
  cvk_command_Buffer const* const command_buffer
); // clang-format on

// TODO: There is probably a better header/location for this
void cvk_scissor_command_set ( // clang-format off
  VkRect2D const* const           scissor,
  cvk_command_Buffer const* const command_buffer
); // clang-format on


//______________________________________
// @section Pipeline: Layout
//____________________________
cvk_Pure cvk_pipeline_Layout cvk_pipeline_layout_create ( // clang-format off
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on

void cvk_pipeline_layout_destroy ( // clang-format off
  cvk_pipeline_Layout* const      layout,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on


//______________________________________
// @section Pipeline: Graphics Context
//____________________________

typedef struct cvk_pipeline_graphics_create_args {
  cvk_device_Logical const* const                                 device_logical;
  VkPipelineCreateFlags                                           flags;
  char                                                            priv_pad[4];
  cvk_pipeline_shaderStage_List* const                            stages;
  VkPipelineVertexInputStateCreateInfo const* const               state_vertexInput;
  VkPipelineInputAssemblyStateCreateInfo const* const             state_inputAssembly;
  cvk_Nullable VkPipelineTessellationStateCreateInfo const* const state_tessellation;
  VkPipelineViewportStateCreateInfo const* const                  state_viewport;
  VkPipelineRasterizationStateCreateInfo const* const             state_rasterization;
  VkPipelineMultisampleStateCreateInfo const* const               state_multisample;
  cvk_Nullable VkPipelineDepthStencilStateCreateInfo const* const state_depthStencil;
  VkPipelineColorBlendStateCreateInfo const* const                state_colorBlend;
  VkPipelineDynamicStateCreateInfo const* const                   state_dynamic;
  cvk_Nullable cvk_Renderpass const* const                        renderpass;
  cvk_Allocator* const                                            allocator;
} cvk_pipeline_graphics_create_args;

cvk_Pure cvk_pipeline_Graphics cvk_pipeline_graphics_create (  // clang-format off
  cvk_pipeline_graphics_create_args const* const arg
);  // clang-format on

void cvk_pipeline_graphics_destroy ( // clang-format off
  cvk_pipeline_Graphics* const    pipeline,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on

void cvk_pipeline_graphics_command_bind ( // clang-format off
  cvk_pipeline_Graphics const* const pipeline,
  cvk_command_Buffer const* const    command_buffer
); // clang-format on

typedef struct cvk_command_draw_args {
  cvk_Nullable uint32_t vertex_len;
  uint32_t              vertex_first;
  cvk_Nullable uint32_t instance_len;
  uint32_t              instance_first;
} cvk_command_draw_args;
void cvk_command_draw (  // clang-format off
  cvk_command_Buffer const* const    command_buffer,
  cvk_command_draw_args const* const arg
);  // clang-format on


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_pipeline
#endif
#ifdef cvk_Implementation_pipeline
#include "./pipeline/state.c"
#include "./pipeline/layout.c"
#include "./pipeline/graphics.c"
// #include "./pipeline/compute.c"
#endif


#endif  // H_cvulkan_pipeline

