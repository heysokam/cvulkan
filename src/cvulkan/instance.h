//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Vulkan Instance: Types & Tools
//_______________________________________________|
#ifndef H_cvk_instance
#define H_cvk_instance
#include "./base.h"
#include "./types.h"


//______________________________________
// @section Instance: Options
//____________________________

/// @description
/// Creates a valid VkInstanceCreateInfo object with the given arguments
cvk_Pure VkInstanceCreateInfo cvk_instance_options_create ( // clang-format off
  cvk_Application const* const  application,
  VkInstanceCreateFlags const   flags,
  cvk_Slice const               layers,
  cvk_instance_Extensions const extensions
); // clang-format on


//______________________________________
// @section Instance: Create/Destroy
//____________________________

typedef struct cvk_instance_create_args {  // clang-format off
  cvk_Nullable cvk_Allocator* const                   allocator;    ///< Will use the default allocator when null
  cvk_Nullable cvk_Slice const                        layers;       ///< Will use the cvk required layers when (0,NULL)
  cvk_Nullable cvk_instance_extensions_Required const extensions;   ///< Will not add any system or user extensions when omitted. Only the defaults will be added.
  cvk_Nullable VkApplicationInfo const* const         application;  ///< Will use `cvk_application_defaults()` when omitted (aka nulll)
  cvk_Nullable cvk_validation_Options const* const    validation;   ///< Will configure debug messenger and validation layers with defaults when null
  cvk_Nullable cvk_bool const                         portability;  ///< Will add Portability flags and extensions when `true`.
} cvk_instance_create_args;  // clang-format on
cvk_Pure cvk_Instance cvk_instance_create (cvk_instance_create_args const* const arg);
void                  cvk_instance_destroy (cvk_Instance* const instance);


//______________________________________
// @section Instance: Helper Tools
//____________________________

/// @description
/// Checks that all layers in the given {@arg required} list are supported by the system
/// @param required
///  List of layers that the function will check support for.
///  Must be a `cvk_Slice[cvk_String]` type
///  Will always return true when the required list is empty.
cvk_bool cvk_instance_layers_checkSupport (cvk_Slice const required, cvk_Allocator* const allocator);


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_instance
#endif
#ifdef cvk_Implementation_instance
#include "./instance.c"
#endif


#endif  // H_cvk_instance

