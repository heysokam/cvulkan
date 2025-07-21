//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview cvulkan: Core Types
//____________________________________|
#ifndef H_cvulkan_types
#define H_cvulkan_types
#include "./base.h"


//______________________________________
// @section Allocator.CPU: Overridable Type
//____________________________
#ifndef cvk_Allocator_CPU
typedef struct cvk_Allocator_CPU_s cvk_Allocator_CPU_t;
#define cvk_Allocator_CPU cvk_Allocator_CPU_t
#endif  // cvk_Allocator_CPU


//______________________________________
// @section Allocator.CPU: Function Types Declarations
//____________________________
typedef cvk_Pure cvk_Slice (*cvk_Fn_allocator_alloc)(cvk_Allocator_CPU* const, cvk_size const, cvk_size const);
typedef cvk_Pure cvk_Slice (*cvk_Fn_allocator_allocZ)(cvk_Allocator_CPU* const, cvk_size const, cvk_size const);
typedef void (*cvk_Fn_allocator_free)(cvk_Allocator_CPU* const, cvk_Slice* const);
typedef void (*cvk_Fn_allocator_copy)(cvk_Allocator_CPU* const, cvk_Slice const* const, cvk_Slice* const);
typedef cvk_Pure cvk_Slice (*cvk_Fn_allocator_duplicate)(cvk_Allocator_CPU* const, cvk_Slice const* const);


//______________________________________
// @section Allocator.CPU: Core Type
//____________________________
struct cvk_Allocator_CPU_s {
  cvk_Fn_allocator_alloc     alloc;
  cvk_Fn_allocator_allocZ    allocZ;
  cvk_Fn_allocator_free      free;
  cvk_Fn_allocator_copy      copy;
  cvk_Fn_allocator_duplicate duplicate;
};


//______________________________________
// @section Allocator: Core Types
//____________________________
#define cvk_Allocator_GPU VkAllocationCallbacks*
typedef struct cvk_Allocator {
  cvk_Allocator_GPU gpu;
  cvk_Allocator_CPU cpu;
} cvk_Allocator;


//______________________________________
// @section Semaphore
//____________________________

typedef struct cvk_Semaphore {
  VkSemaphore           ct;
  VkSemaphoreCreateInfo cfg;
} cvk_Semaphore;


//______________________________________
// @section Fence
//____________________________

typedef struct cvk_Fence {
  VkFence           ct;
  VkFenceCreateInfo cfg;
} cvk_Fence;


//______________________________________
// @section Application Info
//____________________________

#define cvk_Application VkApplicationInfo


//______________________________________
// @section Validation
//____________________________

typedef struct cvk_validation_Options {
  cvk_Readonly cvk_bool                             layers_active;  ///< Will enable validation layers when true
  cvk_Readonly cvk_bool                             debug_active;   ///< Will enable debug messenger when true
  cvk_Readonly VkDebugUtilsMessageSeverityFlagsEXT  debug_severity;
  cvk_Readonly VkDebugUtilsMessageTypeFlagsEXT      debug_msgType;
  cvk_Readonly PFN_vkDebugUtilsMessengerCallbackEXT debug_callback;
  cvk_Readonly cvk_pointer                          debug_userdata;
} cvk_validation_Options;

typedef struct cvk_Validation {
  VkDebugUtilsMessengerEXT           debug_ct;
  VkDebugUtilsMessengerCreateInfoEXT debug_cfg;
  cvk_bool                           debug_active;
  cvk_bool                           layers_active;
} cvk_Validation;


//______________________________________
// @section Instance
//____________________________
typedef struct cvk_Instance {
  VkInstance           ct;
  VkInstanceCreateInfo cfg;
  cvk_Allocator        allocator;
  VkApplicationInfo    application;
  cvk_Validation       validation;
} cvk_Instance;

/// @description
/// Function shape expected by cvk to request the required extensions from the system.
/// @returns cvk_Slice[cvk_String] List of string literals with the names of the required extensions
typedef cvk_Slice (*cvk_Fn_extensions_Instance_getRequired)(void);


//______________________________________
// @section Surface
//____________________________
#include "./surface.h"  ///< Special case. cvulkan has no tools for Surface, only a typedef alias.


//______________________________________
// @section Device: Queue Families
//____________________________

/// @description
/// Describes a list of QueueFamily Properties.
/// Typed `cvk_Slice` for clarity of intention:
/// `VkQueueFamilyProperties[]` (aka: `cvk_Slice( VkQueueFamilyProperties )`)
typedef struct cvk_queueFamilies_properties_List {
  cvk_size                 len;
  cvk_Readonly cvk_size    itemsize;
  VkQueueFamilyProperties* ptr;
} cvk_queueFamilies_properties_List;

typedef cvk_Optional_u32 cvk_QueueID;

