//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
//! @fileoverview Memory Management tools for the GPU and CPU
//____________________________________________________________|
#ifndef H_cvulkan_memory
#define H_cvulkan_memory
#include "./base.h"

#define cvk_Allocator_CPU void*
#define cvk_Allocator_GPU VkAllocationCallbacks*

typedef struct cvk_Allocator {
  cvk_Allocator_GPU gpu;
  cvk_Allocator_CPU cpu;  // TODO:
} cvk_Allocator;

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
