//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
#include "../sync.h"


cvk_Pure cvk_Semaphore cvk_semaphore_create (
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
) {
  cvk_Semaphore result = (cvk_Semaphore) /* clang-format off */{
    .ct      = NULL,
    .cfg     = (VkSemaphoreCreateInfo){
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
      .pNext = NULL,
      .flags = /* Vk.Spec -> Reserved for future use */ (VkSemaphoreCreateFlags)0,
    },
  };
  cvk_result_check(vkCreateSemaphore(device_logical->ct, &result.cfg, allocator->gpu, &result.ct),
    "Failed to create a GPU Semaphore");  // clang-format on
  return result;
}


void cvk_semaphore_destroy (
  cvk_Semaphore* const            semaphore,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  semaphore->cfg = (VkSemaphoreCreateInfo){ 0 };
  vkDestroySemaphore(device_logical->ct, semaphore->ct, allocator->gpu);
}

