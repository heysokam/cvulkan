//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../../result.h"
#include "../../sync.h"


cvk_Pure cvk_command_Pool cvk_command_pool_create (
  cvk_command_pool_create_args const* const arg
) {
  cvk_command_Pool result = (cvk_command_Pool) /* clang-format off */{
    .ct                  = NULL,
    .cfg                 = (VkCommandPoolCreateInfo){
    .sType               = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext               = NULL,
    .flags               = (VkCommandPoolCreateFlags)arg->flags,
    .queueFamilyIndex    = arg->queueID,
  }};                                                           // clang-format on
  cvk_result_check(/* clang-format off */vkCreateCommandPool(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create a CommandPool context");  // clang-format on
  return result;
}


void cvk_command_pool_destroy (
  cvk_command_Pool* const         pool,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  pool->cfg = (VkCommandPoolCreateInfo){ 0 };
  vkDestroyCommandPool(device_logical->ct, pool->ct, allocator->gpu);
}

