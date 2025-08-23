//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
#include "../data.h"


cvk_Pure cvk_Buffer cvk_buffer_create (
  cvk_buffer_create_args const* const arg
) {
  cvk_Buffer result = (cvk_Buffer) /* clang-format off */{
    .ct                      = NULL,
    .cfg                     = (VkBufferCreateInfo){
      .sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .pNext                 = NULL,
      .flags                 = arg->flags,
      .size                  = arg->size,
      .usage                 = arg->usage,
      .sharingMode           = arg->sharing,
      .queueFamilyIndexCount = (arg->queueFamilies_ptr && !arg->queueFamilies_len) ? 1 : (uint32_t)arg->queueFamilies_len,
      .pQueueFamilyIndices   = arg->queueFamilies_ptr,
  }};                                                        // clang-format on
  cvk_result_check(/* clang-format off */vkCreateBuffer(arg->device_logical->ct, &result.cfg, arg->allocator->gpu, &result.ct),
    "Failed to create a GPU data Buffer.");  // clang-format on
  vkGetBufferMemoryRequirements(arg->device_logical->ct, result.ct, &result.memory.requirements);
  result.memory.kind = cvk_memory_properties_type(&result.memory, arg->device_physical, arg->memory_flags);
  return result;
}


void cvk_buffer_destroy (
  cvk_Buffer* const               buffer,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
) {
  buffer->cfg                 = (VkBufferCreateInfo){ 0 };
  buffer->memory.requirements = (VkMemoryRequirements){ 0 };
  buffer->memory.kind         = cvk_Optional_u32_none;
  if (buffer->ct) vkDestroyBuffer(device_logical->ct, buffer->ct, allocator->gpu);
}


inline void cvk_buffer_command_copy (
  cvk_Buffer const* const           A,
  cvk_Buffer const* const           B,
  cvk_buffer_copy_args const* const arg
) {  // clang-format off
  vkCmdCopyBuffer(arg->command_buffer->ct, A->ct, B->ct, 1, &(VkBufferCopy){
    .size = A->cfg.size
  });
  // clang-format on
}


inline void cvk_buffer_bind (
  cvk_Buffer const* const           buffer,
  cvk_buffer_bind_args const* const arg
) {
  cvk_result_check(/* clang-format off */vkBindBufferMemory(arg->device_logical->ct, buffer->ct, arg->memory->ct, arg->offset),
    "Failed to bind a block of Device.Memory to a Buffer.Memory context.");  // clang-format on
}


inline void cvk_buffer_vertex_command_bind (
  cvk_Buffer const* const         buffer,
  cvk_command_Buffer const* const command_buffer
) {
  VkDeviceSize const offsets[] = { 0 };
  vkCmdBindVertexBuffers(command_buffer->ct, 0, 1, &buffer->ct, offsets);  // TODO: Configurable
}


inline void cvk_buffer_index_command_bind (
  cvk_Buffer const* const         buffer,
  cvk_command_Buffer const* const command_buffer
) {
  vkCmdBindIndexBuffer(command_buffer->ct, buffer->ct, 0, VK_INDEX_TYPE_UINT16);  // TODO: Configurable
}

