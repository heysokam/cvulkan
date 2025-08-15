//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
// @deps cdk
#define GLFW_INCLUDE_VULKAN
#define csys_Implementation
#include "helpers/csys.h"
#define ctime_Implementation
#include "helpers/ctime.h"
// @deps cvulkan
#define cvk_Implementation
#include <cvulkan.h>


//______________________________________
// SpirV shaders used by this example
extern unsigned char examples_shaders_texture_frag_spv[];
extern unsigned int  examples_shaders_texture_frag_spv_len;
#include "./shaders/texture.frag.c"
//____________________________
extern unsigned char examples_shaders_texture_vert_spv[];
extern unsigned int  examples_shaders_texture_vert_spv_len;
#include "./shaders/texture.vert.c"
//______________________________________


//______________________________________
// Generic boilerplate shared by all Examples
#include "./helpers/bootstrap.c"
#include "./helpers/cmath.h"
#include "./helpers/cmath.c"
#include "./helpers/image.c"
#include "./helpers/model.c"
//______________________________________

typedef struct example_Framebuffers {
  cvk_framebuffer_List list;
  // @note Must be a 2D array. We need one attachment pointer for each Swapchain.Image
  cvk_Slice /* VkImageView[][] */ attachments;  ///< Slice of VkImageView[N]
} example_Framebuffers;
typedef struct example_Pipeline {
  cvk_pipeline_Graphics           graphics;
  VkPipelineShaderStageCreateInfo stages[2];
  cvk_Renderpass                  renderpass;
  VkAttachmentDescription         attachment_cfg[2];
  VkAttachmentReference           depthStencil;
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

typedef struct example_wvp_Data {
  Mat4 world, view, projection;
} example_wvp_Data;

typedef struct example_WVP {
  cvk_Buffer       buffer;
  cvk_Memory       memory;
  char             priv_pad[8];
  example_wvp_Data data;
} example_WVP;

typedef struct example_Texture {
  cvk_image_Data    data;
  cvk_Memory        memory;
  cvk_image_View    view;
  cvk_image_Sampler sampler;
  stbi_uc*          pixels;
  int               width;
  int               height;
  int               channels;
  char              priv_pad[4];
  VkDeviceSize      size;
} example_Texture;

typedef struct example_Vertices {
  example_Buffer                    buffer;
  VkVertexInputBindingDescription   binding;
  VkVertexInputAttributeDescription attributes[3];
  char                              priv_pad[4];
  example_Memory                    memory;
  cvk_Slice /* Vertex[]   */        data;
} example_Vertices;
typedef struct example_Indices {
  // @note
  // There is no need to keep the staging buffer around.
  // Keeping it until app termination just for simplicity of the code
  example_Buffer             buffer;
  example_Memory             memory;
  cvk_Slice /* uint32_t[] */ data;
} example_Indices;
typedef struct example_Geometry {
  example_Vertices vertices;
  example_Indices  indices;
  cgltf_data*      gltf;
} example_Geometry;

typedef struct example_Depth {
  cvk_image_Data          data;
  cvk_Memory              memory;
  cvk_image_View          view;
  VkAttachmentDescription attachment_cfg;
  char                    priv_pad[4];
} example_Depth;


typedef struct example_descriptors_Layout {
  VkDescriptorSetLayout           ct;
  VkDescriptorSetLayoutCreateInfo cfg;
  VkDescriptorSetLayoutBinding    bindings[2];
} example_descriptors_Layout;

typedef struct example_descriptors_Pool {
  VkDescriptorPool           ct;
  VkDescriptorPoolCreateInfo cfg;
  VkDescriptorPoolSize       sizes[2];
} example_descriptors_Pool;

typedef struct example_Descriptors {
  // FIX: Organize this mess. Names are misleading and data is not grouped well
  example_descriptors_Pool    pool;
  example_descriptors_Layout  layout;
  VkDescriptorSetLayout       sets_layouts[example_frames_Len];
  VkDescriptorSetAllocateInfo sets_cfg;
  VkDescriptorSet             sets_ptr[example_frames_Len];
  VkWriteDescriptorSet        writes_ptr[2];  // @note Number of uniforms
  VkDescriptorBufferInfo      buffer_cfg[example_frames_Len];
  VkDescriptorImageInfo       texture_cfg[example_frames_Len];
} example_Descriptors;

typedef struct Example {
  example_Bootstrap gpu;
  struct {
    cvk_Shader vert;
    cvk_Shader frag;
  } shader;
  example_Pipeline     pipeline;
  example_Framebuffers device_framebuffers;
  example_Sync         sync;
  cvk_bool             resized;
  char                 priv_pad1[12];
  example_WVP          wvp[example_frames_Len];
  example_Texture      texture;
  example_Geometry     geometry;
  example_Depth        depth;
  example_Descriptors  descriptors;
  ctime_Time           time;
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
  example_Bootstrap* const  gpu,
  example_Sync const* const sync,
  cgltf_data const* const   gltf
) {
  example_Vertices result = (example_Vertices){ 0 };

  // Get the GLTF data
  result.data = (cvk_Slice){ .itemsize = sizeof(Vertex) };
  gltf_get_attribute(gltf, "POSITION", &result.data);
  gltf_get_attribute(gltf, "TEXCOORD_0", &result.data);

  // Create the GPU/CPU handles
  result.buffer.ram  = cvk_buffer_create(&(cvk_buffer_create_args){
     .device_physical = &gpu->device_physical,
     .device_logical  = &gpu->device_logical,
     .usage           = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
     .size            = result.data.itemsize * result.data.len,
     .memory_flags    = cvk_memory_HostVisible | cvk_memory_HostCoherent,
     .allocator       = &gpu->instance.allocator,
  });
  result.buffer.vram = cvk_buffer_create(&(cvk_buffer_create_args){
    .device_physical = &gpu->device_physical,
    .device_logical  = &gpu->device_logical,
    .usage           = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    .size            = result.data.itemsize * result.data.len,
    .memory_flags    = cvk_memory_DeviceLocal,
    .allocator       = &gpu->instance.allocator,
  });
  result.memory.ram  = cvk_memory_create(&(cvk_memory_create_args){
     .device_logical = &gpu->device_logical,
     .data           = result.data.ptr,
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
  cvk_buffer_bind(
    &result.buffer.ram,
    &(cvk_buffer_bind_args){
      .device_logical = &gpu->device_logical,
      .memory         = &result.memory.ram,
    }
  );
  cvk_buffer_bind(
    &result.buffer.vram,
    &(cvk_buffer_bind_args){
      .device_logical = &gpu->device_logical,
      .memory         = &result.memory.vram,
    }
  );

  // Copy the RAM buffer data to the VRAM buffer synchronously
  // clang-format off
  cvk_command_Buffer command_buffer = cvk_command_buffer_allocate(&(cvk_command_buffer_allocate_args){
    .device_logical = &gpu->device_logical,
    .command_pool   = &sync->command_pool,
  });
  cvk_command_buffer_begin2(&command_buffer, &(cvk_command_buffer_begin_args){
    .flags = cvk_command_buffer_OneTimeSubmit,
  });
  //__________________

  cvk_buffer_command_copy(&result.buffer.ram, &result.buffer.vram, &(cvk_buffer_copy_args){
    .command_buffer = &command_buffer,
  });

  //__________________
  cvk_command_buffer_end(&command_buffer);
  cvk_device_queue_submit(&gpu->device_queue, &(cvk_device_queue_submit_args){
    .command_buffer = &command_buffer,
  });
  cvk_device_queue_wait(&gpu->device_queue);
  cvk_command_buffer_free(&command_buffer, &(cvk_command_buffer_free_args){
    .device_logical = &gpu->device_logical,
    .command_pool   = &sync->command_pool,
  });
  // clang-format on

  result.binding = cvk_vertex_binding_description(0, sizeof(Vertex), cvk_false);
  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  result.attributes[0] = (VkVertexInputAttributeDescription){ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos) };
  result.attributes[1] = (VkVertexInputAttributeDescription){ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) };
  result.attributes[2] = (VkVertexInputAttributeDescription){ 2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv) };
  #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  // clang-format on
  return result;
}