typedef struct cvk_QueueFamilies {
  cvk_queueFamilies_properties_List properties;
  cvk_QueueID                       graphics;  ///< ID of the first Graphics queue found on the device
  cvk_QueueID                       present;   ///< ID of the first queue with Surface Present support found on the device
  cvk_QueueID                       compute;   ///< ID of the first Compute queue found on the device
  cvk_QueueID                       transfer;  ///< ID of the first Transfer-only queue found with support on the device
  ///< @note Both Graphics and Compute queues can transfer by default
} cvk_QueueFamilies;


//______________________________________
// @section Device: Swapchain Support
//____________________________

/// @description
/// Describes a list of Swapchain formats.
/// Typed `cvk_Slice` for clarity of intention:
/// `VkSurfaceFormatKHR[]` (aka: `cvk_Slice( VkSurfaceFormatKHR )`)
typedef struct cvk_device_swapchain_Formats {
  cvk_size              len;
  cvk_Readonly cvk_size itemsize;
  VkSurfaceFormatKHR*   ptr;
} cvk_device_swapchain_Formats;

/// @description
/// Describes a list of Swapchain Present modes.
/// Typed `cvk_Slice` for clarity of intention:
/// `VkPresentModeKHR[]` (aka: `cvk_Slice( VkPresentModeKHR )`)
typedef struct cvk_device_swapchain_Modes {
  cvk_size              len;
  cvk_Readonly cvk_size itemsize;
  VkPresentModeKHR*     ptr;
} cvk_device_swapchain_Modes;

typedef struct cvk_device_swapchain_Support {
  VkSurfaceCapabilitiesKHR     capabilities;
  char                         priv_pad[4];
  cvk_device_swapchain_Formats formats;
  cvk_device_swapchain_Modes   modes;
} cvk_device_swapchain_Support;


//______________________________________
// @section Device: Physical
//____________________________
typedef struct cvk_device_Physical {
  VkPhysicalDevice                 ct;
  cvk_Optional_u32                 id;                ///< Identifier that represents its position in the devices list at the time of initialization
  char                             priv_pad[4];       //
  cvk_QueueFamilies                queueFamilies;     ///< QueueFamilies available on this device
  cvk_device_swapchain_Support     swapchainSupport;  ///< Swapchain Support features on this device
  VkPhysicalDeviceMemoryProperties memory;            ///< Memory Properties of this device
} cvk_device_Physical;
/// @description
/// Describes a list of VkPhysicalDevice contexts.
/// Used mainly to hold the list of devices available on the system.
/// Typed `cvk_Slice` for clarity of intention:
/// `VkPhysicalDevice[]` (aka: `cvk_Slice( VkPhysicalDevice )`)
typedef struct cvk_device_physical_List {
  cvk_size              len;
  cvk_Readonly cvk_size itemsize;
  VkPhysicalDevice*     ptr;
} cvk_device_physical_List;

typedef cvk_Pure cvk_bool (*cvk_Fn_device_physical_isSuitable)( // clang-format off
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
); // clang-format on


//______________________________________
// @section Device: Queue
//____________________________

/// Must be in range[0.0 .. 1.0]
typedef float cvk_QueuePriority;

typedef struct cvk_device_Queue {
  VkQueue                 ct;
  VkDeviceQueueCreateInfo cfg;
  cvk_QueuePriority       priority;
  char                    priv_pad[4];
} cvk_device_Queue;


//______________________________________
// @section Device: Extensions
//____________________________

/// @description
/// Typed `cvk_Slice` for clarity of intention:
/// Describes a list of (string literal) names of Device.Logical extensions.
/// `cvk_String[]` (aka: `cvk_Slice( char const* )`)
typedef struct cvk_device_Extensions {
  cvk_size              len;
  cvk_Readonly cvk_size itemsize;
  cvk_String*           ptr;
} cvk_device_Extensions;

/// @description
/// Describes a list of Extension Properties of a Device.Physical.
/// Typed `cvk_Slice` for clarity of intention:
/// `VkExtensionProperties[]` (aka: `cvk_Slice( VkExtensionProperties )`)
typedef struct cvk_device_extensions_Properties {
  cvk_size               len;
  cvk_Readonly cvk_size  itemsize;
  VkExtensionProperties* ptr;
} cvk_device_extensions_Properties;


//______________________________________
// @section Device: Swapchain
//____________________________

typedef struct cvk_device_swapchain_Image {
  VkImage       ct;
  VkImageView   view;
  cvk_Semaphore finished;
} cvk_device_swapchain_Image;

typedef struct cvk_device_swapchain_image_List {
  cvk_size                    len;
  cvk_Readonly cvk_size       itemsize;
  cvk_device_swapchain_Image* ptr;
} cvk_device_swapchain_image_List;

typedef struct cvk_device_Swapchain {
  VkSwapchainKHR                  ct;
  VkSwapchainCreateInfoKHR        cfg;
  cvk_device_swapchain_image_List images;
} cvk_device_Swapchain;


