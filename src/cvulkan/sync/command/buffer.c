//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
#include "../../result.h"
#include "../../sync.h"


cvk_Pure cvk_command_Buffer cvk_command_buffer_allocate (
  cvk_command_buffer_allocate_args const* const arg
) {  // clang-format off
  cvk_command_Buffer result = (cvk_command_Buffer){
    .ct                   = NULL,
    .cfg                  = (VkCommandBufferAllocateInfo){
      .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .pNext              = NULL,
      .commandPool        = arg->pool->ct,
      .level              = (VkCommandBufferLevel)arg->level,
      .commandBufferCount = (arg->count) ? (uint32_t)arg->count : 1,
    },
  };
  cvk_result_check(vkAllocateCommandBuffers(arg->device_logical->ct, &result.cfg, &result.ct),
    "Failed to create a CommandBuffer context");  // clang-format on
  return result;
}

