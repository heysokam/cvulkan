//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
// @deps cdk
#define GLFW_INCLUDE_VULKAN
#define csys_Implementation
#include <csys.h>
// @deps cvulkan
#define cvk_Implementation
#include <cvulkan.h>

extern unsigned char examples_shaders_triangle_frag_spv[];
extern unsigned int  examples_shaders_triangle_frag_spv_len;
#include "./shaders/triangle.frag.c"

extern unsigned char examples_shaders_triangle_vert_spv[];
extern unsigned int  examples_shaders_triangle_vert_spv_len;
#include "./shaders/triangle.vert.c"

typedef struct {
  cvk_Instance         instance;
  cvk_Surface          surface;
  cvk_device_Physical  device_physical;
  cvk_device_Queue     device_queue;
  cvk_device_Logical   device_logical;
  cvk_device_Swapchain device_swapchain;
  cvk_framebuffer_List device_framebuffers;
} GPU;

typedef struct example_Pipeline {
  cvk_pipeline_Graphics           graphics;
  VkPipelineShaderStageCreateInfo stages[2];
  cvk_Renderpass                  renderpass;
} example_Pipeline;

typedef struct example_Sync {
  cvk_command_Pool   command_pool;
  cvk_command_Buffer command_buffer;
  cvk_Semaphore      imageAvailable;
  cvk_Semaphore      renderFinished;
  cvk_Fence          framesPending;
} example_Sync;

