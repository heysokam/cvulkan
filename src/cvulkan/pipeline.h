//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
//! @fileoverview Vulkan Pipeline: Types & Tools
//________________________________________________|
#ifndef H_cvulkan_pipeline
#define H_cvulkan_pipeline
#include "./base.h"
#include "./memory.h"
#include "./device.h"
#include "./renderpass.h"


//______________________________________
// @section Pipeline: State
//____________________________

typedef struct cvk_pipeline_shaderStage_List {
  cvk_size                               len;
  cvk_Readonly cvk_size                  itemsize;
  VkPipelineShaderStageCreateInfo* const ptr;
} cvk_pipeline_shaderStage_List;


//______________________________________
// @section Pipeline: Layout
//____________________________

typedef struct cvk_pipeline_Layout {
  VkPipelineLayout           ct;
  VkPipelineLayoutCreateInfo cfg;
} cvk_pipeline_Layout;

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

typedef struct cvk_pipeline_Graphics {
  VkPipeline                   ct;
  VkGraphicsPipelineCreateInfo cfg;
  cvk_pipeline_Layout          layout;
  cvk_Renderpass               renderpass;
} cvk_pipeline_Graphics;

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



//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_pipeline
#endif
#ifdef cvk_Implementation_pipeline
#include "./pipeline/layout.c"
#include "./pipeline/graphics.c"
// #include "./pipeline/compute.c"
#endif


#endif  // H_cvulkan_pipeline

