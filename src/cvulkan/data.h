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

/// @description
/// Configuration options for `cvk_memory_create`.
typedef struct cvk_memory_create_args {
  cvk_device_Logical const* const   device_logical;
  cvk_Nullable cvk_pointer const    data;       ///< Will automatically map+copy the memory to the GPU when not `NULL`
  cvk_Nullable cvk_Allocator* const allocator;  ///< Can only be `NULL` when `.data` is `NULL`
  VkDeviceSize                      size_alloc;
  VkDeviceSize                      size_data;
  uint32_t                          kind;
  cvk_Nullable cvk_bool const       persistent;  ///< The memory will remain mapped after creation.
  cvk_Nullable VkDeviceSize const   offset;
} cvk_memory_create_args;

/// @description
/// Creates a valid `Memory` object using the given `arg` configuration options.
///
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_memory_destroy` using the same `allocator`.
cvk_Pure cvk_Memory cvk_memory_create (  // clang-format off
  cvk_memory_create_args const* const arg
);  // clang-format on

/// @description
/// Releases any memory and handles created by `cvk_memory_create` using the same `allocator`.
/// @warning The pointer at `memory.data` will not be freed, and must be managed by the caller independently.
void cvk_memory_destroy ( // clang-format off
  cvk_Memory* const               memory,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
); // clang-format on

/// @description
/// Searches for, and returns, the correct memory type for the given `memory` properties.
/// Will return the first type that matches the memoryTypes of `device_physical` and the given `flags`.
/// Will return `Optional.u32.none` and trigger a `cvk_assert` call when it fails to find a matching type.
cvk_Pure cvk_Optional_u32 cvk_memory_properties_type ( // clang-format off
  cvk_memory_Properties const* const memory,
  cvk_device_Physical const* const   device_physical,
  VkMemoryPropertyFlags const        flags
); // clang-format on


//______________________________________
// @section Buffer
//____________________________

/// @description
/// Configuration options for `cvk_buffer_create`.
typedef struct cvk_buffer_create_args {
  cvk_device_Physical const* const       device_physical;
  cvk_device_Logical const* const        device_logical;
  cvk_Allocator* const                   allocator;
  VkDeviceSize const                     size;
  VkBufferUsageFlags const               usage;
  cvk_memory_Flags const                 memory_flags;
  cvk_Nullable VkBufferCreateFlags const flags;
  cvk_Nullable VkSharingMode const       sharing;
  cvk_Nullable uint32_t const* const     queueFamilies_ptr;  ///< Ignored by spec, unless `VK_SHARING_MODE_CONCURRENT`.
  cvk_Nullable cvk_size                  queueFamilies_len;  ///< Ignored by spec, unless `VK_SHARING_MODE_CONCURRENT`.
} cvk_buffer_create_args;

/// @description
/// Creates a valid `Buffer` object using the given `arg` configuration options.
///
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_buffer_destroy` using the same `allocator`.
cvk_Pure cvk_Buffer cvk_buffer_create (  // clang-format off
  cvk_buffer_create_args const* const arg
);  // clang-format on

/// @description
/// Releases any memory and handles created by `cvk_buffer_create` using the same `allocator`.
void cvk_buffer_destroy ( // clang-format off
  cvk_Buffer* const               buffer,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
); // clang-format on

/// @description
/// Configuration options for `cvk_buffer_copy`.
typedef struct cvk_buffer_copy_args {
  cvk_command_Buffer const* const command_buffer;
} cvk_buffer_copy_args;

/// @description
/// Orders Vulkan to record a command to copy all the data from Buffer `A` into Buffer `B`.
/// @note Thin inline wrapper for `vkCmdCopyBuffer` using the cvulkan API.
void cvk_buffer_command_copy ( // clang-format off
  cvk_Buffer const* const A,
  cvk_Buffer const* const B,
  cvk_buffer_copy_args const*const arg
); // clang-format on

/// @description
/// Configuration options for `cvk_buffer_bind`.
typedef struct cvk_buffer_bind_args {
  cvk_device_Logical const* const device_logical;
  cvk_Memory const* const         memory;
  cvk_Nullable VkDeviceSize const offset;  ///< Will bind from the start when omitted (aka. offset == 0)
} cvk_buffer_bind_args;

/// @description
/// Orders Vulkan to record a command to connect the given `buffer` to the given `arg.memory`, so that it can be used for other operations.
/// @note Thin inline wrapper for `vkBindBufferMemory` using the cvulkan API.
void cvk_buffer_bind (  // clang-format off
  cvk_Buffer const* const           buffer,
  cvk_buffer_bind_args const* const arg
);  // clang-format on

