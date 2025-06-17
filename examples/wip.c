//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
// @deps cdk
#include <stdint.h>
#define GLFW_INCLUDE_VULKAN
#define csys_Implementation
#include <csys.h>
// @deps cvulkan
#define cvk_Implementation
#include <cvulkan.h>

extern unsigned char examples_shaders_triangle_frag_spv[];
extern unsigned int examples_shaders_triangle_frag_spv_len;
#include "./shaders/triangle.frag.c"

extern unsigned char examples_shaders_triangle_vert_spv[];
extern unsigned int examples_shaders_triangle_vert_spv_len;
#include "./shaders/triangle.vert.c"

typedef struct {
  cvk_Instance         instance;
  cvk_Surface          surface;
  cvk_device_Physical  device_physical;
  cvk_device_Queue     device_queue;
  cvk_device_Logical   device_logical;
  cvk_device_Swapchain device_swapchain;
} GPU;

typedef struct Example {
  GPU gpu;
  struct {
    cvk_Shader vert;
    cvk_Shader frag;
  } shader;
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


static GPU example_gpu_create (
  GLFWwindow* const window_ct,  ///< Only for surface creation. @see example_surface_create for more info
  cvk_Size2D const  window_size
) {  // clang-format off
  GPU result           = (GPU){ 0 };
  result.instance         = cvk_instance_create(&(cvk_instance_create_args){ 0 });  // Create with all defaults
  result.surface          = example_surface_create(result.instance.ct, window_ct, result.instance.allocator.gpu);
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
  });                           // clang-format on
  return result;
}


static void example_gpu_destroy (
  GPU* gpu
) {
  cvk_device_swapchain_destroy(&gpu->device_swapchain, &gpu->device_logical, &gpu->instance.allocator);
  cvk_device_logical_destroy(&gpu->device_logical, &gpu->instance.allocator);
  cvk_device_queue_destroy(&gpu->device_queue, &gpu->instance.allocator);
  cvk_device_physical_destroy(&gpu->device_physical, &gpu->instance.allocator);
  example_surface_destroy(gpu->instance.ct, gpu->surface, gpu->instance.allocator.gpu);
  cvk_instance_destroy(&gpu->instance);
}

static Example example_create (
  csys_System const* const system,
  cvk_SpirV                vert,
  cvk_SpirV                frag
) {
  Example result     = (Example){ 0 };
  result.gpu         = example_gpu_create(system->window.ct, (cvk_Size2D){ .width = system->window.width, .height = system->window.height });
  result.shader.vert = cvk_shader_create(&(cvk_shader_create_args){
    .device_logical = &result.gpu.device_logical,
    .stage          = cvk_shader_stage_Vertex,
    .code           = vert,
    .entryFn_name   = "main",  // <- @note main is already the default when not provided
    .allocator      = &result.gpu.instance.allocator,
  });
  result.shader.frag = cvk_shader_create(&(cvk_shader_create_args){
    .device_logical = &result.gpu.device_logical,
    .stage          = cvk_shader_stage_Fragment,
    .code           = frag,
    .allocator      = &result.gpu.instance.allocator,
  });
  return result;
}

static void example_destroy (
  Example* const example
) {
  cvk_shader_destroy(&example->gpu.device_logical, &example->shader.frag, &example->gpu.instance.allocator);
  cvk_shader_destroy(&example->gpu.device_logical, &example->shader.vert, &example->gpu.instance.allocator);
  example_gpu_destroy(&example->gpu);
}

int main () {
  // Initialize
  csys_System system  = csys_init(csys_init_Options_defaults());
  Example     example = example_create(
    /* system */ &system,
    /* vert   */ (cvk_SpirV){ .ptr = (uint32_t*)examples_shaders_triangle_vert_spv, .len = examples_shaders_triangle_vert_spv_len},
    /* frag   */ (cvk_SpirV){ .ptr = (uint32_t*)examples_shaders_triangle_frag_spv, .len = examples_shaders_triangle_frag_spv_len }
  );
  // Update Loop
  while (!csys_close(&system)) { csys_update(&system); }
  // Terminate
  example_destroy(&example);
  csys_term(&system);
  return 0;
}

