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

//______________________________________
// SpirV shaders used by this example
extern unsigned char examples_shaders_triangle_buffered_frag_spv[];
extern unsigned int  examples_shaders_triangle_buffered_frag_spv_len;
#include "./shaders/triangle_buffered.frag.c"
//____________________________
extern unsigned char examples_shaders_triangle_buffered_vert_spv[];
extern unsigned int  examples_shaders_triangle_buffered_vert_spv_len;
#include "./shaders/triangle_buffered.vert.c"
//______________________________________


//______________________________________
// Generic boilerplate shared by all Examples
#include "./helpers/bootstrap.c"
#include "./helpers/math.c"
//______________________________________


typedef struct example_Pipeline {
  cvk_pipeline_Graphics           graphics;
  VkPipelineShaderStageCreateInfo stages[2];
  cvk_Renderpass                  renderpass;
} example_Pipeline;
enum { example_frames_Len = 2 };
typedef struct example_Sync {
  cvk_command_Pool   command_pool;
  cvk_command_Buffer command_buffer[example_frames_Len];
  cvk_Semaphore      imageAvailable[example_frames_Len];
  cvk_Fence          framesPending[example_frames_Len];
} example_Sync;

typedef struct example_Buffer {
  cvk_Buffer ram;
  cvk_Buffer vram;
} example_Buffer;
typedef struct example_Memory {
  cvk_Memory ram;
  cvk_Memory vram;
} example_Memory;
typedef struct example_Vertices {
  example_Buffer                    buffer;
  VkVertexInputBindingDescription   binding;
  VkVertexInputAttributeDescription attributes[2];
  char                              priv_pad[4];
  example_Memory                    memory;
} example_Vertices;
typedef struct example_Indices {
  // @note
  // There is no need to keep the staging buffer around.
  // Keeping it until app termination just for simplicity of the code
  example_Memory memory;
  example_Buffer buffer;
} example_Indices;

typedef struct Example {
  example_Bootstrap gpu;
  struct {
    cvk_Shader vert;
    cvk_Shader frag;
  } shader;
  example_Pipeline     pipeline;
  cvk_framebuffer_List device_framebuffers;
  example_Sync         sync;
  cvk_bool             resized;
  char                 priv_pad[4];
  example_Vertices     verts;
  example_Indices      inds;
} Example;


//______________________________________
// TODO: Move to cvulkan
//____________________________
static VkVertexInputBindingDescription cvk_vertex_binding_description (
  uint32_t const slot,
  uint32_t const stride,
  cvk_bool const instanced
) {
  return (VkVertexInputBindingDescription){
    .inputRate = instanced ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX,
    .binding   = slot,
    .stride    = stride,
  };
}
//__________________
//______________________________________


