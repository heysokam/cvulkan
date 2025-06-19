//:_________________________________________________________________
//  csys  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:_________________________________________________________________
#ifndef H_csys_glfw
#define H_csys_glfw
#include "./base.h"
#include "./aliases.h"
#include "./api.h"

// #define csys_WGPU // NOTE: Here only for debugging. This belongs on the buildsystem

#if defined csys_Implementation && defined csys_WGPU
#define csys_Implementation_wgpu
#endif
#ifdef csys_Implementation_wgpu
#define csys_WGPU
#endif  // csys_wgpu_Implementation

#ifdef csys_WGPU
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <webgpu.h>
#include <wgpu.h>
#pragma GCC diagnostic pop
#include "./wgpu.h"
#endif  // csys_WGPU

#endif  // H_csys_glfw

