//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
// @deps cdk
#define GLFW_INCLUDE_VULKAN
#define csys_Implementation
#include <csys.h>
// @deps cvulkan
#define cvk_Implementation
#include <cvulkan.h>


typedef struct Example {
  cvk_Instance        instance;
  cvk_Surface         surface;
  cvk_device_Physical device_physical;
  cvk_device_Queue    device_queue;
  cvk_device_Logical  device_logical;
} Example;


/// @important
/// Surface requesting would force this library to depend on a specific windowing library.
/// As such, cvulkan expects you to send the VkSurfaceKHR handle as input.
/// How you request that handle is up to you. cvulkan doesn't need to know about it.
///
/// @description
/// Example of how to create a valid Vulkan Surface for the given GLFW {@arg window}
///
static cvk_Surface example_surface_create (
  VkInstance const       instance,
  GLFWwindow* const      window,
  VkAllocationCallbacks* allocator
) {
  VkSurfaceKHR result = NULL;  // clang-format off
  cvk_result_check(glfwCreateWindowSurface(instance, window, allocator, &result),
    "Failed to create the Vulkan Surface for the given GLFW window.");  // clang-format on
  return result;
}
#define example_surface_destroy vkDestroySurfaceKHR

static Example example_create (
  GLFWwindow* const window  // Only used for surface creation. @see example_surface_create for more info
) {
  Example result         = (Example){ 0 };
  result.instance        = cvk_instance_create(&(cvk_instance_create_args){ 0 }); // Create with all defaults
  result.surface         = example_surface_create(result.instance.ct, window, result.instance.allocator.gpu);
  result.device_physical = cvk_device_physical_create(&(cvk_device_physical_create_args){
    .instance   = &result.instance,
    .surface    = result.surface,
    .forceFirst = cvk_true,
  });  // clang-format off
  result.device_queue = cvk_device_queue_create_noContext(&(cvk_device_queue_create_args){
    .instance = &result.instance,
    .device   = &result.device_physical,
    .id       = result.device_physical.queueFamilies.graphics,
    .priority = 1.0f,
  });
  result.device_logical = cvk_device_logical_create(&(cvk_device_logical_create_args){
    .physical  = &result.device_physical,
    .queue     = &result.device_queue,
    .allocator = &result.instance.allocator,
  });  // clang-format on
  cvk_device_queue_create_context(&result.device_queue, &result.device_logical);
  return result;
}

static void example_destroy (
  Example* gpu
) {
  cvk_device_logical_destroy(&gpu->device_logical, &gpu->instance.allocator);
  cvk_device_queue_destroy(&gpu->device_queue, &gpu->instance.allocator);
  cvk_device_physical_destroy(&gpu->device_physical, &gpu->instance.allocator);
  example_surface_destroy(gpu->instance.ct, gpu->surface, gpu->instance.allocator.gpu);
  cvk_instance_destroy(&gpu->instance);
}

int main () {
  // Initialize
  csys_System sys = csys_init(csys_init_Options_defaults());
  Example     gpu = example_create(sys.window.ct);
  // Update Loop
  while (!csys_close(&sys)) { csys_update(&sys); }
  // Terminate
  example_destroy(&gpu);
  csys_term(&sys);
  return 0;
}

