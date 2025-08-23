//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Vulkan Pipeline: Types & Tools
//________________________________________________|
#ifndef H_cvulkan_pipeline
#define H_cvulkan_pipeline
#include "./base.h"
#include "./types.h"


//______________________________________
// @section Pipeline: State
//____________________________

/// @description
/// Creates a valid `VkPipelineDynamicStateCreateInfo` using the given `states` config arguments.
cvk_Pure VkPipelineDynamicStateCreateInfo cvk_pipeline_state_dynamic_setup ( // clang-format off
  cvk_size const       states_len,
  VkDynamicState const states_ptr[]
); // clang-format on

/// @description
/// Creates a valid `VkPipelineVertexInputStateCreateInfo` using the cvulkan defaults.
/// The result can changed as needed. Check the implementation to see what the default values are.
cvk_Pure VkPipelineVertexInputStateCreateInfo cvk_pipeline_state_vertexInput_defaults ();

/// @description
/// Creates a valid `VkPipelineInputAssemblyStateCreateInfo` using the cvulkan defaults.
/// The result can changed as needed. Check the implementation to see what the default values are.
cvk_Pure VkPipelineInputAssemblyStateCreateInfo cvk_pipeline_state_inputAssembly_defaults ();

/// @description
/// Creates a valid `VkPipelineViewportStateCreateInfo` using the cvulkan defaults.
/// The result can changed as needed. Check the implementation to see what the default values are.
cvk_Pure VkPipelineViewportStateCreateInfo cvk_pipeline_state_viewport_defaults ();

/// @description
/// Creates a valid `VkPipelineRasterizationStateCreateInfo` using the cvulkan defaults.
/// The result can changed as needed. Check the implementation to see what the default values are.
cvk_Pure VkPipelineRasterizationStateCreateInfo cvk_pipeline_state_rasterization_defaults ();

/// @description
/// Creates a valid `VkPipelineMultisampleStateCreateInfo` using the cvulkan defaults.
/// The result can changed as needed. Check the implementation to see what the default values are.
cvk_Pure VkPipelineMultisampleStateCreateInfo cvk_pipeline_state_multisample_defaults ();

/// @description
/// Creates a valid `VkPipelineColorBlendAttachmentState` using the cvulkan defaults.
/// The result can changed as needed. Check the implementation to see what the default values are.
cvk_Pure VkPipelineColorBlendAttachmentState cvk_pipeline_state_colorBlend_attachment_defaults ();

/// @description
/// Configuration options for `cvk_pipeline_state_colorBlend_setup_args`.
typedef struct cvk_pipeline_state_colorBlend_setup_args {
  cvk_Nullable cvk_size const                                   attachments_len;  ///< Will default to `1` when `.ptr` is not NULL and `.len` is omitted.
  cvk_Nullable VkPipelineColorBlendAttachmentState const* const attachments_ptr;  ///< Can be NULL to not setup any color blend attachment state.
} cvk_pipeline_state_colorBlend_setup_args;

/// @description
/// Creates a valid `VkPipelineColorBlendAttachmentState` using the given `arg` configuration, and other cvulkan defaults.
/// The result can changed as needed. Check the implementation to see what other defaults are setup.
cvk_Pure VkPipelineColorBlendStateCreateInfo cvk_pipeline_state_colorBlend_setup (  // clang-format off
  cvk_pipeline_state_colorBlend_setup_args const* const arg
);  // clang-format on

/// @description
/// Orders Vulkan to record a command to activate the given `viewport` object so that it can be used for other operations.
/// @note Thin inline wrapper for `vkCmdSetViewport` using the cvulkan API.
void cvk_viewport_command_set ( // clang-format off
  VkViewport const* const         viewport,
  cvk_command_Buffer const* const command_buffer
); // clang-format on

/// @description
/// Orders Vulkan to record a command to activate the given `scissor` object so that it can be used for other operations.
/// @note Thin inline wrapper for `vkCmdSetScissor` using the cvulkan API.
void cvk_scissor_command_set ( // clang-format off
  VkRect2D const* const           scissor,
  cvk_command_Buffer const* const command_buffer
); // clang-format on


//______________________________________
// @section Pipeline: Layout
//____________________________

typedef struct cvk_pipeline_layout_create_args {
  cvk_device_Logical const* const           device_logical;
  char                                      priv_pad[4];
  cvk_Nullable uint32_t                     sets_len;
  cvk_Nullable VkDescriptorSetLayout const* sets_ptr;
  cvk_Allocator* const                      allocator;
} cvk_pipeline_layout_create_args;
cvk_Pure cvk_pipeline_Layout cvk_pipeline_layout_create (  // clang-format off
  cvk_pipeline_layout_create_args const* const arg
);  // clang-format on

