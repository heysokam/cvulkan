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
  cvk_Nullable cvk_pointer const    data;        ///< {@link cvk_memory_create} Will automatically map+copy the memory to the GPU when not `NULL`
  uint32_t                          kind;
  cvk_Nullable cvk_bool const       persistent;  ///< The memory will remain mapped after creation.
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

cvk_Pure cvk_Optional_u32 cvk_memory_properties_type ( // clang-format off
  cvk_memory_Properties const* const memory,
  cvk_device_Physical const* const   device_physical,
  VkMemoryPropertyFlags const        flags
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
  cvk_command_Buffer const* const command_buffer;
} cvk_buffer_copy_args;
void cvk_buffer_command_copy ( // clang-format off
  cvk_Buffer const* const A,
  cvk_Buffer const* const B,
  cvk_buffer_copy_args const*const arg
); // clang-format on


//______________________________________
// @section Image.Data
//____________________________
typedef struct cvk_image_data_bind_args {
  cvk_device_Logical const* const device_logical;
  cvk_Memory const* const         memory;
  cvk_Nullable VkDeviceSize const offset;
} cvk_image_data_bind_args;
void cvk_image_data_bind ( // clang-format off
  cvk_image_Data const* const      image,
  cvk_image_data_bind_args const* const arg
); // clang-format on

typedef struct cvk_image_data_create_args {
  cvk_device_Physical const* const      device_physical;
  cvk_device_Logical const* const       device_logical;
  cvk_Allocator const* const            allocator;
  VkFormat const                        format;
  VkImageUsageFlags const               usage;
  cvk_memory_Flags const                memory_flags;
  cvk_Nullable VkImageCreateFlags const flags;
  cvk_Nullable VkImageType const        dimensions;  ///< Creates a 1D image when omitted or 0
  cvk_Nullable uint32_t                 width;       ///< Will use 1 when omitted or 0
  cvk_Nullable uint32_t                 height;      ///< Will use 1 when omitted or 0
  cvk_Nullable uint32_t                 depth;       ///< Will use 1 when omitted or 0
  cvk_Nullable VkSampleCountFlags const samples;     ///< Will use 1 when omitted or 0
  cvk_Nullable VkImageTiling const      tiling;      ///< Optimal is the default (aka 0)
  cvk_Nullable VkSharingMode const      sharing;     ///< Exclusive is the default (aka 0)
  cvk_Nullable VkImageLayout const      layout;      ///< Undefined is the default (aka 0)
  cvk_Nullable uint32_t const           mip_len;     ///< MipMap levels count. Will use 1 when omitted or 0
  cvk_Nullable uint32_t const           layers_len;  ///< ArrayLayers count. Will use 1 when omitted or 0
} cvk_image_data_create_args;
cvk_Pure cvk_image_Data cvk_image_data_create (  // clang-format off
  cvk_image_data_create_args const* const arg
);                                                   // clang-format on

void cvk_image_data_destroy ( // clang-format off
  cvk_image_Data* const           image_data,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator const* const      allocator
); // clang-format on

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

void cvk_image_data_command_transition ( // clang-format off
  cvk_image_Data* const                       image_data,
  cvk_image_data_transition_args const* const arg
); // clang-format on

typedef struct cvk_image_data_copy_args {
  cvk_command_Buffer const* const command_buffer;
  cvk_Nullable VkDeviceSize const offset_buffer;
  cvk_Nullable VkOffset3D const   offset_image;
  char                            priv_pad[4];
} cvk_image_data_copy_args;

void cvk_image_data_command_copy_fromBuffer (  // clang-format off
  cvk_image_Data const* const           image_data,
  cvk_Buffer const* const               buffer,
  cvk_image_data_copy_args const* const arg
);  // clang-format on


//______________________________________
// @section Image.View
//____________________________

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

typedef struct cvk_image_sampler_create_args {
  cvk_device_Physical const* const        device_physical;
  cvk_device_Logical const* const         device_logical;
  cvk_Allocator* const                    allocator;
  cvk_Nullable VkSamplerCreateFlags const flags;
  cvk_Nullable VkFilter const             filter_min;  ///< Default (aka. 0) is Nearest
  cvk_Nullable VkFilter const             filter_mag;  ///< Default (aka. 0) is Nearest
  cvk_Nullable VkSamplerMipmapMode const  mip_mode;    ///< Default (aka. 0) is Nearest
  cvk_Nullable float const                mip_lodBias;
  cvk_Nullable VkSamplerAddressMode const address_U;   ///< Default (aka. 0) is Repeat
  cvk_Nullable VkSamplerAddressMode const address_V;   ///< Default (aka. 0) is Repeat
  cvk_Nullable VkSamplerAddressMode const address_W;   ///< Default (aka. 0) is Repeat
  cvk_Nullable cvk_bool const             anisotropy_enabled;
  cvk_Nullable float const
    anisotropy_max;  ///< Default `.device_physical->properties.limits.maxSamplerAnisotropy` when omitted (aka. 0.0f) and `anisotropy_enabled`. Clamped to range `[1.0 .. .device_physical->properties.limits.maxSamplerAnisotropy]`
  cvk_Nullable cvk_bool const      compare_enabled;
  cvk_Nullable VkCompareOp const   compare_op;    ///< Default (aka. 0) is Never
  cvk_Nullable float const         lod_min;
  cvk_Nullable float const         lod_max;
  cvk_Nullable VkBorderColor const border_color;  ///< Default (aka. 0) is Float.Transparent.Black
  cvk_Nullable cvk_bool const      unnormalized;  ///< Default (aka. 0) is normalized (aka. 0..1 sampling range)
} cvk_image_sampler_create_args;

cvk_Pure cvk_image_Sampler cvk_image_sampler_create (  // clang-format off
  cvk_image_sampler_create_args const* const arg
);                                                // clang-format on

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