static example_Vertices example_verts_create (
  example_Bootstrap* const      gpu,
  cvk_command_Pool const* const command_pool
) {
  example_Vertices result       = (example_Vertices){ 0 };
  uint32_t const   verts_len    = 4;
  Vertex           verts_ptr[4] = {
    // clang-format off
    [0]= (Vertex){.pos= (Vec2){.x= -0.5f, .y= -0.5f}, .color= (Vec3){.r= 1.0f, .g= 0.0f, .b= 0.0f}},
    [1]= (Vertex){.pos= (Vec2){.x=  0.5f, .y= -0.5f}, .color= (Vec3){.r= 0.0f, .g= 1.0f, .b= 0.0f}},
    [2]= (Vertex){.pos= (Vec2){.x=  0.5f, .y=  0.5f}, .color= (Vec3){.r= 0.0f, .g= 0.0f, .b= 1.0f}},
    [3]= (Vertex){.pos= (Vec2){.x= -0.5f, .y=  0.5f}, .color= (Vec3){.r= 1.0f, .g= 1.0f, .b= 1.0f}},
  };
  result.buffer.ram = cvk_buffer_create(&(cvk_buffer_create_args){
    .device_physical = &gpu->device_physical,
    .device_logical  = &gpu->device_logical,
    .usage           = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    .size            = sizeof(verts_ptr[0]) * verts_len,
    .memory_flags    = cvk_memory_HostVisible | cvk_memory_HostCoherent,
    .allocator       = &gpu->instance.allocator,
  });
  result.buffer.vram = cvk_buffer_create(&(cvk_buffer_create_args){
    .device_physical = &gpu->device_physical,
    .device_logical  = &gpu->device_logical,
    .usage           = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    .size            = sizeof(verts_ptr[0]) * verts_len,
    .memory_flags    = cvk_memory_DeviceLocal,
    .allocator       = &gpu->instance.allocator,
  });
  result.memory.ram = cvk_memory_create(&(cvk_memory_create_args){
    .device_logical = &gpu->device_logical,
    .data           = (void*)verts_ptr,
    .kind           = result.buffer.ram.memory.kind,
    .size_alloc     = result.buffer.ram.memory.requirements.size,
    .size_data      = result.buffer.ram.cfg.size,
    .allocator      = &gpu->instance.allocator,
  });
  result.memory.vram = cvk_memory_create(&(cvk_memory_create_args){
    .device_logical = &gpu->device_logical,
    .kind           = result.buffer.vram.memory.kind,
    .size_alloc     = result.buffer.vram.memory.requirements.size,
    .size_data      = result.buffer.vram.cfg.size,
    .allocator      = &gpu->instance.allocator,
  });
  cvk_buffer_bind(&result.buffer.ram, &(cvk_buffer_bind_args){
    .device_logical = &gpu->device_logical,
    .memory         = &result.memory.ram,
  });
  cvk_buffer_bind(&result.buffer.vram, &(cvk_buffer_bind_args){
    .device_logical = &gpu->device_logical,
    .memory         = &result.memory.vram,
  });
  cvk_buffer_copy(&result.buffer.ram, &result.buffer.vram, &(cvk_buffer_copy_args){
    .device_logical = &gpu->device_logical,
    .device_queue   = &gpu->device_queue,
    .pool           = command_pool,
  });  // clang-format on
  result.binding = cvk_vertex_binding_description(0, sizeof(Vertex), cvk_false);
  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  result.attributes[0] = (VkVertexInputAttributeDescription){ 0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, pos) };
  result.attributes[1] = (VkVertexInputAttributeDescription){ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) };
  #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  // clang-format on
  return result;
}


static void example_verts_destroy (
  example_Vertices* const  verts,
  example_Bootstrap* const gpu
) {
  verts->binding = (VkVertexInputBindingDescription){ 0 };
  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  verts->attributes[0] = (VkVertexInputAttributeDescription){ 0 };
  verts->attributes[1] = (VkVertexInputAttributeDescription){ 0 };
  #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  // clang-format on
  cvk_buffer_destroy(&verts->buffer.ram, &gpu->device_logical, &gpu->instance.allocator);
  cvk_buffer_destroy(&verts->buffer.vram, &gpu->device_logical, &gpu->instance.allocator);
  cvk_memory_destroy(&verts->memory.ram, &gpu->device_logical, &gpu->instance.allocator);
  cvk_memory_destroy(&verts->memory.vram, &gpu->device_logical, &gpu->instance.allocator);
}


static example_Indices example_inds_create (
  example_Bootstrap* const      gpu,
  cvk_command_Pool const* const command_pool
) {
  example_Indices result = (example_Indices){ 0 };

  uint32_t const inds_len    = 6;
  uint16_t       inds_ptr[6] = { /* tri0 */ 0, 1, 2, /* tri1 */ 2, 3, 0 };

  result.buffer.ram  = cvk_buffer_create(&(cvk_buffer_create_args){
     .device_physical = &gpu->device_physical,
     .device_logical  = &gpu->device_logical,
     .usage           = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
     .size            = sizeof(inds_ptr[0]) * inds_len,
     .memory_flags    = cvk_memory_HostVisible | cvk_memory_HostCoherent,
     .allocator       = &gpu->instance.allocator,
  });
  result.buffer.vram = cvk_buffer_create(&(cvk_buffer_create_args){
    .device_physical = &gpu->device_physical,
    .device_logical  = &gpu->device_logical,
    .usage           = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    .size            = sizeof(inds_ptr[0]) * inds_len,
    .memory_flags    = cvk_memory_DeviceLocal,
    .allocator       = &gpu->instance.allocator,
  });
  result.memory.ram  = cvk_memory_create(&(cvk_memory_create_args){
     .device_logical = &gpu->device_logical,
     .data           = (void*)inds_ptr,
     .kind           = result.buffer.ram.memory.kind,
     .size_alloc     = result.buffer.ram.memory.requirements.size,
     .size_data      = result.buffer.ram.cfg.size,
     .allocator      = &gpu->instance.allocator,
  });
  result.memory.vram = cvk_memory_create(&(cvk_memory_create_args){
    .device_logical = &gpu->device_logical,
    .kind           = result.buffer.vram.memory.kind,
    .size_alloc     = result.buffer.vram.memory.requirements.size,
    .size_data      = result.buffer.vram.cfg.size,
    .allocator      = &gpu->instance.allocator,
  });  // clang-format off
  cvk_buffer_bind(&result.buffer.ram, &(cvk_buffer_bind_args){
    .device_logical = &gpu->device_logical,
    .memory         = &result.memory.ram,
  });
  cvk_buffer_bind(&result.buffer.vram, &(cvk_buffer_bind_args){
    .device_logical = &gpu->device_logical,
    .memory         = &result.memory.vram,
  });
  cvk_buffer_copy(&result.buffer.ram, &result.buffer.vram, &(cvk_buffer_copy_args){
    .device_logical = &gpu->device_logical,
    .device_queue   = &gpu->device_queue,
    .pool           = command_pool,
  });  // clang-format on

  return result;
}