/// @description
/// Orders Vulkan to record a command to connect the given vertex `buffer` object so that it can be used for other operations.
/// @note Thin inline wrapper for `vkCmdBindVertexBuffers` using the cvulkan API.
void cvk_buffer_vertex_command_bind ( // clang-format off
  cvk_Buffer const* const         buffer,
  cvk_command_Buffer const* const command_buffer
); // clang-format on

/// @description
/// Orders Vulkan to record a command to connect the given index `buffer` object so that it can be used for other operations.
/// @note Thin inline wrapper for `vkCmdBindIndexBuffer` using the cvulkan API.
void cvk_buffer_index_command_bind ( // clang-format off
  cvk_Buffer const* const         buffer,
  cvk_command_Buffer const* const command_buffer
); // clang-format on


//______________________________________
// @section Image.Data
//____________________________

/// @description
/// Configuration options for `cvk_image_data_create`.
typedef struct cvk_image_data_create_args {
  cvk_device_Physical const* const      device_physical;
  cvk_device_Logical const* const       device_logical;
  cvk_Allocator const* const            allocator;
  VkFormat const                        format;
  VkImageUsageFlags const               usage;
  cvk_memory_Flags const                memory_flags;
  cvk_Nullable VkImageCreateFlags const flags;
  cvk_Nullable VkImageType const        dimensions;         ///< Creates a 1D image when omitted or 0
  cvk_Nullable uint32_t                 width;              ///< Will use 1 when omitted or 0
  cvk_Nullable uint32_t                 height;             ///< Will use 1 when omitted or 0
  cvk_Nullable uint32_t                 depth;              ///< Will use 1 when omitted or 0
  cvk_Nullable VkSampleCountFlags const samples;            ///< Will use 1 when omitted or 0
  cvk_Nullable VkImageTiling const      tiling;             ///< Default: Optimal (aka. 0)
  cvk_Nullable VkSharingMode const      sharing;            ///< Default: Exclusive (aka. 0)
  cvk_Nullable VkImageLayout const      layout;             ///< Undefined is the default (aka 0)
  cvk_Nullable uint32_t const           mip_len;            ///< MipMap levels count. Will use 1 when omitted or 0
  cvk_Nullable uint32_t const           layers_len;         ///< ArrayLayers count. Will use 1 when omitted or 0
  cvk_Nullable uint32_t const* const    queueFamilies_ptr;  ///< Ignored by spec, unless `VK_SHARING_MODE_CONCURRENT`.
  cvk_Nullable cvk_size                 queueFamilies_len;  ///< Ignored by spec, unless `VK_SHARING_MODE_CONCURRENT`.
} cvk_image_data_create_args;

