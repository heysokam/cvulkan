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


//______________________________________
// @section Pipeline: Layout
//____________________________

typedef struct cvk_pipeline_Layout {
  VkPipelineLayout           ct;
  VkPipelineLayoutCreateInfo cfg;
} cvk_pipeline_Layout;

cvk_Pure cvk_pipeline_Layout cvk_pipeline_layout_create ( // clang-format off
  cvk_device_Logical* const device_logical,
  cvk_Allocator* const      allocator
); // clang-format on

void cvk_pipeline_layout_destroy ( // clang-format off
  cvk_device_Logical* const  device_logical,
  cvk_pipeline_Layout* const layout,
  cvk_Allocator* const       allocator
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

