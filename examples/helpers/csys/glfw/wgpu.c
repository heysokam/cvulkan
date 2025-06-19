//:_________________________________________________________________
//  csys  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:_________________________________________________________________
#include "./wgpu.h"

#if defined _WIN32
#error "Windows WGPUSurface creation not implemented"
#elif defined __linux__
#define GLFW_EXPOSE_NATIVE_X11
#include <X11/Xlib.h>
#elif defined __APPLE__
#error "Mac WGPUSurface creation not implemented"
#else
#error "WGPUSurface creation for the current system is not supported"
#endif
#include <GLFW/glfw3native.h>


#ifdef _WIN32
static WGPUSurface wgpu_surface_createWin (
  WGPUInstance instance,
  GLFWwindow* window
) {  // clang-format off
  (void)instance;
  (void)window;
  return NULL;
}  // clang-format on
#endif

#ifdef __APPLE__
static WGPUSurface wgpu_surface_createMac (
  WGPUInstance instance
) {  // clang-format off
  (void)instance;
  return NULL;
}  // clang-format on
#endif

#ifdef __linux__
static WGPUSurface wgpu_surface_createX11 (
  WGPUInstance instance,
  GLFWwindow* window
) {  // clang-format off
  Display* x11_display = glfwGetX11Display();
  Window   x11_window  = glfwGetX11Window(window);
  return wgpuInstanceCreateSurface(
    /* instance   */ instance,
    /* descriptor */ &(WGPUSurfaceDescriptor const){
      .nextInChain = (WGPUChainedStruct const*)&(WGPUSurfaceSourceXlibWindow const){
        .chain     = (WGPUChainedStruct const){
          .sType   = WGPUSType_SurfaceSourceXlibWindow,
          },
        .display   = x11_display,
        .window    = x11_window,
      },
    }
  );
}  // clang-format on
#endif


WGPUSurface glfw_wgpu_surface_create (
  WGPUInstance instance,
  GLFWwindow* window
) {  // clang-format off
  #if defined _WIN32
  return wgpu_surface_createWin(instance);
  #elif defined __linux__
  return wgpu_surface_createX11(instance, window);
  #elif defined __APPLE__
  return wgpu_surface_createMac(instance);
  #endif
}  // clang-format on

