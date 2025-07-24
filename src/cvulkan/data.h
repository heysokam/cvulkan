//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Vulkan Data: Types & Tools
//_____________________________________________|
#ifndef H_cvk_data
#define H_cvk_data
#include "./base.h"
#include "./types.h"


//______________________________________
// @section Memory
//____________________________

/// @description
/// Alias to `VkMemoryPropertyFlagBits` for naming consistency.
/// Fully compatible with the original type.
typedef enum cvk_memory_Kind {
  cvk_memory_DeviceLocal  = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
  cvk_memory_HostVisible  = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
  cvk_memory_HostCoherent = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
  cvk_memory_HostCached   = VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
  cvk_memory_LazyAlloc    = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT,
  cvk_memory_Protected    = VK_MEMORY_PROPERTY_PROTECTED_BIT,
  cvk_memory_Kind_Force32 = VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM,
} cvk_memory_Kind;
typedef VkMemoryPropertyFlags cvk_memory_Flags;

typedef struct cvk_memory_create_args {
  cvk_device_Logical const* const   device_logical;
  cvk_Nullable cvk_pointer const    data;  ///< {@link cvk_memory_create} Will automatically map+copy the memory to the GPU when not `NULL`
  uint32_t                          kind;
  char                              priv_pad1[4];
  VkDeviceSize                      size_alloc;
  VkDeviceSize                      size_data;
  cvk_Nullable VkDeviceSize const   offset;
  cvk_Nullable cvk_Allocator* const allocator;  ///< Can only be `NULL` when `.data` is `NULL`
} cvk_memory_create_args;
cvk_Pure cvk_Memory cvk_memory_create (  // clang-format off
  cvk_memory_create_args const* const arg
);                                              // clang-format on

void cvk_memory_destroy ( // clang-format off
  cvk_Memory* const               memory,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
); // clang-format on


//______________________________________
// @section Buffer
//____________________________

typedef struct cvk_buffer_create_args {
  cvk_device_Physical const* const       device_physical;
  cvk_device_Logical const* const        device_logical;
  cvk_Nullable VkBufferCreateFlags const flags;
  char                                   priv_pad1[4];
  VkDeviceSize const                     size;
  VkBufferUsageFlags const               usage;
  cvk_Nullable VkSharingMode const       sharing;
  cvk_memory_Flags const                 memory_flags;
  char                                   priv_pad2[4];
  cvk_Allocator* const                   allocator;
} cvk_buffer_create_args;
cvk_Pure cvk_Buffer cvk_buffer_create (  // clang-format off
  cvk_buffer_create_args const* const arg
);  // clang-format on

void cvk_buffer_destroy ( // clang-format off
  cvk_Buffer* const               buffer,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
); // clang-format on

cvk_Pure cvk_Optional_u32 cvk_buffer_memoryType ( // clang-format off
  cvk_Buffer const* const          buffer,
  cvk_device_Physical const* const device_physical,
  VkMemoryPropertyFlags const      flags
); // clang-format on

typedef struct cvk_buffer_bind_args {
  cvk_device_Logical const* const device_logical;
  cvk_Memory const* const         memory;
  cvk_Nullable VkDeviceSize const offset;
} cvk_buffer_bind_args;
void cvk_buffer_bind (  // clang-format off
  cvk_Buffer const*const            buffer,
  cvk_buffer_bind_args const* const arg
);  // clang-format on

void cvk_buffer_vertex_command_bind ( // clang-format off
  cvk_Buffer const* const         buffer,
  cvk_command_Buffer const* const command_buffer
); // clang-format on

void cvk_buffer_index_command_bind ( // clang-format off
  cvk_Buffer const* const         buffer,
  cvk_command_Buffer const* const command_buffer
); // clang-format on

typedef struct cvk_buffer_copy_args {
  cvk_command_Pool const* const   pool;
  cvk_device_Logical const* const device_logical;
  cvk_device_Queue const* const   device_queue;
} cvk_buffer_copy_args;
void cvk_buffer_copy ( // clang-format off
  cvk_Buffer const* const A,
  cvk_Buffer const* const B,
  cvk_buffer_copy_args const*const arg
); // clang-format on


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_data
#endif
#ifdef cvk_Implementation_data
#include "./data/buffer.c"
#include "./data/memory.c"
#endif


#endif  // H_cvk_data

