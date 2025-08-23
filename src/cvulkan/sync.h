//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Vulkan Sync Structures: Tools
//______________________________________________|
#ifndef H_cvulkan_sync
#define H_cvulkan_sync
#include "./base.h"
#include "./types.h"


//______________________________________
// @section Command Pool
//____________________________

/// @description
/// Configuration options for `cvk_command_pool_create`.
typedef struct cvk_command_pool_create_args {
  cvk_device_Logical const* const           device_logical;
  cvk_Allocator* const                      allocator;
  cvk_QueueID const                         queueID;
  cvk_Nullable cvk_command_pool_Flags const flags;
} cvk_command_pool_create_args;

/// @description
/// Creates a valid `Command.Pool` object using the given `arg` configuration options.
///
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_command_pool_destroy` using the same `allocator`.
cvk_Pure cvk_command_Pool cvk_command_pool_create (  // clang-format off
  cvk_command_pool_create_args const* const arg
);  // clang-format on

/// @description
/// Releases any memory and handles created by `cvk_command_pool_create` using the same `allocator`.
void cvk_command_pool_destroy ( // clang-format off
  cvk_command_Pool* const         pool,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on


//______________________________________
// @section Command Buffer
//____________________________

/// @description
/// Configuration options for `cvk_command_buffer_allocate`.
typedef struct cvk_command_buffer_allocate_args {
  cvk_device_Logical const* const             device_logical;
  cvk_command_Pool const* const               command_pool;
  cvk_Nullable cvk_command_buffer_Level const level;  ///< Primary (0) is the default
  char                                        priv_pad[4];
  cvk_Nullable cvk_size const                 count;  ///< Will default to 1 when omitted.
} cvk_command_buffer_allocate_args;

/// @description
/// Creates a valid `Command.Buffer` object using the given `arg` configuration options.
///
/// @note
/// Command Buffers are deallocated automatically by Vulkan when their Command.Pool is destroyed,
/// but the user could safely call `cvk_command_buffer_free` when they are done with this command buffer.
cvk_Pure cvk_command_Buffer cvk_command_buffer_allocate (  // clang-format off
  cvk_command_buffer_allocate_args const* const arg
);  // clang-format on

/// @description
/// Configuration options for `cvk_command_buffer_free`.
typedef struct cvk_command_buffer_free_args {
  cvk_command_Pool const* const   command_pool;
  cvk_device_Logical const* const device_logical;
} cvk_command_buffer_free_args;

/// @description
/// Releases any memory and handles created by `cvk_command_buffer_allocate` using the same `command_pool`.
void cvk_command_buffer_free (  // clang-format off
  cvk_command_Buffer* const                 command_buffer,
  cvk_command_buffer_free_args const* const arg
);  // clang-format on

/// @description
/// Flags that mark what a `Command.Buffer` will be used for.
///
/// @note
/// Thin wrapper for `VkCommandBufferUsageFlagBits` using the cvulkan naming convention.
/// Fully compatible with the original.
typedef enum cvk_command_buffer_Usage {
  cvk_command_buffer_OneTimeSubmit      = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  cvk_command_buffer_RenderpassContinue = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
  cvk_command_buffer_Simultaneous       = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
  cvk_command_buffer_Usage_Force32      = VK_COMMAND_BUFFER_USAGE_FLAG_BITS_MAX_ENUM
} cvk_command_buffer_Usage;

/// @description
/// Configuration options for `cvk_command_buffer_begin`.
typedef struct cvk_command_buffer_begin_args {
  VkCommandBufferUsageFlags const                          flags;
  char                                                     priv_pad[4];
  cvk_Nullable VkCommandBufferInheritanceInfo const* const inheritance;
} cvk_command_buffer_begin_args;

/// @description
/// Orders Vulkan to start recording commands using the given `command_buffer` and `arg` configuration options.
/// The caller is responsible for calling `cvk_command_buffer_end` when the recording process is done.
/// @note Thin inline wrapper for `vkBeginCommandBuffer` using the cvulkan API.
void cvk_command_buffer_begin2 ( // clang-format off
  cvk_command_Buffer const* const            command_buffer,
  cvk_command_buffer_begin_args const* const arg
); // clang-format on

/// @description
/// Orders Vulkan to start recording commands using the given `command_buffer` with the default configuration options.
/// The caller is responsible for calling `cvk_command_buffer_end` when the recording process is done.
/// @note Thin inline wrapper for `vkBeginCommandBuffer` using the cvulkan API.
void cvk_command_buffer_begin (  // clang-format off
  cvk_command_Buffer const* const command_buffer
);  // clang-format on

/// @description
/// Stops the command recording process started by `cvk_command_buffer_begin` using the same `command_buffer`.
/// @note Thin inline wrapper for `vkEndCommandBuffer` using the cvulkan API.
void cvk_command_buffer_end (  // clang-format off
  cvk_command_Buffer const* const command_buffer
);  // clang-format on

/// @description
/// Orders Vulkan to set the state of the given `command_buffer` to its initial state.
/// @note Thin inline wrapper for `vkResetCommandBuffer` using the cvulkan API.
void cvk_command_buffer_reset ( // clang-format off
  cvk_command_Buffer const* const command_buffer,
  cvk_bool const                  releaseResources
); // clang-format on


//______________________________________
// @section Semaphore
//____________________________

/// @description
/// Creates a valid `Semaphore` object using the given `allocator`.
/// @note Thin wrapper for `vkCreateSemaphore` using the cvulkan API.
cvk_Pure cvk_Semaphore cvk_semaphore_create (  // clang-format off
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
);  // clang-format on

/// @description
/// Releases any memory and handles created by `cvk_semaphore_create` using the same `allocator`.
void cvk_semaphore_destroy ( // clang-format off
  cvk_Semaphore* const            semaphore,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on


//______________________________________
// @section Fence
//____________________________

/// @description
/// Creates a valid `Fence` object using the given `allocator`.
/// @note Thin wrapper for `vkCreateFence` using the cvulkan API.
cvk_Pure cvk_Fence cvk_fence_create (  // clang-format off
  cvk_device_Logical const* const device_logical,
  cvk_bool const                  signaled,
  cvk_Allocator const* const      allocator
);  // clang-format on

/// @description
/// Releases any memory and handles created by `cvk_fence_create` using the same `allocator`.
void cvk_fence_destroy ( // clang-format off
  cvk_Fence* const                fence,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on

/// @description
/// Orders the CPU to synchronously wait (indefinitely) until the given `fence` is signaled by the GPU.
/// @note Thin inline wrapper for `vkWaitForFences` using the cvulkan API.
void cvk_fence_wait ( // clang-format off
  cvk_Fence const* const          fence,
  cvk_device_Logical const* const device_logical
); // clang-format on

/// @description
/// Orders Vulkan to set the state of the given `fence` to its initial state.
/// @note Thin inline wrapper for `vkResetFences` using the cvulkan API.
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