/// @description
/// Creates a valid `Image.Data` object using the given `arg` configuration options.
///
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_image_data_destroy` using the same `allocator`.
cvk_Pure cvk_image_Data cvk_image_data_create (  // clang-format off
  cvk_image_data_create_args const* const arg
);  // clang-format on

/// @description
/// Releases any memory and handles created by `cvk_image_data_create` using the same `allocator`.
void cvk_image_data_destroy ( // clang-format off
  cvk_image_Data* const           image_data,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
); // clang-format on

/// @description
/// Configuration options for `cvk_image_data_bind`.
typedef struct cvk_image_data_bind_args {
  cvk_device_Logical const* const device_logical;
  cvk_Memory const* const         memory;
  cvk_Nullable VkDeviceSize const offset;
} cvk_image_data_bind_args;

/// @description
/// Orders Vulkan to record a command to connect the given `image` to the given `arg.memory`, so that it can be used for other operations.
/// @note Thin inline wrapper for `vkBindImageMemory` using the cvulkan API.
void cvk_image_data_bind ( // clang-format off
  cvk_image_Data const* const      image,
  cvk_image_data_bind_args const* const arg
); // clang-format on

/// @description
/// Configuration options for `cvk_image_data_transition`.
typedef struct cvk_image_data_transition_args {
  cvk_command_Buffer const* const       command_buffer;
  VkAccessFlags const                   access_src;
  VkAccessFlags const                   access_trg;
  VkPipelineStageFlags const            stage_src;
  VkPipelineStageFlags const            stage_trg;
  VkImageLayout const                   layout_old;
  VkImageLayout const                   layout_new;
  cvk_Nullable VkImageAspectFlags const aspect;  ///< Defaults to Color (aka. `1`) when omitted   (spec forbids `0`)
  char                                  priv_pad[4];
} cvk_image_data_transition_args;

/// @description
/// Orders Vulkan to record a command to transition the layout of the given `image_data` from `arg.layout_old` into `arg.layout_new`, so that it can be used for other operations.
/// @note Thin wrapper for `vkCmdPipelineBarrier` using the cvulkan API.
void cvk_image_data_command_transition ( // clang-format off
  cvk_image_Data* const                       image_data,
  cvk_image_data_transition_args const* const arg
); // clang-format on

/// @description
/// Configuration options for `cvk_image_data_copy`.
typedef struct cvk_image_data_copy_args {
  cvk_command_Buffer const* const command_buffer;
  cvk_Nullable VkDeviceSize const offset_buffer;
  cvk_Nullable VkOffset3D const   offset_image;
  char                            priv_pad[4];
} cvk_image_data_copy_args;

/// @description
/// Orders Vulkan to record a command to copy all the data from `buffer` into `image_data`.
/// @note Thin wrapper for `vkCmdCopyBufferToImage` using the cvulkan API.
void cvk_image_data_command_copy_fromBuffer (  // clang-format off
  cvk_image_Data const* const           image_data,
  cvk_Buffer const* const               buffer,
  cvk_image_data_copy_args const* const arg
);  // clang-format on


//______________________________________
// @section Image.View
//____________________________

/// @description
/// Configuration options for `cvk_image_view_create`.
typedef struct cvk_image_view_create_args {
  cvk_image_Data const* const           image_data;
  cvk_device_Logical const* const       device_logical;
  cvk_Allocator const* const            allocator;
  cvk_Nullable VkImageAspectFlags const aspect;  ///< Defaults to Color (aka. `1`) when omitted   (spec forbids `0`)
  char                                  priv_pad[4];
} cvk_image_view_create_args;

cvk_Pure cvk_image_View cvk_image_view_create (  // clang-format off
  cvk_image_view_create_args const* const arg
);  // clang-format on

void cvk_image_view_destroy (  // clang-format off
  cvk_image_View* const           image_view,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
);  // clang-format on


//______________________________________
// @section Image.Sampler
//____________________________

/// @description
/// Configuration options for `cvk_image_sampler_create`.
typedef struct cvk_image_sampler_create_args /* clang-format off */ {
  cvk_device_Physical const* const        device_physical;
  cvk_device_Logical const* const         device_logical;
  cvk_Allocator* const                    allocator;
  cvk_Nullable VkSamplerCreateFlags const flags;
  cvk_Nullable VkFilter const             filter_min;          ///< Default: Nearest (aka. 0)
  cvk_Nullable VkFilter const             filter_mag;          ///< Default: Nearest (aka. 0)
  cvk_Nullable VkSamplerMipmapMode const  mip_mode;            ///< Default: Nearest (aka. 0)
  cvk_Nullable float const                mip_lodBias;         ///< Default: 0.0f
  cvk_Nullable VkSamplerAddressMode const address_U;           ///< Default: Repeat  (aka. 0)
  cvk_Nullable VkSamplerAddressMode const address_V;           ///< Default: Repeat  (aka. 0)
  cvk_Nullable VkSamplerAddressMode const address_W;           ///< Default: Repeat  (aka. 0)
  cvk_Nullable cvk_bool const             anisotropy_enabled;  ///< Default: Disabled
  cvk_Nullable float const                anisotropy_max;      ///< Default: `.device_physical->properties.limits.maxSamplerAnisotropy` when omitted (aka. 0.0f) and `anisotropy_enabled`. Clamped to range `[1.0 .. .device_physical->properties.limits.maxSamplerAnisotropy]`
  cvk_Nullable cvk_bool const             compare_enabled;     ///< Default: Disabled
  cvk_Nullable VkCompareOp const          compare_op;          ///< Default: Never   (aka. 0)
  cvk_Nullable float const                lod_min;             ///< Default: 0.0f
  cvk_Nullable float const                lod_max;             ///< Default: 0.0f
  cvk_Nullable VkBorderColor const        border_color;        ///< Default: Float.Transparent.Black (aka. 0)
  cvk_Nullable cvk_bool const             unnormalized;        ///< Default: Normalized (aka. 0..1 sampling range)
} cvk_image_sampler_create_args;  // clang-format on

cvk_Pure cvk_image_Sampler cvk_image_sampler_create (  // clang-format off
  cvk_image_sampler_create_args const* const arg
);                               // clang-format on

void cvk_image_sampler_destroy ( // clang-format off
  cvk_image_Sampler* const        image_sampler,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
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
#include "./data/image.c"
#endif


#endif  // H_cvk_data

