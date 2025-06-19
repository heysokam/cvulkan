//:_________________________________________________________________
//  csys  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:_________________________________________________________________
//! @fileoverview WGPUSurface request with csys+glfw
//___________________________________________________|
#ifndef H_csys_glfw_backend_wgpu
#define H_csys_glfw_backend_wgpu
#include "./base.h"

WGPUSurface glfw_wgpu_surface_create (WGPUInstance instance, GLFWwindow* window);

//______________________________________
// @section Single Header Support
//____________________________
#ifdef csys_Implementation_wgpu
#include "./wgpu.c"
#endif


#endif  // H_csys_glfw_backend_wgpu
