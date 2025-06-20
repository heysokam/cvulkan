//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
//! @fileoverview
//! cvulkan boilerplate bootstrapping code for the examples
//__________________________________________________________|
// @deps cvulkan
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <cvulkan.h>


typedef struct example_Bootstrap {
  cvk_Instance         instance;
  cvk_Surface          surface;
  cvk_device_Physical  device_physical;
  cvk_device_Queue     device_queue;
  cvk_device_Logical   device_logical;
  cvk_device_Swapchain device_swapchain;
} example_Bootstrap;


/// @important
/// Surface requesting would force this library to depend on a specific windowing library.
/// As such, cvulkan expects you to send the VkSurfaceKHR handle as input.
/// How you request that handle is up to you. cvulkan doesn't need to know about it.
///
/// @description
/// Example of how to create a valid Vulkan Surface for the given GLFW {@arg window}
///
static cvk_Surface example_bootstrap_surface_create (
  VkInstance const       instance,
  GLFWwindow* const      window,
  VkAllocationCallbacks* allocator
) {
  VkSurfaceKHR result = NULL;  // clang-format off
  cvk_result_check(glfwCreateWindowSurface(instance, window, allocator, &result),
    "Failed to create the Vulkan Surface for the given GLFW window.");  // clang-format on
  return result;
}
#define example_bootstrap_surface_destroy vkDestroySurfaceKHR


static example_Bootstrap example_bootstrap_create (
  GLFWwindow* const window_ct,  ///< Only for surface creation. @see bootstrap_surface_create for more info
  cvk_Size2D const  window_size
) {  // clang-format off
  example_Bootstrap result = (example_Bootstrap){ 0 };
  result.instance         = cvk_instance_create(&(cvk_instance_create_args){ 0 });  // Create with all defaults
  result.surface          = example_bootstrap_surface_create(result.instance.ct, window_ct, result.instance.allocator.gpu);
  result.device_physical  = cvk_device_physical_create(&(cvk_device_physical_create_args){
    .instance             = &result.instance,
    .surface              = result.surface,
    .forceFirst           = cvk_true,
  });
  result.device_queue     = cvk_device_queue_create_noContext(&(cvk_device_queue_create_args){
    .instance             = &result.instance,
    .device               = &result.device_physical,
    .id                   = result.device_physical.queueFamilies.graphics,
    .priority             = 1.0f,
  });
  result.device_logical   = cvk_device_logical_create(&(cvk_device_logical_create_args){
    .physical             = &result.device_physical,
    .queue                = &result.device_queue,
    .allocator            = &result.instance.allocator,
  });
  cvk_device_queue_create_context(&result.device_queue, &result.device_logical);
  result.device_swapchain = cvk_device_swapchain_create(&(cvk_device_swapchain_create_args){
    .device_physical      = &result.device_physical,
    .device_logical       = &result.device_logical,
    .surface              = result.surface,
    .size                 = window_size,
    .allocator            = &result.instance.allocator,
  });  // clang-format on
  return result;
}


static void example_bootstrap_destroy (
  example_Bootstrap* gpu
) {
  cvk_device_swapchain_destroy(&gpu->device_swapchain, &gpu->device_logical, &gpu->instance.allocator);
  cvk_device_logical_destroy(&gpu->device_logical, &gpu->instance.allocator);
  cvk_device_queue_destroy(&gpu->device_queue, &gpu->instance.allocator);
  cvk_device_physical_destroy(&gpu->device_physical, &gpu->instance.allocator);
  example_bootstrap_surface_destroy(gpu->instance.ct, gpu->surface, gpu->instance.allocator.gpu);
  cvk_instance_destroy(&gpu->instance);
}

