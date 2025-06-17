//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
//! @fileoverview Base cvulkan tools used by all other modules
//_____________________________________________________________|
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

#ifndef cvk_String_equal
#include <string.h>
#define cvk_String_equal(A, B) (strcmp((A), (B)) == 0)
#endif  // cvk_String_equal


//______________________________________
// @section Primitives
//____________________________

typedef uint32_t   cvk_Version;
typedef VkExtent2D cvk_Size2D;
typedef VkExtent3D cvk_Size3D;


typedef struct cvk_Slice_s {
  cvk_size    len;
  cvk_size    itemsize; ///< Readonly for statically typed slices.
  cvk_pointer ptr;
} cvk_Slice_t;
#ifndef cvk_Slice  // clang-format off
#define cvk_Slice cvk_Slice_t
#define cvk_Slice_empty() (cvk_Slice){ .len = 0, .itemsize= 0, .ptr = NULL }
#define cvk_Slice_isEmpty(slice) ((slice).ptr == NULL || (slice).len == 0)
// clang-format on
#endif  // cvk_Slice

#ifndef cvk_Optional_u32
#define cvk_Optional_u32               uint32_t
#define cvk_Optional_u32_none          UINT32_MAX
#define cvk_Optional_u32_isNone(val)   ((val) == cvk_Optional_u32_none)
#define cvk_Optional_u32_hasValue(val) (!cvk_Optional_u32_isNone((val)))
#endif  // cvk_Optional


//______________________________________
// @section stdlib Helpers
//____________________________

#ifndef cvk_print
#include <stdio.h>
#define cvk_print printf
#endif  // cvk_print


//______________________________________
// @section Pragmas (compiler helpers)
//____________________________

#ifndef cvk_discard
/// @description
/// Marks the given value as explicitly unused
#define cvk_discard(val) (void)(val)
#endif  // cvk_discard


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