typedef struct Example {
  GPU gpu;
  struct {
    cvk_Shader vert;
    cvk_Shader frag;
  } shader;
  example_Pipeline pipeline;
  example_Sync     sync;
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


static example_Pipeline example_pipeline_create (
  GPU* const        gpu,
  cvk_Shader* const vert,
  cvk_Shader* const frag
) {
  example_Pipeline result = (example_Pipeline){ 0 };

  //________________________________________________
  // TODO: cvk_pipeline_state_dynamic_create()
  VkDynamicState const dynamicStates[2] = {
    [0] = VK_DYNAMIC_STATE_VIEWPORT,
    [1] = VK_DYNAMIC_STATE_SCISSOR,
  };
  VkPipelineDynamicStateCreateInfo state_dynamic = (VkPipelineDynamicStateCreateInfo){
    .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    .pNext             = NULL,
    .flags             = (VkPipelineDynamicStateCreateFlags)0,  // Vk.Spec -> Reserved for future use
    .dynamicStateCount = 2,
    .pDynamicStates    = dynamicStates,
  };

  //________________________________________________
  // TODO: cvk_pipeline_state_vertexInput_create()
  VkPipelineVertexInputStateCreateInfo state_vertexInput = (VkPipelineVertexInputStateCreateInfo){
    .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .pNext                           = NULL,
    .flags                           = /* Vk.Spec -> Reserved for future use */ (VkPipelineVertexInputStateCreateFlags)0,
    .vertexBindingDescriptionCount   = 0,     // TODO: Configurable
    .pVertexBindingDescriptions      = NULL,  // TODO: Configurable
    .vertexAttributeDescriptionCount = 0,     // TODO: Configurable
    .pVertexAttributeDescriptions    = NULL,  // TODO: Configurable
  };

  //________________________________________________
  // TODO: cvk_pipeline_state_inputAssembly_create()
  VkPipelineInputAssemblyStateCreateInfo state_inputAssembly = (VkPipelineInputAssemblyStateCreateInfo){
    .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .pNext                  = NULL,
    .flags                  = /* Vk.Spec -> Reserved for future use */ (VkPipelineInputAssemblyStateCreateFlags)0,
    .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,  // TODO: Configurable
    .primitiveRestartEnable = VK_FALSE,                             // TODO: Configurable
  };

  //________________________________________________
  // TODO: cvk_pipeline_state_viewport_create()
  VkPipelineViewportStateCreateInfo state_viewport = (VkPipelineViewportStateCreateInfo){
    .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .pNext         = NULL,
    .flags         = /* Vk.Spec -> Reserved for future use */ (VkPipelineViewportStateCreateFlags)0,
    .viewportCount = 1,     // TODO: Configurable.   Current: Viewport is Dynamic State
    .pViewports    = NULL,  // TODO: NULL because Viewport is Dynamic State
    .scissorCount  = 1,     // TODO: Configurable.   Current: Viewport.Scissor is Dynamic State
    .pScissors     = NULL,  // TODO: NULL because Viewport.Scissor is Dynamic State
  };

  //________________________________________________
  // TODO: cvk_pipeline_state_rasterization_create()
  VkPipelineRasterizationStateCreateInfo state_rasterization = (VkPipelineRasterizationStateCreateInfo){
    .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .pNext                   = NULL,
    .flags                   = /* Vk.Spec -> Reserved for future use */ (VkPipelineRasterizationStateCreateFlags)0,
    .depthClampEnable        = VK_FALSE,                 // TODO: Configurable (niche). Requires Device.Feature
    .rasterizerDiscardEnable = VK_FALSE,                 // TODO: Does this need to be configurable at all ?? Basically disables framebuffer output
    .polygonMode             = VK_POLYGON_MODE_FILL,     // TODO: Configurable (niche-ish). Requires Device.Feature
    .lineWidth               = 1.0f,                     // TODO: Configurable (niche). >1.0 requires Device.Feature for widelines
    .cullMode                = VK_CULL_MODE_BACK_BIT,    // TODO: Configurable
    .frontFace               = VK_FRONT_FACE_CLOCKWISE,  // TODO: Configurable
    .depthBiasEnable         = VK_FALSE,                 // TODO: Configurable (niche). Useful for ShadowMapping
    .depthBiasConstantFactor = 0.0f,                     // TODO: Configurable (niche). Useful for ShadowMapping
    .depthBiasClamp          = 0.0f,                     // TODO: Configurable (niche). Useful for ShadowMapping
    .depthBiasSlopeFactor    = 0.0f,                     // TODO: Configurable (niche). Useful for ShadowMapping
  };

  //________________________________________________
  // TODO: cvk_pipeline_state_multisample_create()
  //     : for MSAA antialiasing
  VkPipelineMultisampleStateCreateInfo state_multisample = (VkPipelineMultisampleStateCreateInfo){
    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = /* Vk.Spec -> Reserved for future use */ (VkPipelineMultisampleStateCreateFlags)0,
    // TODO: Configurable
    //     : Set to disabled for now.
    .sampleShadingEnable   = VK_FALSE,
    .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
    .minSampleShading      = 1.0f,
    .pSampleMask           = NULL,
    .alphaToCoverageEnable = VK_FALSE,
    .alphaToOneEnable      = VK_FALSE,
  };

  //________________________________________________
  // TODO: cvk_pipeline_state_colorBlend_create()
  // Attachment State
  // clang-format off
  VkPipelineColorBlendStateCreateInfo state_colorBlend = (VkPipelineColorBlendStateCreateInfo){
    .sType                 = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .pNext                 = NULL,
    .flags                 = (VkPipelineColorBlendStateCreateFlags)0,  // TODO: RASTERIZATION_ORDER_ATTACHMENT_ACCESS extension
      // TODO: Configurable. true for Bitwise combination blending method
      .logicOpEnable       = VK_FALSE,
      .logicOp             = VK_LOGIC_OP_COPY,  // TODO: Configurable. Bitwise Operator for blending
      .attachmentCount     = 1,
      .pAttachments        = &(VkPipelineColorBlendAttachmentState){
      .blendEnable         = VK_TRUE,                              // TODO: Configurable
      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,            // TODO: Configurable
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,  // TODO: Configurable
      .colorBlendOp        = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp        = VK_BLEND_OP_ADD,
      .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    },
    // TODO: Do these need to be configurable ?
    .blendConstants[0]     = 0.0f,
    .blendConstants[1]     = 0.0f,
    .blendConstants[2]     = 0.0f,
    .blendConstants[3]     = 0.0f,
  };  // clang-format on

  //________________________________________________
  // TODO: cvk_pipeline_state_depthStencil_create()
  //     : NULL, disabled
  // VkPipelineDepthStencilStateCreateInfo depthStencil = (VkPipelineDepthStencilStateCreateInfo){ 0 };
  VkPipelineDepthStencilStateCreateInfo* state_depthStencil = NULL;
  //________________________________________________
  // TODO: cvk_pipeline_state_tessellation_create()
  VkPipelineTessellationStateCreateInfo* state_tessellation = NULL;

  //________________________________________________
  result.renderpass = cvk_renderpass_create(&(cvk_renderpass_create_args){
    .device_logical = &gpu->device_logical,
    .swapchain      = &gpu->device_swapchain,
    .allocator      = &gpu->instance.allocator,
  });

  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  // TODO: Configurable
  result.stages[0] = vert->stage;
  result.stages[1] = frag->stage;
  #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  // clang-format on

  result.graphics = cvk_pipeline_graphics_create(&(cvk_pipeline_graphics_create_args){
    .state_vertexInput   = &state_vertexInput,
    .state_inputAssembly = &state_inputAssembly,
    .state_tessellation  = state_tessellation,
    .state_viewport      = &state_viewport,
    .state_rasterization = &state_rasterization,
    .state_multisample   = &state_multisample,
    .state_depthStencil  = state_depthStencil,
    .state_colorBlend    = &state_colorBlend,
    .state_dynamic       = &state_dynamic,
    .renderpass          = &result.renderpass,
    .stages              = &(cvk_pipeline_shaderStage_List){ .ptr = result.stages, .len = 2 },
    .device_logical      = &gpu->device_logical,
    .allocator           = &gpu->instance.allocator,
  });

  //________________________________________________
  // Cleanup and Return the Pipeline Object
  return result;
}


static void example_pipeline_destroy (
  example_Pipeline* const         pipeline,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  cvk_pipeline_graphics_destroy(&pipeline->graphics, device_logical, allocator);
  cvk_renderpass_destroy(&pipeline->renderpass, device_logical, allocator);
}


static cvk_Pure example_Sync example_sync_create (
  GPU* const gpu
) {
  example_Sync result = (example_Sync){
    .command_pool = cvk_command_pool_create(&(cvk_command_pool_create_args){
      .device_logical = &gpu->device_logical,
      .queueID        = gpu->device_physical.queueFamilies.graphics,
      .flags          = cvk_command_pool_Reset,
      .allocator      = &gpu->instance.allocator,
    }),
  };
  result.command_buffer = cvk_command_buffer_allocate(&(cvk_command_buffer_allocate_args){
    .device_logical = &gpu->device_logical,
    .pool           = &result.command_pool,
  });
  result.imageAvailable = cvk_semaphore_create(&gpu->device_logical, &gpu->instance.allocator);
  result.renderFinished = cvk_semaphore_create(&gpu->device_logical, &gpu->instance.allocator);
  result.framesPending  = cvk_fence_create(&gpu->device_logical, /*signaled*/ cvk_true, &gpu->instance.allocator);
  return result;
}


static void example_sync_destroy (
  example_Sync* const             sync,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  cvk_semaphore_destroy(&sync->imageAvailable, device_logical, allocator);
  cvk_semaphore_destroy(&sync->renderFinished, device_logical, allocator);
  cvk_fence_destroy(&sync->framesPending, device_logical, allocator);
  cvk_command_pool_destroy(&sync->command_pool, device_logical, allocator);
}


static Example example_create (
  csys_System const* const system,
  cvk_SpirV const* const   vert,
  cvk_SpirV const* const   frag
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
  result.pipeline    = example_pipeline_create(&result.gpu, &result.shader.vert, &result.shader.frag);

  result.gpu.device_framebuffers = cvk_device_swapchain_framebuffers_create(&(cvk_device_swapchain_framebuffers_create_args){
    .swapchain      = &result.gpu.device_swapchain,
    .device_logical = &result.gpu.device_logical,
    .renderpass     = &result.pipeline.renderpass,
    .allocator      = &result.gpu.instance.allocator,
  });

  result.sync = example_sync_create(&result.gpu);
  return result;
}


static void example_destroy (
  Example* const example
) {
  example_sync_destroy(&example->sync, &example->gpu.device_logical, &example->gpu.instance.allocator);
  cvk_framebuffer_list_destroy(&example->gpu.device_framebuffers, &example->gpu.device_logical, &example->gpu.instance.allocator);
  example_pipeline_destroy(&example->pipeline, &example->gpu.device_logical, &example->gpu.instance.allocator);
  cvk_shader_destroy(&example->gpu.device_logical, &example->shader.frag, &example->gpu.instance.allocator);
  cvk_shader_destroy(&example->gpu.device_logical, &example->shader.vert, &example->gpu.instance.allocator);
  example_gpu_destroy(&example->gpu);
}


static void example_update (
  Example* const example
) {
  //____________________________
  // Frame Overview:
  // 1. Wait for the previous frame to finish
  // 2. Get an image from the Swapchain
  // 3. Record a command buffer that draws into the swapchain image
  // 4. Submit the recorded command buffer
  // 5. Present the Swapchain image to the screen
  //____________________________

  //______________________________________
  // 1. Wait for the previous frame to finish
  cvk_fence_wait(&example->sync.framesPending, &example->gpu.device_logical);
  cvk_fence_reset(&example->sync.framesPending, &example->gpu.device_logical);

  //______________________________________
  // 2. Get an image from the Swapchain
  cvk_size const imageID = cvk_device_swapchain_nextImageID(&(cvk_device_swapchain_nextImageID_args){
    .device_logical = &example->gpu.device_logical,
    .swapchain      = &example->gpu.device_swapchain,
    .semaphore      = &example->sync.imageAvailable,
  });

  //______________________________________
  // 3. Record the Command Buffer
  cvk_command_buffer_reset(&example->sync.command_buffer, /* releaseResources */ cvk_false);
  cvk_command_buffer_begin(&example->sync.command_buffer);  // clang-format off
  // TODO: cvk_command_renderpass_begin()
  vkCmdBeginRenderPass(example->sync.command_buffer.ct, &(VkRenderPassBeginInfo){
    .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext           = NULL,
    .renderPass      = example->pipeline.renderpass.ct,
    .framebuffer     = example->gpu.device_framebuffers.ptr[imageID].ct,
    .renderArea      = { .offset = { 0, 0 }, .extent = example->gpu.device_swapchain.cfg.imageExtent },
    .clearValueCount = 1,
    .pClearValues    = &(VkClearValue){ .color = { .float32 = { [0] = 0.222f, [1] = 0.333f, [2] = 0.444f, [3] = 1.0f } } },
  }, VK_SUBPASS_CONTENTS_INLINE);  // clang-format on
  // TODO: cvk_command_pipeline_bind()
  vkCmdBindPipeline(example->sync.command_buffer.ct, VK_PIPELINE_BIND_POINT_GRAPHICS, example->pipeline.graphics.ct);
  //____________________________
  // TODO: cvk_command_viewport_set()
  // clang-format off
  vkCmdSetViewport(example->sync.command_buffer.ct, 0, 1, &(VkViewport){
    .x        = 0.0f,
    .y        = 0.0f,
    .width    = (float)example->gpu.device_swapchain.cfg.imageExtent.width,
    .height   = (float)example->gpu.device_swapchain.cfg.imageExtent.height,
    .minDepth = /* range[0..1] */ 0.0f,  // TODO: Configurable Reverse Depth
    .maxDepth = /* range[0..1] */ 1.0f,  // TODO: Configurable Reverse Depth
  });  // clang-format on
  //____________________________
  // TODO: cvk_command_scissor_set()
  // clang-format off
  vkCmdSetScissor(example->sync.command_buffer.ct, 0, 1, &(VkRect2D){
    .offset = (VkOffset2D){.x= 0, .y= 0},
    .extent = example->gpu.device_swapchain.cfg.imageExtent,
  });  // clang-format on
  //____________________________
  // TODO: cvk_command_draw()
  vkCmdDraw(example->sync.command_buffer.ct, 3, 1, 0, 0);
  //____________________________
  // TODO: cvk_command_renderpass_end()
  vkCmdEndRenderPass(example->sync.command_buffer.ct);
  cvk_command_buffer_end(&example->sync.command_buffer);

  //______________________________________
  // 4. Submit the recorded command buffer
  // TODO: cvk_device_queue_submit()
  // clang-format off
  cvk_result_check(vkQueueSubmit(example->gpu.device_queue.ct, 1, &(VkSubmitInfo ){
    .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext                = NULL,
    .waitSemaphoreCount   = 1,
    .pWaitSemaphores      = &example->sync.imageAvailable.ct,
    .pWaitDstStageMask    = &(VkPipelineStageFlags){VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
    .commandBufferCount   = 1,
    .pCommandBuffers      = &example->sync.command_buffer.ct,
    .signalSemaphoreCount = 1,
    .pSignalSemaphores    = &example->sync.renderFinished.ct,
  }, example->sync.framesPending.ct), "Failed to submit the Example's Command Buffer to the Graphics Queue");  // clang-format on

  //______________________________________
  // 5. Present the Swapchain image to the screen
  // TODO: cvk_device_queue_present()
  // clang-format off
  cvk_result_check(vkQueuePresentKHR(example->gpu.device_queue.ct, &(VkPresentInfoKHR){
    .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .pNext              = NULL,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores    = &example->sync.renderFinished.ct,
    .swapchainCount     = 1,
    .pSwapchains        = &example->gpu.device_swapchain.ct,
    .pImageIndices      = (uint32_t const*)&imageID,
    .pResults           = NULL, // todo: How to deal with multiple swapchains. This returns their VkResult[]
  }), "Failed when presenting the Graphics Commands with the given Queue");  // clang-format on
}

int main () {
  // Initialize
  csys_System system  = csys_init(csys_init_Options_defaults());
  Example     example = example_create(
    /* system */ &system,  // clang-format off
    /* vert   */ &(cvk_SpirV){ .ptr = (uint32_t*)examples_shaders_triangle_vert_spv, .len = examples_shaders_triangle_vert_spv_len/sizeof(uint32_t), .itemsize= sizeof(uint32_t) },
    /* frag   */ &(cvk_SpirV){ .ptr = (uint32_t*)examples_shaders_triangle_frag_spv, .len = examples_shaders_triangle_frag_spv_len/sizeof(uint32_t), .itemsize= sizeof(uint32_t) }
  );  // clang-format on
  // Update Loop
  while (!csys_close(&system)) {
    csys_update(&system);
    example_update(&example);
  }
  // Terminate
  example_destroy(&example);
  csys_term(&system);
  return 0;
}

