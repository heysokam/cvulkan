//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
//! @fileoverview
//! Example of using cvulkan to bootstrap all boilerplate
//! up to retrieving a `VkSwapchainKHR` handle.
//!
//! The functions used in this pure C example
//! provide the same functionality that `vk-bootstrap` does for C++
//! https://github.com/charles-lunarg/vk-bootstrap
//!
//! @note
//! Instead of using the Builder Pattern, like vk-bootstrap does,
//! cvulkan uses vanilla C patterns and tools to achieve the same result.
//________________________________________________________________________|
// @deps cdk
#define GLFW_INCLUDE_VULKAN
#define csys_Implementation
#include <csys.h>
// @deps cvulkan
#define cvk_Implementation
#include <cvulkan.h>


int main () {
  // Initialize: Window+Input
  csys_System sys = csys_init(csys_init_Options_defaults());


  //________________________________________________
  // Initialize: GPU Instance
  // @note Validation Layers and Debug Messenger are active by default (unless specified otherwise)
  cvk_Instance instance = cvk_instance_create(&(cvk_instance_create_args){ 0 });  // Create with all defaults


  //________________________________________________
  // Initialize: GPU Surface
  // @important
  // Surface requesting not in cvulkan (by design).
  // It would force this library to depend on a specific windowing library.
  // As such, cvulkan expects you to send the VkSurfaceKHR handle as input.
  // How you request that handle is up to you. cvulkan doesn't need to know about it.
  //
  // @description
  // Create a Vulkan Surface for the GLFWwindow context.
  cvk_Surface surface = NULL;  // clang-format off
  cvk_result_check(glfwCreateWindowSurface(instance.ct, sys.window.ct, instance.allocator.gpu, &surface),
    "Failed to create the Vulkan Surface for the given GLFW window.");  // clang-format on


  //________________________________________________
  // Initialize: GPU Device.Physical
  cvk_device_Physical device_physical = cvk_device_physical_create(&(cvk_device_physical_create_args){
    .instance   = &instance,
    .surface    = surface,
    .forceFirst = cvk_true,
  });


  //________________________________________________
  // Initialize: GPU Device.Queue (without context)
  // @not Queue.ct requires a logical device, but the logical device requires Queues information
  cvk_device_Queue device_queue = cvk_device_queue_create_noContext(&(cvk_device_queue_create_args){
    .instance = &instance,
    .device   = &device_physical,
    .id       = device_physical.queueFamilies.graphics,
    .priority = 1.0f,
  });


  //________________________________________________
  // Initialize: GPU Device.Logical
  cvk_device_Logical device_logical = cvk_device_logical_create(&(cvk_device_logical_create_args){
    .physical  = &device_physical,
    .queue     = &device_queue,
    .allocator = &instance.allocator,
  });


  //________________________________________________
  // Initialize: GPU Device.Queue Context
  // @note Could not be init without a device_logical
  cvk_device_queue_create_context(&device_queue, &device_logical);


  //________________________________________________
  // Initialize: GPU Device.Swapchain
  // @note Initializes the Swapchain with Images + Views + Semaphores
  cvk_device_Swapchain device_swapchain = cvk_device_swapchain_create(&(cvk_device_swapchain_create_args){
    .device_physical = &device_physical,
    .device_logical  = &device_logical,
    .surface         = surface,
    .size            = (cvk_Size2D){ .width = sys.window.width, .height = sys.window.height },
    .allocator       = &instance.allocator,
  });


  //________________________________________________
  // Terminate: All GPU Bootstrapped data
  cvk_device_swapchain_destroy(&device_swapchain, &device_logical, &instance.allocator);
  cvk_device_logical_destroy(&device_logical, &instance.allocator);
  cvk_device_queue_destroy(&device_queue, &instance.allocator);
  cvk_device_physical_destroy(&device_physical, &instance.allocator);
  vkDestroySurfaceKHR(instance.ct, surface, instance.allocator.gpu);
  // To check that everything worked:
  // Introduce a bug by changing the order of this call, or removing one of the above.
  // Validation Layers will catch it, and report the error to cli.
  cvk_instance_destroy(&instance);


  //________________________________________________
  // Terminate the Window and Return
  csys_term(&sys);
  return 0;
}

