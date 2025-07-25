//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
#include "../pipeline.h"


cvk_Pure cvk_pipeline_Layout cvk_pipeline_layout_create (
  cvk_pipeline_layout_create_args const* const arg
) {  // clang-format off
  cvk_pipeline_Layout result = (cvk_pipeline_Layout){
    .ct                     = NULL,
    .cfg                    = (VkPipelineLayoutCreateInfo){
    .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext                  = NULL,
    .flags                  = (VkPipelineLayoutCreateFlags)0,  // TODO: INDEPENDENT_SETS extension
    .setLayoutCount         = arg->sets_len,
    .pSetLayouts            = arg->sets_ptr,
    .pushConstantRangeCount = 0,                               // TODO: Configurable
    .pPushConstantRanges    = NULL,                            // TODO: Configurable
  }};
  cvk_result_check(vkCreatePipelineLayout(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create the Pipeline's Layout.");
  // clang-format on
  return result;
}


void cvk_pipeline_layout_destroy (
  cvk_pipeline_Layout* const      layout,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  layout->cfg = (VkPipelineLayoutCreateInfo){ 0 };
  vkDestroyPipelineLayout(device_logical->ct, layout->ct, allocator->gpu);
}