static void example_inds_destroy (
  example_Indices* const   inds,
  example_Bootstrap* const gpu
) {
  // inds->binding = (VkVertexInputBindingDescription){ 0 };
  // clang-format off
  // #pragma GCC diagnostic push
  // #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  // inds->attributes[0] = (VkVertexInputAttributeDescription){ 0 };
  // inds->attributes[1] = (VkVertexInputAttributeDescription){ 0 };
  // #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  // clang-format on
  cvk_buffer_destroy(&inds->buffer.ram, &gpu->device_logical, &gpu->instance.allocator);
  cvk_buffer_destroy(&inds->buffer.vram, &gpu->device_logical, &gpu->instance.allocator);
  cvk_memory_destroy(&inds->memory.ram, &gpu->device_logical, &gpu->instance.allocator);
  cvk_memory_destroy(&inds->memory.vram, &gpu->device_logical, &gpu->instance.allocator);
}


static example_Pipeline example_pipeline_create (
  example_Bootstrap* const      gpu,
  example_Vertices const* const verts,
  example_Indices const* const  inds,
  cvk_Shader* const             vert,
  cvk_Shader* const             frag
) {
  example_Pipeline result = (example_Pipeline){ 0 };

  cvk_discard(inds);  // FIX: Remove

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
    .vertexBindingDescriptionCount   = 1,                  // TODO: Configurable
    .pVertexBindingDescriptions      = &verts->binding,    // TODO: Configurable
    .vertexAttributeDescriptionCount = 2,                  // TODO: Configurable
    .pVertexAttributeDescriptions    = verts->attributes,  // TODO: Configurable
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
  example_Bootstrap* const gpu
) {
  example_Sync result = (example_Sync){
    .command_pool = cvk_command_pool_create(&(cvk_command_pool_create_args){
      .device_logical = &gpu->device_logical,
      .queueID        = gpu->device_physical.queueFamilies.graphics,
      .flags          = cvk_command_pool_Reset,
      .allocator      = &gpu->instance.allocator,
    }),
  };
  for (cvk_size id = 0; id < example_frames_Len; ++id) {  // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    result.command_buffer[id] = cvk_command_buffer_allocate(&(cvk_command_buffer_allocate_args){
      .device_logical = &gpu->device_logical,
      .pool           = &result.command_pool,
    });
    result.imageAvailable[id] = cvk_semaphore_create(&gpu->device_logical, &gpu->instance.allocator);
    result.framesPending[id]  = cvk_fence_create(&gpu->device_logical, /*signaled*/ cvk_true, &gpu->instance.allocator);
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  }  // clang-format on
  return result;
}


static void example_sync_destroy (
  example_Sync* const             sync,
  cvk_device_Logical const* const device_logical,
  cvk_Allocator* const            allocator
) {
  for (cvk_size id = 0; id < example_frames_Len; ++id) {  // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    cvk_semaphore_destroy(&sync->imageAvailable[id], device_logical, allocator);
    cvk_fence_destroy(&sync->framesPending[id], device_logical, allocator);
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  }  // clang-format on
  cvk_command_pool_destroy(&sync->command_pool, device_logical, allocator);
}


