//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
#include "../data.h"


cvk_Pure cvk_Memory cvk_memory_create (
  cvk_memory_create_args const* const arg
) {  // clang-format off
  cvk_Memory result = (cvk_Memory){
    .ct                = NULL,
    .data              = arg->data,
    .persistent        = arg->persistent,
    .cfg               = (VkMemoryAllocateInfo){
      .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .pNext           = NULL,
      .memoryTypeIndex = arg->kind,
      .allocationSize  = arg->size_alloc,
    },
  };
  cvk_result_check(vkAllocateMemory(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to allocate a block of GPU memory.");
  if (arg->data) {
    cvk_result_check(vkMapMemory(
      /* device */ arg->device_logical->ct,
      /* memory */ result.ct,
      /* offset */ arg->offset,
      /* size   */ arg->size_data,
      /* flags  */ (VkMemoryMapFlags)0,
      /* ppData */ &result.data
    ), "Failed to map a GPU Memory block.");  // clang-format on
    arg->allocator->cpu.copy(
      /* A   */ &arg->allocator->cpu,
      /* src */ &(cvk_Slice){ arg->size_data, 1, arg->data },
      /* trg */ &(cvk_Slice){ arg->size_data, 1, result.data }
    );
    if (!result.persistent) vkUnmapMemory(arg->device_logical->ct, result.ct);
  }
  return result;
}


void cvk_memory_destroy (
  cvk_Memory* const               memory,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
) {
  memory->cfg = (VkMemoryAllocateInfo){ 0 };
  if (memory->persistent) vkUnmapMemory(device_logical->ct, memory->ct);
  vkFreeMemory(device_logical->ct, memory->ct, allocator->gpu);
}