static void example_verts_destroy (
  example_Vertices* const  vertices,
  example_Bootstrap* const gpu
) {
  gpu->instance.allocator.cpu.free(&gpu->instance.allocator.cpu, &vertices->data);
  vertices->binding = (VkVertexInputBindingDescription){ 0 };
  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  vertices->attributes[0] = (VkVertexInputAttributeDescription){ 0 };
  vertices->attributes[1] = (VkVertexInputAttributeDescription){ 0 };
  vertices->attributes[2] = (VkVertexInputAttributeDescription){ 0 };
  #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  // clang-format on
  cvk_buffer_destroy(&vertices->buffer.ram, &gpu->device_logical, &gpu->instance.allocator);
  cvk_buffer_destroy(&vertices->buffer.vram, &gpu->device_logical, &gpu->instance.allocator);
  cvk_memory_destroy(&vertices->memory.ram, &gpu->device_logical, &gpu->instance.allocator);
  cvk_memory_destroy(&vertices->memory.vram, &gpu->device_logical, &gpu->instance.allocator);
}


static example_Indices example_inds_create (
  example_Bootstrap* const  gpu,
  example_Sync const* const sync,
  cgltf_data const* const   gltf
) {
  example_Indices result = (example_Indices){ 0 };

  // Get the GLTF data
  result.data = (cvk_Slice){ .itemsize = sizeof(uint16_t) };
  gltf_get_indices(gltf, &result.data);

  // Create the GPU/CPU handles
  result.buffer.ram  = cvk_buffer_create(&(cvk_buffer_create_args){
     .device_physical = &gpu->device_physical,
     .device_logical  = &gpu->device_logical,
     .usage           = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
     .size            = result.data.itemsize * result.data.len,
     .memory_flags    = cvk_memory_HostVisible | cvk_memory_HostCoherent,
     .allocator       = &gpu->instance.allocator,
  });
  result.buffer.vram = cvk_buffer_create(&(cvk_buffer_create_args){
    .device_physical = &gpu->device_physical,
    .device_logical  = &gpu->device_logical,
    .usage           = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    .size            = result.data.itemsize * result.data.len,
    .memory_flags    = cvk_memory_DeviceLocal,
    .allocator       = &gpu->instance.allocator,
  });
  result.memory.ram  = cvk_memory_create(&(cvk_memory_create_args){
     .device_logical = &gpu->device_logical,
     .data           = result.data.ptr,
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

  // Copy the RAM buffer data to the VRAM buffer synchronously
  // clang-format off
  cvk_command_Buffer command_buffer = cvk_command_buffer_allocate(&(cvk_command_buffer_allocate_args){
    .device_logical = &gpu->device_logical,
    .command_pool   = &sync->command_pool,
  });
  cvk_command_buffer_begin2(&command_buffer, &(cvk_command_buffer_begin_args){
    .flags = cvk_command_buffer_OneTimeSubmit,
  });
  //__________________

  cvk_buffer_command_copy(&result.buffer.ram, &result.buffer.vram, &(cvk_buffer_copy_args){
    .command_buffer = &command_buffer,
  });

  //__________________
  cvk_command_buffer_end(&command_buffer);
  cvk_device_queue_submit(&gpu->device_queue, &(cvk_device_queue_submit_args){
    .command_buffer = &command_buffer,
  });
  cvk_device_queue_wait(&gpu->device_queue);
  cvk_command_buffer_free(&command_buffer, &(cvk_command_buffer_free_args){
    .device_logical = &gpu->device_logical,
    .command_pool   = &sync->command_pool,
  });
  // clang-format on

  return result;
}

static void example_inds_destroy (
  example_Indices* const   indices,
  example_Bootstrap* const gpu
) {
  gpu->instance.allocator.cpu.free(&gpu->instance.allocator.cpu, &indices->data);
  cvk_buffer_destroy(&indices->buffer.ram, &gpu->device_logical, &gpu->instance.allocator);
  cvk_buffer_destroy(&indices->buffer.vram, &gpu->device_logical, &gpu->instance.allocator);
  cvk_memory_destroy(&indices->memory.ram, &gpu->device_logical, &gpu->instance.allocator);
  cvk_memory_destroy(&indices->memory.vram, &gpu->device_logical, &gpu->instance.allocator);
}


static example_Geometry example_geometry_create (
  example_Bootstrap* const  gpu,
  example_Sync const* const sync,
  cvk_String const          path
) {
  // FIX: Remove
  cvk_discard(gpu);

  example_Geometry result = (example_Geometry){ 0 };

  // Read the GLTF file data
  cgltf_options gltf_options = (cgltf_options){ 0 };
  // clang-format off
  cgltf_result_check(cgltf_parse_file(&gltf_options, path, &result.gltf),
    "Failed to load the geometry from the target .glb file");
  cgltf_result_check(cgltf_load_buffers(&gltf_options, result.gltf, path),
    "Failed to load the buffers data from the target .glb file.");
  cgltf_result_check(cgltf_validate(result.gltf),
    "Failed to validate the data from the target .glb file.");
  // clang-format on

  // Populate the Vertices/Indices data
  result.vertices = example_verts_create(gpu, sync, result.gltf);
  result.indices  = example_inds_create(gpu, sync, result.gltf);

  // Return the result
  return result;
}

static void example_geometry_destroy (
  example_Geometry* const  geometry,
  example_Bootstrap* const gpu
) {
  example_verts_destroy(&geometry->vertices, gpu);
  example_inds_destroy(&geometry->indices, gpu);
  cgltf_free(geometry->gltf);
}


static example_Descriptors example_descriptors_create (
  example_Bootstrap* const     gpu,
  example_WVP const            wvp[static example_frames_Len],
  example_Texture const* const texture
) {
  example_Descriptors result = (example_Descriptors){ 0 };

  cvk_discard(texture);  // FIX: Remove

  //________________________________________________
  // TODO: cvk_descriptor_pool_create
  // clang-format off
  result.pool = (example_descriptors_Pool){
    .ct                = NULL,
    .cfg               = (VkDescriptorPoolCreateInfo){
      .sType           = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
      .pNext           = NULL,
      .flags           = (VkDescriptorPoolCreateFlags)0,    // TODO: Configurable & Custom Enum
      .maxSets         = example_frames_Len,                // TODO: Configurable
      .poolSizeCount   = 0,
      .pPoolSizes      = NULL,
    },
  };
  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  result.pool.sizes[0] = (VkDescriptorPoolSize){
    .type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, // TODO: Configurable & Custom Enum
    .descriptorCount = example_frames_Len,                // TODO: Configurable
  };
  result.pool.sizes[1] = (VkDescriptorPoolSize){
    .type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, // TODO: Configurable & Custom Enum
    .descriptorCount = example_frames_Len,                        // TODO: Configurable
  };
  #pragma GCC diagnostic pop
  // clang-format on
  result.pool.cfg.poolSizeCount = 2;
  result.pool.cfg.pPoolSizes    = result.pool.sizes;
  cvk_result_check(
    vkCreateDescriptorPool(gpu->device_logical.ct, &result.pool.cfg, gpu->instance.allocator.gpu, &result.pool.ct),
    "Failed to create a Descriptor.Pool context."
  );  // clang-format on

  //________________________________________________
  // TODO: cvk_descriptor_layout_create
  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  result.layout.bindings[0] = (VkDescriptorSetLayoutBinding){
    .binding            = 0,
    .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, // TODO: Configurable & Custom Enum
    .descriptorCount    = 1,
    .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,        // TODO: Configurable & Custom Enum
    .pImmutableSamplers = NULL,                              // TODO: Image Sampling
  };
  result.layout.bindings[1] = (VkDescriptorSetLayoutBinding){
    .binding            = 1,
    .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, // TODO: Configurable & Custom Enum
    .descriptorCount    = 1,
    .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,      // TODO: Configurable & Custom Enum
    .pImmutableSamplers = NULL,                              // TODO: Image Sampling
  };
  #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  // clang-format on

  result.layout.cfg = (VkDescriptorSetLayoutCreateInfo){
    .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .pNext        = NULL,
    .flags        = (VkDescriptorSetLayoutCreateFlags)0,  // TODO: Customizable (eg: Update-after-Bind)
    .bindingCount = 2,
    .pBindings    = result.layout.bindings,
  };  // clang-format off
  cvk_result_check(vkCreateDescriptorSetLayout(gpu->device_logical.ct, &result.layout.cfg, gpu->instance.allocator.gpu, &result.layout.ct),
    "Failed to create a Descriptor.Layout context.");
  // clang-format on


  //________________________________________________
  // TODO: cvk_descriptor_set_create
  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  for (cvk_size id = 0; id < example_frames_Len; ++id) result.sets_layouts[id] = result.layout.ct;
  #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  // clang-format on
  result.sets_cfg = (VkDescriptorSetAllocateInfo){
    .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .pNext              = NULL,
    .descriptorPool     = result.pool.ct,
    .descriptorSetCount = example_frames_Len,
    .pSetLayouts        = result.sets_layouts,
  };  // clang-format off
  cvk_result_check(vkAllocateDescriptorSets(gpu->device_logical.ct, &result.sets_cfg, result.sets_ptr),
    "Failed to allocate list of Descriptor.Set contexts.");
  // clang-format on

  for (cvk_size id = 0; id < example_frames_Len; ++id) {
    // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    result.buffer_cfg[id] = (VkDescriptorBufferInfo){
      .buffer = wvp[id].buffer.ct,
      .offset = 0,
      .range  = sizeof(example_wvp_Data),
    };
    result.texture_cfg[id] = (VkDescriptorImageInfo){
      .sampler     = texture->sampler.ct,
      .imageView   = texture->view.ct,
      .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };
    result.writes_ptr[0] = (VkWriteDescriptorSet){
      .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext            = NULL,
      .dstSet           = result.sets_ptr[id],
      .dstBinding       = 0,
      .dstArrayElement  = 0,
      .descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
      .descriptorCount  = 1,
      .pBufferInfo      = &result.buffer_cfg[id], // TODO: Configurable
      .pImageInfo       = NULL,                   // TODO: Configurable
      .pTexelBufferView = NULL,                   // TODO: Configurable
    };
    result.writes_ptr[1] = (VkWriteDescriptorSet){
      .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext            = NULL,
      .dstSet           = result.sets_ptr[id],
      .dstBinding       = 1,
      .dstArrayElement  = 0,
      .descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      .descriptorCount  = 1,
      .pBufferInfo      = NULL,                    // TODO: Configurable
      .pImageInfo       = &result.texture_cfg[id], // TODO: Configurable
      .pTexelBufferView = NULL,                    // TODO: Configurable
    };
    vkUpdateDescriptorSets(
      /* device               */ gpu->device_logical.ct,
      /* descriptorWriteCount */ 2,
      /* pDescriptorWrites    */ result.writes_ptr,
      /* descriptorCopyCount  */ 0,
      /* pDescriptorCopies    */ NULL
    );
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
    // clang-format on
  }

  return result;
}

static void example_descriptors_destroy (
  example_Descriptors* const descriptors,
  example_Bootstrap* const   gpu
) {
  //________________________________________________
  // TODO: cvk_descriptor_pool_destroy
  descriptors->pool.cfg = (VkDescriptorPoolCreateInfo){ 0 };
  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  descriptors->pool.sizes[0] = (VkDescriptorPoolSize){ 0 };
  descriptors->pool.sizes[1] = (VkDescriptorPoolSize){ 0 };
  #pragma GCC diagnostic pop
  // clang-format on
  if (descriptors->pool.ct) vkDestroyDescriptorPool(gpu->device_logical.ct, descriptors->pool.ct, gpu->instance.allocator.gpu);
  //________________________________________________
  // TODO: cvk_descriptor_layout_destroy
  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  descriptors->layout.bindings[0] = (VkDescriptorSetLayoutBinding){ 0 };
  descriptors->layout.bindings[1] = (VkDescriptorSetLayoutBinding){ 0 };
  #pragma GCC diagnostic pop
  // clang-format on
  descriptors->layout.cfg = (VkDescriptorSetLayoutCreateInfo){ 0 };
  if (descriptors->layout.ct) vkDestroyDescriptorSetLayout(gpu->device_logical.ct, descriptors->layout.ct, gpu->instance.allocator.gpu);
}


static example_WVP example_wvp_create (
  example_Bootstrap* const gpu
) {
  example_WVP result = (example_WVP){ 0 };  // clang-format off
  result.buffer      = cvk_buffer_create(&(cvk_buffer_create_args){
    .device_physical = &gpu->device_physical,
    .device_logical  = &gpu->device_logical,
    .usage           = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    .size            = sizeof(example_wvp_Data),
    .memory_flags    = cvk_memory_HostVisible | cvk_memory_HostCoherent,
    .allocator       = &gpu->instance.allocator,
  });
  result.memory      = cvk_memory_create(&(cvk_memory_create_args){
    .device_logical = &gpu->device_logical,
    .data           = (void*)&result.data,
    .kind           = result.buffer.memory.kind,
    .persistent     = cvk_true,
    .size_alloc     = result.buffer.memory.requirements.size,
    .size_data      = result.buffer.cfg.size,
    .allocator      = &gpu->instance.allocator,
  });
  cvk_buffer_bind(&result.buffer, &(cvk_buffer_bind_args){
    .device_logical = &gpu->device_logical,
    .memory         = &result.memory,
  });  // clang-format on
  return result;
}

static void example_wvp_destroy (
  example_WVP* const       wvp,
  example_Bootstrap* const gpu
) {
  wvp->data = (example_wvp_Data){ 0 };
  cvk_buffer_destroy(&wvp->buffer, &gpu->device_logical, &gpu->instance.allocator);
  cvk_memory_destroy(&wvp->memory, &gpu->device_logical, &gpu->instance.allocator);
}

static void example_wvp_upload (
  example_WVP* const       wvp,
  example_Bootstrap* const gpu
) {
  gpu->instance.allocator.cpu.copy(
    /* A   */ &gpu->instance.allocator.cpu,
    /* src */ &(cvk_Slice){ 1, sizeof(example_wvp_Data), &wvp->data },
    /* trg */ &(cvk_Slice){ 1, sizeof(example_wvp_Data), wvp->memory.data }
  );
}

static void example_wvp_update (
  example_WVP* const       wvp,
  ctime_Time* const        time,
  example_Bootstrap* const gpu
) {
  // clang-format off
  double time_elapsed = (double)(ctime_nsec(time) - time->start) / 1000000000;
  Mat4 mat4_identity; mat4x4_identity(mat4_identity);
  // clang-format on
  mat4x4_rotate_Z(wvp->data.world, mat4_identity, (float)(deg_to_rad(90) * time_elapsed));
  Vec3 eye    = { [X] = 2.0f, [Y] = 2.0f, [Z] = 2.0f };
  Vec3 center = { [X] = 0.0f, [Y] = 0.0f, [Z] = 0.0f };
  Vec3 up     = { [X] = 0.0f, [Y] = 0.0f, [Z] = 1.0f };
  mat4x4_look_at(wvp->data.view, eye, center, up);
  float fov   = 90;
  float ratio = (float)gpu->device_swapchain.cfg.imageExtent.width / (float)gpu->device_swapchain.cfg.imageExtent.height;
  float near  = 0.1f;
  float far   = 10.0f;
  mat4x4_perspective(wvp->data.projection, fov / 2, ratio, near, far);
  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  wvp->data.projection[1][1] *= -1;  // Convert OpenGL Y axis for Vulkan. (possibly: LH-cw to RH-ccw)
  #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  // clang-format on
  example_wvp_upload(wvp, gpu);
}


static example_Texture example_texture_create (
  example_Bootstrap* const  gpu,
  example_Sync const* const sync,
  cvk_String const          path
) {
  example_Texture result = (example_Texture){ 0 };

  result.pixels = stbi_load(path, &result.width, &result.height, &result.channels, STBI_rgb_alpha);
  cvk_assert(result.pixels, "Couldn't load the example image from the given path.");
  result.size = (uint32_t)result.width * (uint32_t)result.height * (uint32_t)STBI_rgb_alpha;

  // RAM data (temporary, only during creation)
  cvk_Buffer ram_buffer = cvk_buffer_create(&(cvk_buffer_create_args){
    .device_physical = &gpu->device_physical,
    .device_logical  = &gpu->device_logical,
    .usage           = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    .size            = result.size,
    .memory_flags    = cvk_memory_HostVisible | cvk_memory_HostCoherent,
    .allocator       = &gpu->instance.allocator,
  });
  cvk_Memory ram_memory = cvk_memory_create(&(cvk_memory_create_args){
    .device_logical = &gpu->device_logical,
    .data           = (void*)result.pixels,
    .kind           = ram_buffer.memory.kind,
    .size_alloc     = ram_buffer.memory.requirements.size,
    .size_data      = ram_buffer.cfg.size,
    .allocator      = &gpu->instance.allocator,
  });  // clang-format off
  cvk_buffer_bind(&ram_buffer, &(cvk_buffer_bind_args){
    .device_logical = &gpu->device_logical,
    .memory         = &ram_memory,
  });  // clang-format on

  // VRAM data
  // clang-format off
  result.data = cvk_image_data_create(&(cvk_image_data_create_args){
    .device_physical = &gpu->device_physical,
    .device_logical  = &gpu->device_logical,
    .dimensions      = VK_IMAGE_TYPE_2D,
    .width           = (uint32_t)result.width,
    .height          = (uint32_t)result.height,
    .format          = VK_FORMAT_R8G8B8A8_SRGB,
    .samples         = VK_SAMPLE_COUNT_1_BIT,     // @note: Default value, can be omitted
    .tiling          = VK_IMAGE_TILING_OPTIMAL,   // @note: Default value, can be omitted
    .memory_flags    = cvk_memory_DeviceLocal,
    .usage           = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
    .sharing         = VK_SHARING_MODE_EXCLUSIVE, // @note: Default value, can be omitted
    .layout          = VK_IMAGE_LAYOUT_UNDEFINED, // @note: Default value, can be omitted
    .allocator       = &gpu->instance.allocator,
  });
  result.memory = cvk_memory_create(&(cvk_memory_create_args){
    .device_logical = &gpu->device_logical,
    .kind           = result.data.memory.kind,
    .size_alloc     = result.data.memory.requirements.size,
    .size_data      = ram_buffer.cfg.size,
    .allocator      = &gpu->instance.allocator,
  });
  cvk_image_data_bind(&result.data, &(cvk_image_data_bind_args){
    .device_logical = &gpu->device_logical,
    .memory         = &result.memory,
  });  // clang-format on


  //________________________________________________
  // TODO: cvk_image_data_copy
  // Sync data (temporary, only during creation)
  cvk_command_Buffer command_buffer = example_command_buffer_begin_onetime(&sync->command_pool, gpu);
  //__________________

  // Submit the Image.upload operation
  // NOTE: Remember: `.access_src= VK_ACCESS_HOST_WRITE_BIT` for explicit access before command buffer submission
  // clang-format off
  cvk_image_data_command_transition(&result.data, &(cvk_image_data_transition_args){
    .layout_old     = VK_IMAGE_LAYOUT_UNDEFINED,
    .layout_new     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    .access_src     = 0,
    .access_trg     = VK_ACCESS_TRANSFER_WRITE_BIT,
    .stage_src      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    .stage_trg      = VK_PIPELINE_STAGE_TRANSFER_BIT,
    .command_buffer = &command_buffer,
  });
  cvk_image_data_command_copy_fromBuffer(&result.data, &ram_buffer, &(cvk_image_data_copy_args){
    .command_buffer = &command_buffer,
  });
  cvk_image_data_command_transition(&result.data, &(cvk_image_data_transition_args){
    .layout_old     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    .layout_new     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    .access_src     = VK_ACCESS_TRANSFER_WRITE_BIT,
    .access_trg     = VK_ACCESS_SHADER_READ_BIT,
    .stage_src      = VK_PIPELINE_STAGE_TRANSFER_BIT,
    .stage_trg      = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
    .command_buffer = &command_buffer,
  });  // clang-format on

  //__________________
  // Cleanup temporary RAM & Sync data
  example_command_buffer_end_onetime(&command_buffer, &sync->command_pool, gpu);
  cvk_buffer_destroy(&ram_buffer, &gpu->device_logical, &gpu->instance.allocator);
  cvk_memory_destroy(&ram_memory, &gpu->device_logical, &gpu->instance.allocator);

  result.view = cvk_image_view_create(&(cvk_image_view_create_args){
    .image_data     = &result.data,
    .device_logical = &gpu->device_logical,
    .allocator      = &gpu->instance.allocator,
  });

  result.sampler = cvk_image_sampler_create(&(cvk_image_sampler_create_args){
    .device_physical = &gpu->device_physical,
    .device_logical  = &gpu->device_logical,
    .allocator       = &gpu->instance.allocator,
    .filter_min      = VK_FILTER_LINEAR,
    .filter_mag      = VK_FILTER_LINEAR,
    .border_color    = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    .mip_mode        = VK_SAMPLER_MIPMAP_MODE_LINEAR,
    // .anisotropy_enabled = cvk_true,  // TODO: Device.Feature: Anisotropy
  });

  return result;
}

static void example_texture_destroy (
  example_Texture* const   texture,
  example_Bootstrap* const gpu
) {
  stbi_image_free(texture->pixels);
  texture->width    = 0;
  texture->height   = 0;
  texture->channels = 0;
  cvk_memory_destroy(&texture->memory, &gpu->device_logical, &gpu->instance.allocator);
  cvk_image_sampler_destroy(&texture->sampler, &gpu->device_logical, &gpu->instance.allocator);
  cvk_image_view_destroy(&texture->view, &gpu->device_logical, &gpu->instance.allocator);
  cvk_image_data_destroy(&texture->data, &gpu->device_logical, &gpu->instance.allocator);
}


static VkAttachmentDescription example_depth_attachment_cfg (
  VkFormat const format
) {
  return (VkAttachmentDescription){
    .flags          = (VkAttachmentDescriptionFlags)0,
    .format         = format,
    .samples        = VK_SAMPLE_COUNT_1_BIT,
    .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };
}

static example_Depth example_depth_create (
  example_Bootstrap* const  gpu,
  example_Sync const* const sync
) {
  example_Depth result = (example_Depth){ 0 };

  // FIX: Pick the best Depth format available for the device);
  result.attachment_cfg = example_depth_attachment_cfg(VK_FORMAT_D32_SFLOAT);

  // clang-format off
  result.data = cvk_image_data_create(&(cvk_image_data_create_args){
    .device_physical = &gpu->device_physical,
    .device_logical  = &gpu->device_logical,
    .allocator       = &gpu->instance.allocator,
    .dimensions      = VK_IMAGE_TYPE_2D,
    .width           = gpu->device_swapchain.cfg.imageExtent.width,
    .height          = gpu->device_swapchain.cfg.imageExtent.height,
    .format          = result.attachment_cfg.format,
    .usage           = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    .memory_flags    = cvk_memory_DeviceLocal,
  });
  result.memory = cvk_memory_create(&(cvk_memory_create_args){
    .device_logical = &gpu->device_logical,
    .kind           = result.data.memory.kind,
    .size_alloc     = result.data.memory.requirements.size,
    .size_data      = sizeof(uint32_t) * result.data.cfg.extent.width * result.data.cfg.extent.height * result.data.cfg.extent.depth,
    .allocator      = &gpu->instance.allocator,
  });
  cvk_image_data_bind(&result.data, &(cvk_image_data_bind_args){
    .device_logical = &gpu->device_logical,
    .memory         = &result.memory,
  });
  result.view = cvk_image_view_create(&(cvk_image_view_create_args){
    .device_logical = &gpu->device_logical,
    .allocator      = &gpu->instance.allocator,
    .image_data     = &result.data,
    .aspect         = VK_IMAGE_ASPECT_DEPTH_BIT,
  });  // clang-format on

  // clang-format off
  cvk_command_Buffer command_buffer = example_command_buffer_begin_onetime(&sync->command_pool, gpu);
  cvk_image_data_command_transition(&result.data, &(cvk_image_data_transition_args){
    .command_buffer = &command_buffer,
    .layout_old     = VK_IMAGE_LAYOUT_UNDEFINED,
    .layout_new     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    .access_src     = 0,
    .access_trg     = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    .stage_src      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    .stage_trg      = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    .aspect         = VK_IMAGE_ASPECT_DEPTH_BIT,  // TODO: Add stencil bit when Depth.Format requires it
  });   // clang-format on
  example_command_buffer_end_onetime(&command_buffer, &sync->command_pool, gpu);

  return result;
}

static void example_depth_destroy (
  example_Depth* const     depth,
  example_Bootstrap* const gpu
) {
  depth->attachment_cfg = (VkAttachmentDescription){ 0 };
  cvk_image_view_destroy(&depth->view, &gpu->device_logical, &gpu->instance.allocator);
  cvk_memory_destroy(&depth->memory, &gpu->device_logical, &gpu->instance.allocator);
  cvk_image_data_destroy(&depth->data, &gpu->device_logical, &gpu->instance.allocator);
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
// NOTE:
// The double allocation in this function is idiotic.
// But we need one Framebuffer for each Swapchain.Image      _(known at runtime)_,
// and multiple Framebuffer.Attachments for each :facepalm:  _(could be arrays, but the above forbids it)_,
// so there is no way around it :( The API without dynamic rendering is a mess.
static example_Framebuffers example_rendering_framebuffers_create (
  example_Bootstrap* const    gpu,
  example_Depth const* const  depth,
  cvk_Renderpass const* const renderpass
) {
  // clang-format off
  example_Framebuffers result = (example_Framebuffers){
    .list       = (cvk_framebuffer_List){
      .len      = gpu->device_swapchain.images.len,
      .itemsize = sizeof(cvk_Framebuffer),
      .ptr      = NULL,
    },
    .attachments = (cvk_Slice){ .len= gpu->device_swapchain.images.len },
  };  // clang-format on

  // Create the outer part of the VkImageView[][2]
  result.attachments = gpu->instance.allocator.cpu.allocZ(&gpu->instance.allocator.cpu, result.attachments.len, sizeof(cvk_Slice));
  for (cvk_size id = 0; id < result.attachments.len; ++id) {
    // .len=2 -> Swapchain.Image.View, Depth.View
    ((cvk_Slice*)result.attachments.ptr)[id] = gpu->instance.allocator.cpu.allocZ(&gpu->instance.allocator.cpu, 2, sizeof(cvk_Slice));
  }

  // Create the inner part of the VkImageView[][2]
  cvk_Slice framebuffers_tempdata = gpu->instance.allocator.cpu.allocZ(&gpu->instance.allocator.cpu, result.list.len, result.list.itemsize);
  result.list.ptr                 = (cvk_Framebuffer*)framebuffers_tempdata.ptr;
  for (cvk_size id = 0; id < result.list.len; ++id) {  // clang-format off
    ((VkImageView*)((cvk_Slice*)result.attachments.ptr)[id].ptr)[0] = gpu->device_swapchain.images.ptr[id].view;
    ((VkImageView*)((cvk_Slice*)result.attachments.ptr)[id].ptr)[1] = depth->view.ct;
    result.list.ptr[id] = cvk_framebuffer_create(&(cvk_framebuffer_create_args){
      .device_logical  = &gpu->device_logical,
      .allocator       = &gpu->instance.allocator,
      .renderpass      = renderpass,
      .size            = &gpu->device_swapchain.cfg.imageExtent,
      .attachments_len = ((cvk_Slice*)result.attachments.ptr)[id].len,
      .attachments_ptr = ((cvk_Slice*)result.attachments.ptr)[id].ptr,
    });  // clang-format on
  }
  return result;
}
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
static void example_rendering_framebuffers_destroy (
  example_Framebuffers* const device_framebuffers,
  example_Bootstrap* const    gpu
) {
  for (cvk_size id = 0; id < device_framebuffers->attachments.len; ++id) {
    gpu->instance.allocator.cpu.free(&gpu->instance.allocator.cpu, &((cvk_Slice*)device_framebuffers->attachments.ptr)[id]);
  }
  gpu->instance.allocator.cpu.free(&gpu->instance.allocator.cpu, &device_framebuffers->attachments);
  cvk_framebuffer_list_destroy(&device_framebuffers->list, &gpu->device_logical, &gpu->instance.allocator);
}
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage


static example_Pipeline example_pipeline_create (
  example_Bootstrap* const         gpu,
  example_Vertices const* const    verts,
  example_Indices const* const     inds,
  example_Depth const* const       depth,
  example_Descriptors const* const sets,
  cvk_Shader* const                vert,
  cvk_Shader* const                frag
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
    .vertexAttributeDescriptionCount = 3,                  // TODO: Configurable
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
    .depthClampEnable        = VK_FALSE,                         // TODO: Configurable (niche). Requires Device.Feature
    .rasterizerDiscardEnable = VK_FALSE,                         // TODO: Does this need to be configurable at all ?? Basically disables framebuffer output
    .polygonMode             = VK_POLYGON_MODE_FILL,             // TODO: Configurable (niche-ish). Requires Device.Feature
    .lineWidth               = 1.0f,                             // TODO: Configurable (niche). >1.0 requires Device.Feature for widelines
    .cullMode                = VK_CULL_MODE_BACK_BIT,            // TODO: Configurable
    .frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE,  // TODO: Configurable
    .depthBiasEnable         = VK_FALSE,                         // TODO: Configurable (niche). Useful for ShadowMapping
    .depthBiasConstantFactor = 0.0f,                             // TODO: Configurable (niche). Useful for ShadowMapping
    .depthBiasClamp          = 0.0f,                             // TODO: Configurable (niche). Useful for ShadowMapping
    .depthBiasSlopeFactor    = 0.0f,                             // TODO: Configurable (niche). Useful for ShadowMapping
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
  // clang-format off
  VkPipelineDepthStencilStateCreateInfo state_depthStencil = (VkPipelineDepthStencilStateCreateInfo){
    .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
    .pNext                 = NULL,
    .flags                 = (VkPipelineDepthStencilStateCreateFlags)0,
    .depthTestEnable       = VK_TRUE,
    .depthWriteEnable      = VK_TRUE,
    .depthCompareOp        = VK_COMPARE_OP_LESS,
    .depthBoundsTestEnable = VK_FALSE,
    .minDepthBounds        = 0.0f,
    .maxDepthBounds        = 1.0f,
    // Disabled Stencil
    .stencilTestEnable     = VK_FALSE,
    .front                 = (VkStencilOpState){ 0 },
    .back                  = (VkStencilOpState){ 0 },
  };  // clang-format on

  //________________________________________________
  // TODO: cvk_pipeline_state_tessellation_create()
  //     : NULL, disabled
  VkPipelineTessellationStateCreateInfo* state_tessellation = NULL;
  //________________________________________________


  //________________________________________________
  // clang-format off
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  result.attachment_cfg[0] = gpu->device_swapchain.attachment_cfg;
  result.attachment_cfg[1] = depth->attachment_cfg;
  #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  // clang-format on
  //__________________
  result.depthStencil = (VkAttachmentReference){
    .attachment = 1,
    .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };
  //__________________
  result.renderpass = cvk_renderpass_create(&(cvk_renderpass_create_args){
    .device_logical     = &gpu->device_logical,
    .allocator          = &gpu->instance.allocator,
    .attachment_cfg_len = 2,
    .attachment_cfg_ptr = result.attachment_cfg,
    .depthStencil       = &result.depthStencil,
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
    .state_depthStencil  = &state_depthStencil,
    .state_colorBlend    = &state_colorBlend,
    .state_dynamic       = &state_dynamic,
    .renderpass          = &result.renderpass,
    .stages              = &(cvk_pipeline_shaderStage_List){ .ptr = result.stages, .len = 2 }, // clang-format off
    .layout              = &(cvk_pipeline_layout_create_args){
      .device_logical    = &gpu->device_logical,
      .sets_ptr          = &sets->layout.ct,
      .allocator         = &gpu->instance.allocator,
    },  // clang-format on
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
      .command_pool   = &result.command_pool,
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
  result.time        = ctime_start();
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
  for (cvk_size id = 0; id < example_frames_Len; ++id) {  // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    result.wvp[id] = example_wvp_create(&result.gpu);
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  }  // clang-format on
  result.texture     = example_texture_create(&result.gpu, &result.sync, "examples/models/box_textured/CesiumLogoFlat.png");
  result.geometry    = example_geometry_create(&result.gpu, &result.sync, "examples/models/box_textured/geometry.gltf");
  result.depth       = example_depth_create(&result.gpu, &result.sync);
  result.descriptors = example_descriptors_create(&result.gpu, result.wvp, &result.texture);

  result.pipeline = example_pipeline_create(
    /* gpu   */ &result.gpu,
    /* verts */ &result.geometry.vertices,
    /* inds  */ &result.geometry.indices,
    /* depth */ &result.depth,
    /* sets  */ &result.descriptors,
    /* vert  */ &result.shader.vert,
    /* frag  */ &result.shader.frag
  );

  result.device_framebuffers = example_rendering_framebuffers_create(&result.gpu, &result.depth, &result.pipeline.renderpass);
  return result;
}


static void example_destroy (
  Example* const example
) {
  cvk_device_logical_wait(&example->gpu.device_logical);
  example_depth_destroy(&example->depth, &example->gpu);
  example_geometry_destroy(&example->geometry, &example->gpu);
  example_texture_destroy(&example->texture, &example->gpu);
  for (cvk_size id = 0; id < example_frames_Len; ++id) {  // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    example_wvp_destroy(&example->wvp[id], &example->gpu);
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
  }  // clang-format on
  example_descriptors_destroy(&example->descriptors, &example->gpu);
  example_sync_destroy(&example->sync, &example->gpu.device_logical, &example->gpu.instance.allocator);
  example_rendering_framebuffers_destroy(&example->device_framebuffers, &example->gpu);
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
  //________________________________________________
  // TODO:
  // The resolution of the Depth buffer should change when the window size changes
  // See the bottom bullet point at: https://vulkan-tutorial.com/en/Depth_buffering
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
  cvk_framebuffer_list_destroy(&example->device_framebuffers.list, &example->gpu.device_logical, &example->gpu.instance.allocator);
  // clang-format off
  cvk_device_swapchain_recreate(&example->gpu.device_swapchain, &(cvk_device_swapchain_recreate_args){
    .device_logical = &example->gpu.device_logical,
    .allocator      = &example->gpu.instance.allocator,
  });  // clang-format on
  example->device_framebuffers = example_rendering_framebuffers_create(&example->gpu, &example->depth, &example->pipeline.renderpass);
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
  // 0. Wait for the previous frame to finish
  cvk_fence_wait(&example->sync.framesPending[frameID], &example->gpu.device_logical);


  //____________________________
  // 1. Compute + Update the WVP Matrix
  example_wvp_update(&example->wvp[frameID], &example->time, &example->gpu);


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
  cvk_command_buffer_begin(&example->sync.command_buffer[frameID]);
  VkClearValue const clear_values[2] = {
    [0] = (VkClearValue){ .color = { .float32 = { [0] = 0.222f, [1] = 0.333f, [2] = 0.444f, [3] = 1.0f } } },
    [1] = (VkClearValue){ .depthStencil = { .depth = 1.0f, .stencil = 0 } },
  };  // clang-format off
  cvk_renderpass_command_begin(&example->pipeline.renderpass, &(cvk_renderpass_command_begin_args){
    .command_buffer = &example->sync.command_buffer[frameID],
    .framebuffer    = &example->device_framebuffers.list.ptr[imageID],
    .extent         = example->gpu.device_swapchain.cfg.imageExtent,
    .clear_len      = 2,
    .clear_ptr      = clear_values,
  });  // clang-format on
  cvk_pipeline_graphics_command_bind(&example->pipeline.graphics, &example->sync.command_buffer[frameID]);
  cvk_buffer_vertex_command_bind(&example->geometry.vertices.buffer.vram, &example->sync.command_buffer[frameID]);
  cvk_buffer_index_command_bind(&example->geometry.indices.buffer.vram, &example->sync.command_buffer[frameID]);
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

  //_____________________________________________
  // TODO: cvk_descriptor_command_bind()
  vkCmdBindDescriptorSets(
    /* commandBuffer      */ example->sync.command_buffer[frameID].ct,
    /* pipelineBindPoint  */ VK_PIPELINE_BIND_POINT_GRAPHICS,  // TODO: Configurable
    /* layout             */ example->pipeline.graphics.layout.ct,
    /* firstSet           */ 0,
    /* descriptorSetCount */ 1,
    /* pDescriptorSets    */ &example->descriptors.sets_ptr[frameID],
    /* dynamicOffsetCount */ 0,
    /* pDynamicOffsets    */ NULL
  );
  //_____________________________________________
  // clang-format off
  cvk_command_draw_indexed(&example->sync.command_buffer[frameID], &(cvk_command_draw_indexed_args){
    .indices_len = (uint32_t)example->geometry.indices.data.len,
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
    /* vert   */ &(cvk_SpirV){ .ptr = (uint32_t*)examples_shaders_texture_vert_spv, .len = examples_shaders_texture_vert_spv_len/sizeof(uint32_t), .itemsize= sizeof(uint32_t) },
    /* frag   */ &(cvk_SpirV){ .ptr = (uint32_t*)examples_shaders_texture_frag_spv, .len = examples_shaders_texture_frag_spv_len/sizeof(uint32_t), .itemsize= sizeof(uint32_t) }
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