void cvk_pipeline_layout_destroy ( // clang-format off
  cvk_pipeline_Layout* const      layout,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on


//______________________________________
// @section Pipeline: Graphics Context
//____________________________

/// @description
/// Configuration options to create a derivative pipeline from `cvk_pipeline_graphics_create`.
typedef struct cvk_pipeline_graphics_derivative_args {
  VkPipeline const handle;
  int32_t const    index;
  char             priv_pad[4];
} cvk_pipeline_graphics_derivative_args;

/// @description
/// Configuration options for `cvk_pipeline_graphics_create`.
typedef struct cvk_pipeline_graphics_create_args {
  cvk_device_Logical const* const                                 device_logical;
  cvk_Allocator* const                                            allocator;
  VkPipelineCreateFlags const                                     flags;
  char                                                            priv_pad1[4];
  cvk_pipeline_shaderStage_List* const                            stages;
  VkPipelineVertexInputStateCreateInfo const* const               state_vertexInput;
  VkPipelineInputAssemblyStateCreateInfo const* const             state_inputAssembly;
  cvk_Nullable VkPipelineTessellationStateCreateInfo const* const state_tessellation;
  VkPipelineViewportStateCreateInfo const* const                  state_viewport;
  VkPipelineRasterizationStateCreateInfo const* const             state_rasterization;
  VkPipelineMultisampleStateCreateInfo const* const               state_multisample;
  cvk_Nullable VkPipelineDepthStencilStateCreateInfo const* const state_depthStencil;
  VkPipelineColorBlendStateCreateInfo const* const                state_colorBlend;
  VkPipelineDynamicStateCreateInfo const* const                   state_dynamic;
  cvk_Nullable cvk_Renderpass const* const                        renderpass;  ///< Will pass an empty renderpass when omitted (aka. NULL)
  cvk_Nullable uint32_t const                                     subpass;     ///< Only needed for Tile-Based GPUs
  char                                                            priv_pad2[4];
  cvk_Nullable cvk_pipeline_layout_create_args const* const       layout;      ///< Will create an all-defaults layout when omitted (aka. NULL)
  cvk_Nullable cvk_pipeline_graphics_derivative_args const* const derivative;  ///< Will not pass derivative args when omitted (aka. NULL)
} cvk_pipeline_graphics_create_args;

/// @description
/// Creates a valid `Pipeline.Graphics` object using the given `arg` configuration options.
///
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_pipeline_graphics_destroy` using the same `allocator`.
cvk_Pure cvk_pipeline_Graphics cvk_pipeline_graphics_create (  // clang-format off
  cvk_pipeline_graphics_create_args const* const arg
);  // clang-format on

/// @description
/// Releases any memory and handles created by `cvk_pipeline_graphics_create` using the same `allocator`.
void cvk_pipeline_graphics_destroy ( // clang-format off
  cvk_pipeline_Graphics* const    pipeline,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
); // clang-format on

/// @description
/// Orders Vulkan to record a command to connect the given `pipeline` object so that it can be used for other operations.
/// @note Thin inline wrapper for `vkCmdBindPipeline` using the cvulkan API.
void cvk_pipeline_graphics_command_bind ( // clang-format off
  cvk_pipeline_Graphics const* const pipeline,
  cvk_command_Buffer const* const    command_buffer
); // clang-format on

/// @description
/// Configuration options for `cvk_command_draw_args`.
typedef struct cvk_command_draw_args {
  cvk_Nullable uint32_t vertex_len;
  uint32_t              vertex_first;
  cvk_Nullable uint32_t instance_len;
  uint32_t              instance_first;
} cvk_command_draw_args;

/// @description
/// Orders Vulkan to record a draw command into the given `command_buffer`.
/// Will use the arguments described in `arg`, or sane defaults when omitted.
/// @note Thin inline wrapper for `vkCmdDraw` using the cvulkan API.
void cvk_command_draw (  // clang-format off
  cvk_command_Buffer const* const    command_buffer,
  cvk_command_draw_args const* const arg
);  // clang-format on

/// @description
/// Configuration options for `cvk_command_draw_indexed_args`.
typedef struct cvk_command_draw_indexed_args {
  cvk_Nullable uint32_t indices_len;
  uint32_t              indices_first;
  cvk_Nullable uint32_t instance_len;
  uint32_t              instance_first;
  int32_t               vertex_offset;
} cvk_command_draw_indexed_args;

/// @description
/// Orders Vulkan to record an indexed draw command into the given `command_buffer`.
/// Will use the arguments described in `arg`, or sane defaults when omitted.
/// @note Thin inline wrapper for `vkCmdDrawIndexed` using the cvulkan API.
void cvk_command_draw_indexed (  // clang-format off
  cvk_command_Buffer const* const    command_buffer,
  cvk_command_draw_indexed_args const* const arg
);  // clang-format on


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_pipeline
#endif
#ifdef cvk_Implementation_pipeline
#include "./pipeline/state.c"
#include "./pipeline/layout.c"
#include "./pipeline/graphics.c"
// #include "./pipeline/compute.c"
#endif


#endif  // H_cvulkan_pipeline

