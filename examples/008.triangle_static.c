//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
//! @fileoverview
//! Single function Triangle using cvulkan.
//! Uses the old Framebuffer+Renderpass API, without Dynamic Rendering.
//_____________________________________________________________________|
// @deps cdk
#define GLFW_INCLUDE_VULKAN
#define csys_Implementation
#include <csys.h>
// @deps cvulkan
#define cvk_Implementation
#include <cvulkan.h>

//______________________________________
// SpirV shaders used by this example
extern unsigned char examples_shaders_triangle_frag_spv[];
extern unsigned int  examples_shaders_triangle_frag_spv_len;
#include "./shaders/triangle.frag.c"
//____________________________
extern unsigned char examples_shaders_triangle_vert_spv[];
extern unsigned int  examples_shaders_triangle_vert_spv_len;
#include "./shaders/triangle.vert.c"
//______________________________________


typedef enum {
  example_frames_Len = 2,
} example_Constants;

int main () {
  // Initialize: Window+Input
  csys_System system = csys_init(csys_init_Options_defaults());


  //________________________________________________
  // Initialize: GPU Instance
  // Create with the GLFW required extensions, and all else as defaults
  // @note Validation Layers and Debug Messenger are active by default (unless specified otherwise)
  cvk_instance_extensions_Required extensions = { 0 };  // clang-format off
  extensions.system.ptr = glfwGetRequiredInstanceExtensions((uint32_t*)&extensions.system.len);
  cvk_Instance instance = cvk_instance_create(&(cvk_instance_create_args){
    .extensions = extensions,
  });  // clang-format on


  //________________________________________________
  // Initialize: GPU Surface
  // @important
  // Surface requesting not in cvulkan (by design).
  // This library is system/windowing agnostic.
  // As such, cvulkan expects you to send the VkSurfaceKHR handle as input.
  // How you request that handle is up to you. cvulkan doesn't need to know about it.
  //
  // @description
  // Create a Vulkan Surface for the GLFWwindow context.
  cvk_Surface surface = NULL;  // clang-format off
  cvk_result_check(glfwCreateWindowSurface(instance.ct, system.window.ct, instance.allocator.gpu, &surface),
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
  // @note Queue.ct requires a logical device, but the logical device requires Queues information
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
    .size            = (cvk_Size2D){ .width = system.window.width, .height = system.window.height },
    .allocator       = &instance.allocator,
  });


  //________________________________________________
  // Initialize: Triangle Shaders
  // clang-format off
  cvk_Shader shader_vert = cvk_shader_create(&(cvk_shader_create_args){
    .device_logical = &device_logical,
    .stage          = cvk_shader_stage_Vertex,
    .code           = &(cvk_SpirV){ .ptr = (uint32_t*)examples_shaders_triangle_vert_spv, .len = examples_shaders_triangle_vert_spv_len/sizeof(uint32_t) },
    .entryFn_name   = "main",  // <- @note main is already the default when not provided
    .allocator      = &instance.allocator,
  });
  cvk_Shader shader_frag = cvk_shader_create(&(cvk_shader_create_args){
    .device_logical = &device_logical,
    .stage          = cvk_shader_stage_Fragment,
    .code           = &(cvk_SpirV){ .ptr = (uint32_t*)examples_shaders_triangle_frag_spv, .len = examples_shaders_triangle_frag_spv_len/sizeof(uint32_t) },
    .allocator      = &instance.allocator,
  });  // clang-format on


  //________________________________________________
  // Initialize: Triangle Pipeline: RenderPass
  cvk_Renderpass pipeline_renderpass = cvk_renderpass_create(&(cvk_renderpass_create_args){
    .device_logical     = &device_logical,
    .allocator          = &instance.allocator,
    .attachment_cfg_ptr = &device_swapchain.attachment_cfg,  // Single attachment for the swapchain image
  });


  //________________________________________________
  // Initialize: Triangle Pipeline: Stages
  VkPipelineShaderStageCreateInfo pipeline_stages[2] = {
    [0] = shader_vert.stage,
    [1] = shader_frag.stage,
  };


  //________________________________________________
  // Initialize: Triangle Pipeline: State
  VkDynamicState const dynamicStates[2] = {
    [0] = VK_DYNAMIC_STATE_VIEWPORT,
    [1] = VK_DYNAMIC_STATE_SCISSOR,
  };
  VkPipelineDynamicStateCreateInfo       state_dynamic               = cvk_pipeline_state_dynamic_setup(2, dynamicStates);
  VkPipelineVertexInputStateCreateInfo   state_vertexInput           = cvk_pipeline_state_vertexInput_setup();
  VkPipelineInputAssemblyStateCreateInfo state_inputAssembly         = cvk_pipeline_state_inputAssembly_setup();
  VkPipelineViewportStateCreateInfo      state_viewport              = cvk_pipeline_state_viewport_setup();
  VkPipelineRasterizationStateCreateInfo state_rasterization         = cvk_pipeline_state_rasterization_setup();
  VkPipelineMultisampleStateCreateInfo   state_multisample           = cvk_pipeline_state_multisample_setup();
  VkPipelineColorBlendAttachmentState    state_colorBlend_attachment = cvk_pipeline_state_colorBlend_attachment_setup();
  // clang-format off
  VkPipelineColorBlendStateCreateInfo    state_colorBlend            = cvk_pipeline_state_colorBlend_setup(&(cvk_pipeline_state_colorBlend_setup_args){
    .attachments_ptr = &state_colorBlend_attachment,
  });  // clang-format on


  //________________________________________________
  // Initialize: Triangle Pipeline: Graphics
  cvk_pipeline_Graphics pipeline_graphics = cvk_pipeline_graphics_create(&(cvk_pipeline_graphics_create_args){
    .state_vertexInput   = &state_vertexInput,
    .state_inputAssembly = &state_inputAssembly,
    .state_tessellation  = NULL, // state_tessellation,
    .state_viewport      = &state_viewport,
    .state_rasterization = &state_rasterization,
    .state_multisample   = &state_multisample,
    .state_depthStencil  = NULL, // state_depthStencil,
    .state_colorBlend    = &state_colorBlend,
    .state_dynamic       = &state_dynamic,
    .renderpass          = &pipeline_renderpass,
    .stages              = &(cvk_pipeline_shaderStage_List){ .ptr = pipeline_stages, .len = 2 },
    .device_logical      = &device_logical,
    .allocator           = &instance.allocator,
  });


  //________________________________________________
  // Initialize: Triangle Framebuffers
  cvk_framebuffer_List device_framebuffers = (cvk_framebuffer_List){
    .len = device_swapchain.images.len,
    .ptr = NULL,
  };
  cvk_Slice framebuffers_tempdata = instance.allocator.cpu.allocZ(&instance.allocator.cpu, device_framebuffers.len, sizeof(*device_framebuffers.ptr));
  device_framebuffers.ptr         = (cvk_Framebuffer*)framebuffers_tempdata.ptr;
  for (cvk_size id = 0; id < device_framebuffers.len; ++id) {
    device_framebuffers.ptr[id] = cvk_framebuffer_create(&(cvk_framebuffer_create_args){
      .device_logical  = &device_logical,
      .allocator       = &instance.allocator,
      .renderpass      = &pipeline_renderpass,
      .size            = &device_swapchain.cfg.imageExtent,
      .attachments_ptr = &device_swapchain.images.ptr[id].view,  // Single attachment for the Swapchain.Image.View
    });
  }


  //________________________________________________
  // Initialize: Triangle Sync Structures
  // clang-format off
  cvk_command_Pool command_pool = cvk_command_pool_create(&(cvk_command_pool_create_args){
    .device_logical = &device_logical,
    .queueID        = device_physical.queueFamilies.graphics,
    .flags          = cvk_command_pool_Reset,
    .allocator      = &instance.allocator,
  });  // clang-format on
  cvk_command_Buffer command_buffer[example_frames_Len];
  cvk_Semaphore      imageAvailable[example_frames_Len];
  cvk_Fence          framesPending[example_frames_Len];
  for (cvk_size id = 0; id < example_frames_Len; ++id) {  // clang-format off
    command_buffer[id] = cvk_command_buffer_allocate(&(cvk_command_buffer_allocate_args){
      .device_logical = &device_logical,
      .command_pool   = &command_pool,
    });
    imageAvailable[id] = cvk_semaphore_create(&device_logical, &instance.allocator);
    framesPending[id]  = cvk_fence_create(&device_logical, /*signaled*/ cvk_true, &instance.allocator);
  }  // clang-format on


  int frameID = 0;
  //________________________________________________
  // Update Loop
  while (!csys_close(&system)) {
    csys_update(&system);  // Update Window+Input

    //____________________________
    // Frame Overview:
    // 1. Wait for the previous frame to finish
    // 2. Get an image from the Swapchain
    // 3. Record a command buffer that draws into the swapchain image
    // 4. Submit the recorded command buffer
    // 5. Present the Swapchain image to the screen
    // 6. Advance to next frame
    //____________________________

    //______________________________________
    // 1. Wait for the previous frame to finish
    cvk_fence_wait(&framesPending[frameID], &device_logical);
    cvk_fence_reset(&framesPending[frameID], &device_logical);

    //______________________________________
    // 2. Get an image from the Swapchain
    cvk_size const imageID = cvk_device_swapchain_nextImageID(&(cvk_device_swapchain_nextImageID_args){
      .device_logical = &device_logical,
      .swapchain      = &device_swapchain,
      .semaphore      = &imageAvailable[frameID],
    });

    //______________________________________
    // 3. Record the Command Buffer
    cvk_command_buffer_reset(&command_buffer[frameID], /* releaseResources */ cvk_false);
    cvk_command_buffer_begin(&command_buffer[frameID]);
    // clang-format off
    cvk_renderpass_command_begin(&pipeline_renderpass, &(cvk_renderpass_command_begin_args){
      .command_buffer = &command_buffer[frameID],
      .framebuffer    = &device_framebuffers.ptr[imageID],
      .extent         = device_swapchain.cfg.imageExtent,
    });  // clang-format on
    cvk_pipeline_graphics_command_bind(&pipeline_graphics, &command_buffer[frameID]);
    // clang-format off
    cvk_viewport_command_set(&(VkViewport){
      .width    = (float)device_swapchain.cfg.imageExtent.width,
      .height   = (float)device_swapchain.cfg.imageExtent.height,
      .maxDepth = 1.0f, // near:0, far:1
    }, &command_buffer[frameID]);  // clang-format on
    // clang-format off
    cvk_scissor_command_set(&(VkRect2D){
      .offset = (VkOffset2D){.x= 0, .y= 0},
      .extent = device_swapchain.cfg.imageExtent,
    }, &command_buffer[frameID]);  // clang-format on
    cvk_command_draw(&command_buffer[frameID], &(cvk_command_draw_args){ 0 });
    cvk_renderpass_command_end(&pipeline_renderpass, &command_buffer[frameID]);
    cvk_command_buffer_end(&command_buffer[frameID]);

    //______________________________________
    // 4. Submit the recorded command buffer
    // clang-format off
    cvk_device_queue_submit(&device_queue, &(cvk_device_queue_submit_args){
      .command_buffer   = &command_buffer[frameID],
      .semaphore_wait   = &imageAvailable[frameID],
      .semaphore_signal = &device_swapchain.images.ptr[imageID].finished,
      .fence            = &framesPending[frameID],
    });  // clang-format on

    //______________________________________
    // 5. Present the Swapchain image to the screen
    //    @note: Called `vkQueuePresentKHR` by Vulkan
    cvk_device_swapchain_present(&device_swapchain, imageID, &device_queue);

    //______________________________________
    // 6. Advance to next frame
    frameID = (frameID + 1) % example_frames_Len;
  }


  //________________________________________________
  // Terminate: GPU Data
  cvk_device_logical_wait(&device_logical);
  // Destroy example.Sync
  for (cvk_size id = 0; id < example_frames_Len; ++id) {  // clang-format off
    cvk_semaphore_destroy(&imageAvailable[id], &device_logical, &instance.allocator);
    cvk_fence_destroy(&framesPending[id], &device_logical, &instance.allocator);
  }  // clang-format on
  cvk_command_pool_destroy(&command_pool, &device_logical, &instance.allocator);
  // Destroy example.Pipeline
  cvk_framebuffer_list_destroy(&device_framebuffers, &device_logical, &instance.allocator);
  cvk_pipeline_graphics_destroy(&pipeline_graphics, &device_logical, &instance.allocator);
  cvk_renderpass_destroy(&pipeline_renderpass, &device_logical, &instance.allocator);
  // Destroy example.Shaders
  cvk_shader_destroy(&shader_frag, &device_logical, &instance.allocator);
  cvk_shader_destroy(&shader_vert, &device_logical, &instance.allocator);
  // Destroy example.GPU
  cvk_device_swapchain_destroy(&device_swapchain, &device_logical, &instance.allocator);
  cvk_device_logical_destroy(&device_logical, &instance.allocator);
  cvk_device_queue_destroy(&device_queue, &instance.allocator);
  cvk_device_physical_destroy(&device_physical, &instance.allocator);
  vkDestroySurfaceKHR(instance.ct, surface, instance.allocator.gpu);
  cvk_instance_destroy(&instance);


  //________________________________________________
  // Terminate: Window
  csys_term(&system);
  return 0;
}

