//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Vulkan SpirV Shaders: Types & Tools
//____________________________________________________|
#ifndef H_cvulkan_shader
#define H_cvulkan_shader
#include "./base.h"
#include "./types.h"


/// @description
/// Minimal alias of `VkShaderStageFlagBits` for naming consistency.
/// @note Fully compatible with VkShaderStageFlagBits
typedef enum cvk_shader_Stage {
  cvk_shader_stage_Vertex   = VK_SHADER_STAGE_VERTEX_BIT,                   // 1 << 0
  cvk_shader_stage_Fragment = VK_SHADER_STAGE_FRAGMENT_BIT,                 // 1 << 4
  cvk_shader_stage_Compute  = VK_SHADER_STAGE_COMPUTE_BIT,                  // 1 << 5
  cvk_shader_stage_Geometry = VK_SHADER_STAGE_GEOMETRY_BIT,                 // 1 << 3
  cvk_shader_stage_TessCtrl = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,     // 1 << 1
  cvk_shader_stage_TessEval = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,  // 1 << 2
  cvk_shader_stage_Force32  = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM,
} cvk_shader_Stage;

typedef struct cvk_shader_create_args {
  cvk_device_Logical const* const device_logical;
  cvk_SpirV const* const          code;
  cvk_shader_Stage const          stage;
  char                            priv_pad[4];
  cvk_Nullable cvk_String const   entryFn_name;  ///< Will use `main` when NULL
  cvk_Allocator* const            allocator;
} cvk_shader_create_args;

cvk_Pure cvk_Shader cvk_shader_create (  // clang-format off
  cvk_shader_create_args const* const arg
);  // clang-format on
//
void cvk_shader_destroy (  // clang-format off
  cvk_device_Logical const* const device_logical,
  cvk_Shader* const               shader,
  cvk_Allocator* const            allocator
);  // clang-format on


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_shader
#endif
#ifdef cvk_Implementation_shader
#include "./shader.c"
#endif


#endif  // H_cvulkan_shader

