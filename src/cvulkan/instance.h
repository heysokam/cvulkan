//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
#ifndef H_cvk_instance
#define H_cvk_instance
#include "./base.h"
#include "./memory.h"
#include "./validation.h"
#include "./application.h"

typedef struct cvk_Instance cvk_Instance;

/// @description
/// Function shape expected by cvk to request the required extensions from the system.
/// @returns cvk_Slice[cvk_String] List of string literals with the names of the required extensions
typedef cvk_Slice (*cvk_Fn_extensions_Instance_getRequired)(void);


//______________________________________
// @section Instance: Options
//____________________________

/// @description
/// Creates a valid VkInstanceCreateInfo object with the given arguments
cvk_Pure VkInstanceCreateInfo
cvk_instance_options_create (cvk_Application const* const application, VkInstanceCreateFlags const flags, cvk_Slice const layers, cvk_Slice const extensions);


//______________________________________
// @section Instance: Create/Destroy
//____________________________
typedef struct cvk_instance_create_args {
  cvk_Nullable cvk_Allocator* const                         allocator;       ///< Will use the default allocator when null
  cvk_Nullable cvk_Slice const                              layers;          ///< Will use the cvk required layers when (0,NULL)
  cvk_Nullable cvk_Slice const                              extensions;      ///< Will use the cvk required extensions when (0,NULL)
  cvk_Nullable cvk_Version const                            app_version;     ///< Accepts a value of 0
  cvk_Nullable cvk_Version const                            engine_version;  ///< Accepts a value of 0
  cvk_Nullable cvk_String const                             app_name;        ///< Accepts a null value
  cvk_Nullable cvk_String const                             engine_name;     ///< Accepts a null value
  cvk_Nullable cvk_validation_Options const* const          validation;      ///< Will configure debug messenger and validation layers with defaults when null
  cvk_Nullable cvk_Fn_extensions_Instance_getRequired const extensions_getRequired;  ///< Will use cvk_extensions_Instance_getRequired when null
} cvk_instance_create_args;
cvk_Pure cvk_Instance cvk_instance_create (cvk_instance_create_args const* const arg);
void                  cvk_instance_destroy (cvk_Instance* const instance);


//______________________________________
// @section Instance: Core Type
//____________________________
struct cvk_Instance {
  VkInstance           ct;
  VkInstanceCreateInfo cfg;
  cvk_Allocator        allocator;
  VkApplicationInfo    application;
  cvk_Validation       validation;
};


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