//______________________________________
// @section Device: Features
//____________________________

/// @description
/// Alias to unify the naming convention of cvulkan types
typedef struct cvk_device_Features {
  // ?? v1_3;
  // ?? v1_2;
  // ?? v1_1;
  VkPhysicalDeviceFeatures2 v1_0;
} cvk_device_Features;


//______________________________________
// @section Device: Logical
//____________________________

typedef struct cvk_device_Logical {
  VkDevice              ct;
  VkDeviceCreateInfo    cfg;
  cvk_device_Features   features;  // FIX: Move to Device.Physical
  cvk_device_Extensions extensions;
} cvk_device_Logical;


//______________________________________
// @section Framebuffer: Context
//____________________________

typedef struct cvk_Framebuffer {
  VkFramebuffer           ct;
  VkFramebufferCreateInfo cfg;
} cvk_Framebuffer;

typedef struct cvk_framebuffer_List {
  cvk_size              len;
  cvk_Readonly cvk_size itemsize;
  cvk_Framebuffer*      ptr;
} cvk_framebuffer_List;


//______________________________________
// @section RenderPass
//____________________________

typedef struct cvk_Renderpass {
  VkRenderPass           ct;
  VkRenderPassCreateInfo cfg;
} cvk_Renderpass;


//______________________________________
// @section Shaders
//____________________________

typedef struct cvk_SpirV {
  cvk_size              len;
  cvk_Readonly cvk_size itemsize;
  uint32_t*             ptr;
} cvk_SpirV;

typedef struct cvk_Shader {
  VkShaderModule                  ct;
  VkShaderModuleCreateInfo        cfg;
  VkPipelineShaderStageCreateInfo stage;
  cvk_SpirV                       code;
} cvk_Shader;


//______________________________________
// @section Pipeline: State
//____________________________

typedef struct cvk_pipeline_shaderStage_List {
  cvk_size                               len;
  cvk_Readonly cvk_size                  itemsize;
  VkPipelineShaderStageCreateInfo* const ptr;
} cvk_pipeline_shaderStage_List;


//______________________________________
// @section Pipeline: Layout
//____________________________

typedef struct cvk_pipeline_Layout {
  VkPipelineLayout           ct;
  VkPipelineLayoutCreateInfo cfg;
} cvk_pipeline_Layout;


//______________________________________
// @section Pipeline: Graphics Context
//____________________________

typedef struct cvk_pipeline_Graphics {
  VkPipeline                   ct;
  VkGraphicsPipelineCreateInfo cfg;
  cvk_pipeline_Layout          layout;
  cvk_Renderpass               renderpass;
} cvk_pipeline_Graphics;


//______________________________________
// @section Command Pool
//____________________________

typedef struct cvk_command_Pool {
  VkCommandPool           ct;
  VkCommandPoolCreateInfo cfg;
} cvk_command_Pool;

/// @description
/// Alias to `VkCommandPoolCreateFlags` for naming consistency.
/// Fully compatible with the original type.
typedef enum cvk_command_pool_Flags {
  cvk_command_pool_Transient     = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
  cvk_command_pool_Reset         = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
  cvk_command_pool_Protected     = VK_COMMAND_POOL_CREATE_PROTECTED_BIT,
  cvk_command_pool_Flags_Force32 = VK_COMMAND_POOL_CREATE_FLAG_BITS_MAX_ENUM,
} cvk_command_pool_Flags;


//______________________________________
// @section Command Buffer
//____________________________

typedef struct cvk_command_Buffer {
  VkCommandBuffer             ct;
  VkCommandBufferAllocateInfo cfg;
} cvk_command_Buffer;

/// @description
/// Alias to `VkCommandBufferLevel` for naming consistency.
/// Fully compatible with the original type.
typedef enum cvk_command_buffer_Level {
  cvk_command_buffer_Primary       = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
  cvk_command_buffer_Secondary     = VK_COMMAND_BUFFER_LEVEL_SECONDARY,
  cvk_command_buffer_Level_Force32 = VK_COMMAND_BUFFER_LEVEL_MAX_ENUM,
} cvk_command_buffer_Level;


//______________________________________
// @section Data
//____________________________

typedef struct cvk_Memory {
  VkDeviceMemory       ct;
  VkMemoryAllocateInfo cfg;
  cvk_pointer          data;
} cvk_Memory;

typedef struct cvk_buffer_Memory {
  VkMemoryRequirements requirements;
  uint32_t             kind;
  char                 priv_pad[4];
} cvk_buffer_Memory;

typedef struct cvk_Buffer {
  VkBuffer           ct;
  VkBufferCreateInfo cfg;
  cvk_buffer_Memory  memory;
} cvk_Buffer;


#endif  // H_cvulkan_types