static Example example_create (
  csys_System const* const system,
  cvk_SpirV const* const   vert,
  cvk_SpirV const* const   frag
) {
  Example result     = (Example){ 0 };
  result.gpu         = example_bootstrap_create(system->window.ct, (cvk_Size2D){ .width = system->window.width, .height = system->window.height });
  result.sync        = example_sync_create(&result.gpu);
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
  result.verts       = example_verts_create(&result.gpu, &result.sync.command_pool);
  result.inds        = example_inds_create(&result.gpu, &result.sync.command_pool);
  result.pipeline    = example_pipeline_create(&result.gpu, &result.verts, &result.inds, &result.shader.vert, &result.shader.frag);

  result.device_framebuffers = cvk_device_swapchain_framebuffers_create(&(cvk_device_swapchain_framebuffers_create_args){
    .swapchain      = &result.gpu.device_swapchain,
    .device_logical = &result.gpu.device_logical,
    .renderpass     = &result.pipeline.renderpass,
    .allocator      = &result.gpu.instance.allocator,
  });
  return result;
}


static void example_destroy (
  Example* const example
) {
  cvk_device_logical_wait(&example->gpu.device_logical);
  example_inds_destroy(&example->inds, &example->gpu);
  example_verts_destroy(&example->verts, &example->gpu);
  example_sync_destroy(&example->sync, &example->gpu.device_logical, &example->gpu.instance.allocator);
  cvk_framebuffer_list_destroy(&example->device_framebuffers, &example->gpu.device_logical, &example->gpu.instance.allocator);
  example_pipeline_destroy(&example->pipeline, &example->gpu.device_logical, &example->gpu.instance.allocator);
  cvk_shader_destroy(&example->gpu.device_logical, &example->shader.frag, &example->gpu.instance.allocator);
  cvk_shader_destroy(&example->gpu.device_logical, &example->shader.vert, &example->gpu.instance.allocator);
  example_bootstrap_destroy(&example->gpu);
}


static void example_swapchain_recreate (
  Example* const           example,
  csys_System const* const system
) {
  //__________________________________________________________________
  // FIX:
  // Resizing is broken.
  // Something about the Queue and Semaphores is not quite correct
  //__________________________________________________________________
  // Get the new size
  int newSize_width  = 0;
  int newSize_height = 0;
  glfwGetFramebufferSize(system->window.ct, &newSize_width, &newSize_height);
  // Pause until the window exits minimize mode (size 0,0)
  while (!newSize_width || !newSize_height) {
    glfwGetFramebufferSize(system->window.ct, &newSize_width, &newSize_height);
    glfwWaitEvents();
  }
  // Recreate the data
  example->gpu.device_swapchain.cfg.imageExtent = (cvk_Size2D){ .width = (uint32_t)newSize_width, .height = (uint32_t)newSize_height };
  cvk_framebuffer_list_destroy(&example->device_framebuffers, &example->gpu.device_logical, &example->gpu.instance.allocator);
  // clang-format off
  cvk_device_swapchain_recreate(&example->gpu.device_swapchain, &(cvk_device_swapchain_recreate_args){
    .device_logical = &example->gpu.device_logical,
    .allocator      = &example->gpu.instance.allocator,
  });  // clang-format on
  example->device_framebuffers = cvk_device_swapchain_framebuffers_create(&(cvk_device_swapchain_framebuffers_create_args){
    .swapchain      = &example->gpu.device_swapchain,
    .device_logical = &example->gpu.device_logical,
    .renderpass     = &example->pipeline.renderpass,
    .allocator      = &example->gpu.instance.allocator,
  });
}


static int frameID = 0;

