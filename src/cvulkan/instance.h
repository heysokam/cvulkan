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
/// Creates a valid `VkInstanceCreateInfo` object using the given configuration options.
cvk_Pure VkInstanceCreateInfo cvk_instance_options_create ( // clang-format off
  cvk_Application const* const  application,
  VkInstanceCreateFlags const   flags,
  cvk_instance_Layers const     layers,
  cvk_instance_Extensions const extensions
); // clang-format on


//______________________________________
// @section Instance: Helper Tools
//____________________________

/// @description
/// Checks that all layers in the given {@arg required} list are supported by the system
/// @param required
///  List of layers that the function will check support for.
///  Must be a `cvk_StringSlice` type
///  Will always return true when the required list is empty.
cvk_bool cvk_instance_layers_checkSupport ( // clang-format off
  cvk_StringSlice const required,
  cvk_Allocator* const  allocator
); // clang-format on

/// @description
/// Frees the memory allocated for the given `list` of layers.
/// All entries will be freed individually, and must be heap allocated.
/// (ie: string literals are not allowed. They must be duplicated with the given allocator)
void cvk_instance_layers_destroy ( // clang-format off
  cvk_instance_Layers  layers,
  cvk_Allocator* const allocator
); // clang-format on

/// @description
/// Configuration options for `cvk_instance_extensions_create`.
typedef struct cvk_instance_extensions_create_args {
  cvk_bool const       debug_active;
  cvk_bool const       portability;
  cvk_Allocator* const allocator;
} cvk_instance_extensions_create_args;

/// @internal
/// Called by `cvk_instance_create`.
/// Use only when implementing an custom version of `cvk_instance_create`
///
/// @description
/// Merges the given `list` of extensions into the result, using the given `arg.allocator`.
/// Filters out any extensions that are not supported on this system.
/// @param `list` Lists of extensions required when creating an instance for the application.
/// @param `arg.debug_active` Will add the DebugMessenger extension when true. Ignored when `list.cvulkan` has a value (aka `list.cvulkan != NULL`)
/// @param `arg.portability` Will add the PortabilityEnumeration extension when true. Ignored when `list.cvulkan` has a value (aka `list.cvulkan != NULL`)
cvk_Pure cvk_instance_Extensions cvk_instance_extensions_create ( // clang-format off
  cvk_instance_extensions_Required const           list,
  cvk_instance_extensions_create_args const* const arg
); // clang-format on

/// @internal
/// Called by `cvk_instance_destroy`.
/// Use only when implementing an custom version of `cvk_instance_destroy`
void cvk_instance_extensions_destroy ( // clang-format off
  cvk_instance_Extensions extensions,
  cvk_Allocator* const    allocator
); // clang-format on


//______________________________________
// @section Instance: Create/Destroy
//____________________________

/// @description
/// Configuration options for `cvk_instance_create`.
typedef struct cvk_instance_create_args {  // clang-format off
  cvk_Nullable cvk_Allocator* const                   allocator;    ///< Will use the default allocator when null
  cvk_Nullable VkApplicationInfo const* const         application;  ///< Will use `cvk_application_defaults()` when omitted (aka null)
  cvk_Nullable cvk_validation_Options const* const    validation;   ///< Will configure debug messenger and validation layers with defaults when null
  cvk_Nullable cvk_StringSlice const*const            layers;       ///< Leave as (0,NULL) for default behavior (will add validation layers when relevant). Will override all layers when not null. All entries must be dynamically allocated.
  cvk_Nullable cvk_instance_extensions_Required const extensions;   ///< Will not add any system or user extensions when omitted. Only the defaults will be added.
  cvk_Nullable cvk_bool const                         portability;  ///< Will add Portability flags and extensions when `true`.
  char                                                priv_pad[4];
} cvk_instance_create_args;  // clang-format on

/// @description
/// Creates an `Instance` object with the required extensions, layers and Validation/Debug information configured as defined by `cvk_instance_create_args`
///
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_instance_destroy` using the same `allocator`,
cvk_Pure cvk_Instance cvk_instance_create (  // clang-format off
  cvk_instance_create_args const* const arg
);  // clang-format on

/// @description
/// Releases any memory and handles created by `cvk_instance_create` for the given `instance`.
void cvk_instance_destroy (  // clang-format off
  cvk_Instance* const instance
);  // clang-format on


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

