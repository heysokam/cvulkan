//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
#ifndef H_cvulkan_base
#define H_cvulkan_base


//______________________________________
// @section Dependencies
//____________________________
#include <stdint.h>
#include <vulkan/vulkan_core.h>


//______________________________________
// @section stdlib Primitives
//____________________________

#ifndef cvk_bool
typedef enum cvk_bool { cvk_false = 0, cvk_true } cvk_bool;
#endif  // cvk_bool

#ifndef cvk_size
#include <stddef.h>
#define cvk_size size_t
#endif  // cvk_size

typedef void*       cvk_pointer;
typedef char const* cvk_String;
typedef cvk_String* cvk_StringList;


//______________________________________
// @section Primitives
//____________________________

typedef uint32_t cvk_Version;

typedef struct cvk_Slice cvk_Slice;
struct cvk_Slice {
  cvk_size    len;
  cvk_pointer ptr;
};
// clang-format off
#define cvk_Slice_empty() (cvk_Slice){ .len = 0, .ptr = NULL }
// clang-format on


//______________________________________
// @section Pragmas
//____________________________

#ifndef cvk_Nullable
/// @description
/// Marker to indicate that a value accepts null without errors
#define cvk_Nullable
#endif  // cvk_Nullable


#ifndef cvk_Readonly
/// @description
/// Marker to indicate that a value should only be read after writing
#define cvk_Readonly
#endif  // cvk_Readonly


#ifndef cvk_Pure
/// @description
/// Compiler marker to indicate that a function cannot have any sideEffects
#if defined(__GNUC__) || defined(__clang__)
#define cvk_Pure __attribute__((const))
#else
#define cvk_Pure
#endif  // gcc/clang
#endif  // cvk_Pure


#endif  // H_cvulkan_base
