//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Memory Management tools for the GPU and CPU
//____________________________________________________________|
#ifndef H_cvulkan_memory
#define H_cvulkan_memory
#include "./base.h"
#include "./types.h"


//______________________________________
// @section Allocator.CPU: Defaults (stdlib)
//____________________________

/// @description Thin wrapper for using stdlib.h/malloc with our allocator API
cvk_Pure cvk_Slice cvk_allocator_alloc_stdlib (cvk_Allocator_CPU* const A, cvk_size const count, cvk_size const itemsize);
/// @description Thin wrapper for using stdlib.h/calloc with our allocator API
cvk_Pure cvk_Slice cvk_allocator_allocZ_stdlib (cvk_Allocator_CPU* const A, cvk_size const count, cvk_size const itemsize);
/// @description Thin wrapper for using stdlib.h/free with our allocator API
void cvk_allocator_free_stdlib (cvk_Allocator_CPU* const A, cvk_Slice* const data);
/// @description Thin wrapper for using stdlib.h/memcpy with our allocator API
void cvk_allocator_copy_stdlib (cvk_Allocator_CPU* const A, cvk_Slice const* const src, cvk_Slice* const trg);
/// @description Thin wrapper for using stdlib.h/malloc+memcpy with our allocator API
cvk_Pure cvk_Slice cvk_allocator_duplicate_stdlib (cvk_Allocator_CPU* const A, cvk_Slice const* const data);
/// @description
/// Function that creates a stdlib CPU allocator wrapper
/// Allows using stdlib.h with our allocator API
cvk_Pure cvk_Allocator_CPU cvk_allocator_cpu_stdlib ();
#ifndef cvk_allocator_cpu_defaults
/// @description
/// Function that creates the default allocator that cvk will use internally for CPU allocations
/// @overridable Add `#define cvk_allocator_cpu_defaults yourAllocatorGeneratorFunction` to your code to change this default
#define cvk_allocator_cpu_defaults() cvk_allocator_cpu_stdlib()
#endif  // cvk_allocator_cpu_defaults


//______________________________________
// @section Allocator.GPU: NULL
//____________________________
#ifndef cvk_allocator_gpu_defaults
/// @description
/// Function that creates the pointer to the default GPU allocator that will be sent to Vulkan
/// @overridable Add `#define cvk_allocator_gpu_defaults yourAllocatorGeneratorFunction` to your code to change this default behavior
#define cvk_allocator_gpu_defaults() (VkAllocationCallbacks*)(NULL)
#endif  // cvk_allocator_gpu_defaults


//______________________________________
// @section Allocator: Default (cpu:stdlib, gpu:NULL)
//____________________________

/// @description
/// Function that creates the default `cvk_Allocator` object used by the cvk API
cvk_Pure cvk_Allocator cvk_Allocator_defaults ();


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_memory
#endif
#ifdef cvk_Implementation_memory
#include "./memory.c"
#endif


#endif  // H_cvulkan_memory

