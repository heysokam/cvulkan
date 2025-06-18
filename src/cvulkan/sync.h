//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
//! @fileoverview Vulkan Sync Structures: Types & Tools
//______________________________________________________|
#ifndef H_cvulkan_sync
#define H_cvulkan_sync
#include "./base.h"
#include "./types.h"


//______________________________________
// @section Command Pool
//____________________________

typedef struct cvk_command_pool_create_args {
  cvk_device_Logical const* const device_logical;
  cvk_QueueID const               queueID;
  cvk_command_pool_Flags const    flags;
  cvk_Allocator* const            allocator;
} cvk_command_pool_create_args;

cvk_Pure cvk_command_Pool cvk_command_pool_create (  // clang-format off
  cvk_command_pool_create_args const* const arg
);  // clang-format on

void cvk_command_pool_destroy ( // clang-format off
  cvk_command_Pool* const         pool,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on


//______________________________________
// @section Command Buffer
//____________________________

typedef struct cvk_command_buffer_allocate_args {
  cvk_device_Logical const* const             device_logical;
  cvk_command_Pool const* const               pool;
  cvk_Nullable cvk_command_buffer_Level const level;  ///< Primary (0) is the default
  char                                        priv_pad[4];
  cvk_Nullable cvk_size const                 count;  ///< Will default to 1 when omitted.
} cvk_command_buffer_allocate_args;

/// @note
/// Command Buffers are deallocated automatically when their CommandPool is destroyed.
/// The `.cfg` field will not be modified, unless zeroed by the caller.
cvk_Pure cvk_command_Buffer cvk_command_buffer_allocate (  // clang-format off
  cvk_command_buffer_allocate_args const* const arg
);  // clang-format on

void cvk_command_buffer_begin (cvk_command_Buffer const* const command_buffer);
void cvk_command_buffer_end (cvk_command_Buffer const* const command_buffer);


//______________________________________
// @section Semaphore
//____________________________

cvk_Pure cvk_Semaphore cvk_semaphore_create (  // clang-format off
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
);  // clang-format on

void cvk_semaphore_destroy ( // clang-format off
  cvk_Semaphore* const            semaphore,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on


//______________________________________
// @section Fence
//____________________________

cvk_Pure cvk_Fence cvk_fence_create (  // clang-format off
  cvk_device_Logical const* const device_logical,
  cvk_bool const                  signaled,
  cvk_Allocator const* const      allocator
);  // clang-format on

void cvk_fence_destroy ( // clang-format off
  cvk_Fence* const                fence,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on

void cvk_fence_wait ( // clang-format off
  cvk_Fence const* const          fence,
  cvk_device_Logical const* const device_logical
); // clang-format on

void cvk_fence_reset ( // clang-format off
  cvk_Fence const* const          fence,
  cvk_device_Logical const* const device_logical
); // clang-format on


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_sync
#endif
#ifdef cvk_Implementation_sync
#include "./sync/command/pool.c"
#include "./sync/command/buffer.c"
#include "./sync/fence.c"
#include "./sync/semaphore.c"
#endif


#endif  // H_cvulkan_sync