static void example_update (
  Example* const           example,
  csys_System const* const system
) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
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
  cvk_fence_wait(&example->sync.framesPending[frameID], &example->gpu.device_logical);

  //______________________________________
  // 2. Get an image from the Swapchain
  VkResult       status  = VK_RESULT_MAX_ENUM;
  cvk_size const imageID = cvk_device_swapchain_nextImageID(&(cvk_device_swapchain_nextImageID_args){
    .device_logical = &example->gpu.device_logical,
    .swapchain      = &example->gpu.device_swapchain,
    .semaphore      = &example->sync.imageAvailable[frameID],
    .status         = &status,
  });
  if (example->resized || status == VK_ERROR_OUT_OF_DATE_KHR || status == VK_SUBOPTIMAL_KHR) {
    cvk_print("--------------> Swapchain needs to be recreated.\n");
    example->resized = false;
    example_swapchain_recreate(example, system);
    return;
  } else if (status) cvk_result_check(status, "Something went wrong when requesting the Swapchain.nextImageID");

  //______________________________________
  // 1.2. Only reset the frame if we are submitting work
  cvk_fence_reset(&example->sync.framesPending[frameID], &example->gpu.device_logical);


  //______________________________________
  // 3. Record the Command Buffer
  cvk_command_buffer_reset(&example->sync.command_buffer[frameID], /* releaseResources */ cvk_false);
  cvk_command_buffer_begin(&example->sync.command_buffer[frameID]);  // clang-format off
  cvk_renderpass_command_begin(&example->pipeline.renderpass, &(cvk_renderpass_command_begin_args){
    .command_buffer = &example->sync.command_buffer[frameID],
    .framebuffer    = &example->device_framebuffers.ptr[imageID],
    .extent         = example->gpu.device_swapchain.cfg.imageExtent,
  });  // clang-format on
  cvk_pipeline_graphics_command_bind(&example->pipeline.graphics, &example->sync.command_buffer[frameID]);
  cvk_buffer_vertex_command_bind(&example->verts.buffer.vram, &example->sync.command_buffer[frameID]);
  cvk_buffer_index_command_bind(&example->inds.buffer.vram, &example->sync.command_buffer[frameID]);
  // clang-format off
  cvk_viewport_command_set(&(VkViewport){
    .width    = (float)example->gpu.device_swapchain.cfg.imageExtent.width,
    .height   = (float)example->gpu.device_swapchain.cfg.imageExtent.height,
    .maxDepth = 1.0f, // near:0, far:1
  }, &example->sync.command_buffer[frameID]);  // clang-format on
  // clang-format off
  cvk_scissor_command_set(&(VkRect2D){
    .offset = (VkOffset2D){.x= 0, .y= 0},
    .extent = example->gpu.device_swapchain.cfg.imageExtent,
  }, &example->sync.command_buffer[frameID]);  // clang-format on
  // clang-format off
  cvk_command_draw_indexed(&example->sync.command_buffer[frameID], &(cvk_command_draw_indexed_args){
    .indices_len = 6,
  });  // clang-format on
  cvk_renderpass_command_end(&example->pipeline.renderpass, &example->sync.command_buffer[frameID]);
  cvk_command_buffer_end(&example->sync.command_buffer[frameID]);

  //______________________________________
  // 4. Submit the recorded command buffer
  // clang-format off
  cvk_device_queue_submit(&example->gpu.device_queue, &(cvk_device_queue_submit_args){
    .command_buffer   = &example->sync.command_buffer[frameID],
    .semaphore_wait   = &example->sync.imageAvailable[frameID],
    .semaphore_signal = &example->gpu.device_swapchain.images.ptr[imageID].finished,
    .fence            = &example->sync.framesPending[frameID],
  });  // clang-format on

  //______________________________________
  // 5. Present the Swapchain image to the screen
  //    @note: Called `vkQueuePresentKHR` by Vulkan
  cvk_device_swapchain_present(&example->gpu.device_swapchain, imageID, &example->gpu.device_queue);

  //______________________________________
  // 6. Advance to next frame
  frameID = (frameID + 1) % example_frames_Len;

#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
}


static void example_resize (
  GLFWwindow* window,
  int         width,
  int         height
) {
  //______________________________________
  // FIX: Setting example->resized to true crashes the app
  //______________________________________
  cvk_discard(width);
  cvk_discard(height);
  Example* const example = (Example*)glfwGetWindowUserPointer(window);
  // example->resized       = true;
  cvk_discard(example);
}


int main () {
  // Initialize: Window/Input
  csys_init_Options system_cfg = csys_init_Options_defaults();
  system_cfg.window.cb_resize  = example_resize;
  // system_cfg.window.resize     = true;  // FIX: Which part of the resizing logic is out of order ?
  csys_System system = csys_init(system_cfg);

  // Initialize: Example Data
  Example example = example_create(
    /* system */ &system,  // clang-format off
    /* vert   */ &(cvk_SpirV){ .ptr = (uint32_t*)examples_shaders_triangle_buffered_vert_spv, .len = examples_shaders_triangle_buffered_vert_spv_len/sizeof(uint32_t), .itemsize= sizeof(uint32_t) },
    /* frag   */ &(cvk_SpirV){ .ptr = (uint32_t*)examples_shaders_triangle_buffered_frag_spv, .len = examples_shaders_triangle_buffered_frag_spv_len/sizeof(uint32_t), .itemsize= sizeof(uint32_t) }
  );  // clang-format on
  glfwSetWindowUserPointer(system.window.ct, (void*)&example);

  // Update Loop
  while (!csys_close(&system)) {
    csys_update(&system);
    example_update(&example, &system);
  }

  // Terminate
  example_destroy(&example);
  csys_term(&system);
  return 0;
}

