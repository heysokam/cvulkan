//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "./result.h"
#include "./shader.h"


cvk_Pure cvk_Shader cvk_shader_create (
  cvk_shader_create_args const* const arg
) {  // clang-format off
  cvk_Slice code_data = arg->allocator->cpu.duplicate(&arg->allocator->cpu, (cvk_Slice const*)arg->code);
  cvk_Shader result = (cvk_Shader){
    .ct       = NULL,
    .code     = (cvk_SpirV){.ptr = (uint32_t*)code_data.ptr, .len = code_data.len, .itemsize = code_data.itemsize},
  };
  result.cfg  = (VkShaderModuleCreateInfo ){
    .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext    = NULL,
    .flags    = (VkShaderModuleCreateFlags)0, // Vk.Spec -> Reserved for future use
    .codeSize = result.code.len * result.code.itemsize,
    .pCode    = result.code.ptr,
  };
  cvk_result_check(vkCreateShaderModule(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create a Shader Module for SpirV code.");  // clang-format on
  result.stage = (VkPipelineShaderStageCreateInfo){
    .sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .pNext               = NULL,
    .flags               = (VkPipelineShaderStageCreateFlags)0,  // TODO: Configure:  ALLOW_VARYING_SUBGROUP_SIZE, REQUIRE_FULL_SUBGROUPS
    .module              = result.ct,
    .pName               = (arg->entryFn_name) ? arg->entryFn_name : "main",
    .stage               = (VkShaderStageFlagBits)arg->stage,
    .pSpecializationInfo = NULL,  // TODO: How to use VkSpecializationInfo ?
  };
  return result;
}


void cvk_shader_destroy (
  cvk_device_Logical const* const device_logical,
  cvk_Shader* const               shader,
  cvk_Allocator* const            allocator
) {
  allocator->cpu.free(&allocator->cpu, (cvk_Slice*)&shader->code);
  shader->stage = (VkPipelineShaderStageCreateInfo){ 0 };
  shader->cfg   = (VkShaderModuleCreateInfo){ 0 };
  vkDestroyShaderModule(device_logical->ct, shader->ct, allocator->gpu);
}

