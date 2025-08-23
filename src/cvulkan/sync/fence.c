//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
#include "../sync.h"


cvk_Pure cvk_Fence cvk_fence_create (
  cvk_device_Logical const* const device_logical,
  cvk_bool const                  signaled,
  cvk_Allocator const* const      allocator
) {
  cvk_Fence result = (cvk_Fence) /* clang-format off */ {
    .ct      = NULL,
    .cfg     = (VkFenceCreateInfo){
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
      .pNext = NULL,
      .flags = (VkFenceCreateFlags)signaled,
    }
  };
  cvk_result_check(vkCreateFence(device_logical->ct, &result.cfg, allocator->gpu, &result.ct),
    "Failed to create a CPU Fence");  // clang-format on
  return result;
}


void cvk_fence_destroy (
  cvk_Fence* const                fence,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  fence->cfg = (VkFenceCreateInfo){ 0 };
  if (fence->ct) vkDestroyFence(device_logical->ct, fence->ct, allocator->gpu);
}


inline void cvk_fence_wait (
  cvk_Fence const* const          fence,
  cvk_device_Logical const* const device_logical
) {
  cvk_result_check(/* clang-format off */vkWaitForFences(device_logical->ct, 1, &fence->ct, cvk_true, UINT64_MAX),
    "Something went wrong when waiting for a CPU Fence.");  // clang-format on
}


inline void cvk_fence_reset (
  cvk_Fence const* const          fence,
  cvk_device_Logical const* const device_logical
) {
  cvk_result_check(/* clang-format off */vkResetFences(device_logical->ct, 1, &fence->ct),
    "Something went wrong when resetting a CPU Fence");  // clang-format on
}

