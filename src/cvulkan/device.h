//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
//! @fileoverview Vulkan Device: Types & Tools
//_____________________________________________|
#ifndef H_cvk_device
#define H_cvk_device
#include "./base.h"
#include "./memory.h"
#include "./instance.h"
#include "./surface.h"


//______________________________________
// @section Forward declares for this file
//____________________________

//
typedef struct cvk_device_Physical cvk_device_Physical;
//
typedef struct cvk_device_Logical cvk_device_Logical;


//______________________________________
// @section Device: Queue Families
//____________________________

/// @description
/// Describes a list of QueueFamily Properties.
/// Typed `cvk_Slice` for clarity of intention:
/// `VkQueueFamilyProperties[]` (aka: `cvk_Slice( VkQueueFamilyProperties )`)
typedef struct cvk_queueFamilies_properties_List {
  cvk_size                 len;
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

cvk_Pure cvk_QueueFamilies cvk_queue_families_create ( // clang-format off
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
); // clang-format on
void cvk_queue_families_destroy ( // clang-format off
  cvk_QueueFamilies* const queueFamilies,
  cvk_Allocator* const     allocator
); // clang-format on


//______________________________________
// @section Device: Physical
//____________________________
struct cvk_device_Physical {
  VkPhysicalDevice  ct;
  cvk_Optional_u32  id;             ///< Identifier that represents its position in the devices list at the time of initialization
  char              priv_pad[4];    //
  cvk_QueueFamilies queueFamilies;  ///< Data of the QueueFamilies available on this device
};
/// @description
/// Describes a list of VkPhysicalDevice contexts.
/// Used mainly to hold the list of devices available on the system.
/// Typed `cvk_Slice` for clarity of intention:
/// `VkPhysicalDevice[]` (aka: `cvk_Slice( VkPhysicalDevice )`)
typedef struct cvk_device_physical_List {
  cvk_size          len;
  VkPhysicalDevice* ptr;
} cvk_device_physical_List;

typedef cvk_Pure cvk_bool (*cvk_Fn_device_physical_isSuitable)( // clang-format off
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
); // clang-format on

cvk_Pure cvk_bool cvk_device_physical_isSuitable_default ( // clang-format off
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
); // clang-format on

#ifndef cvk_device_physical_isSuitable
#define cvk_device_physical_isSuitable cvk_device_physical_isSuitable_default
#endif

/// @description
/// Allocates a `cvk_device_physical_List` that contains all the devices available on the system.
/// The caller is responsible for freeing the result with the allocator contained in the {@arg instance}.
cvk_Pure cvk_device_physical_List cvk_device_physical_getAvailable (cvk_Instance* const instance);

typedef struct cvk_device_physical_create_args {
  cvk_Instance* const                                  instance;
  VkSurfaceKHR const                                   surface;
  cvk_Nullable cvk_Fn_device_physical_isSuitable const isSuitable;  ///< Called on every device found. Will use default when NULL
  cvk_bool const                                       forceFirst;  ///< Whether device selection should stop when the first device was found
  char                                                 priv_pad[4];
} cvk_device_physical_create_args;
cvk_Pure cvk_device_Physical cvk_device_physical_create (cvk_device_physical_create_args* const arg);
void                         cvk_device_physical_destroy (cvk_device_Physical* const device, cvk_Allocator* const allocator);


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

typedef struct cvk_device_queue_create_args {
  cvk_Instance const* const        instance;
  cvk_device_Physical const* const device;
  cvk_QueueID const                id;
  cvk_QueuePriority const          priority;
  cvk_bool const                   Protected;
  char                             priv_pad[4];
} cvk_device_queue_create_args;
cvk_Pure VkDeviceQueueCreateInfo cvk_device_queue_options_create ( // clang-format off
  cvk_QueueID  family,
  uint32_t     count,
  float const* priorities,
  cvk_bool     Protected
); // clang-format on

/// @description
/// Creates all the data required for creating a Device.Queue, except for its `.ct` context field.
/// Must be called before `cvk_device_queue_create_context`
/// @note:
/// The (`queue_create_noContext`, `queue_create_context`) pair of functions exists to solve a cyclic dependency.
/// The Queue's `.cfg` field must be used when creating a Device.Logical.context.
/// But a creating a Queue.context requires a Device.Logical.context.
cvk_Pure cvk_device_Queue cvk_device_queue_create_noContext (cvk_device_queue_create_args* const arg);

/// @description
/// Assigns the context field of the given {@arg queue}.
/// Must be called after `cvk_device_queue_create_noContext`.
/// @see {@link cvk_device_queue_create_noContext} for more info.
void cvk_device_queue_create_context ( // clang-format off
  cvk_device_Queue* const         queue,
  cvk_device_Logical const* const device
); // clang-format on

// clang-format off
/// @note Queue objects currently don't hold anything that needs to be destroyed
#define cvk_device_queue_destroy(ct, A) (void)(ct); (void)(A)
// clang-format on


//______________________________________
// @section Device: Extensions
//____________________________

/// @description
/// Typed `cvk_Slice` for clarity of intention:
/// Describes a list of (string literal) names of Device.Logical extensions.
/// `cvk_String[]` (aka: `cvk_Slice( char const* )`)
typedef struct cvk_device_Extensions {
  cvk_size    len;
  cvk_String* ptr;
} cvk_device_Extensions;
/// @description
/// Allocates the list of Device.Logical extensions used by this library by default.
/// The caller is responsible for freeing the result with the given {@arg allocator}.
cvk_Pure cvk_device_Extensions cvk_device_Extensions_default (cvk_Allocator* const allocator);

/// @description
/// Describes a list of Extension Properties of a Device.Physical.
/// Typed `cvk_Slice` for clarity of intention:
/// `VkExtensionProperties[]` (aka: `cvk_Slice( VkExtensionProperties )`)
typedef struct cvk_device_extensions_Properties {
  cvk_size               len;
  VkExtensionProperties* ptr;
} cvk_device_extensions_Properties;
/// @description
/// Allocates the list of Device.Physical Extension Properties available on this system.
/// The caller is responsible for freeing the result with the given {@arg allocator}.
cvk_Pure cvk_device_extensions_Properties cvk_device_extensions_properties_create ( // clang-format off
  cvk_device_Physical const* const device,
  cvk_Allocator* const             allocator
); // clang-format on

cvk_Pure cvk_bool cvk_device_extensions_supported ( // clang-format off
  cvk_device_Physical const* const device,
  cvk_device_Extensions const      extensions,
  cvk_Allocator* const             allocator
); // clang-format on


//______________________________________
// @section Device: Swapchain Support
//____________________________

/// @description
/// Describes a list of Swapchain formats.
/// Typed `cvk_Slice` for clarity of intention:
/// `VkSurfaceFormatKHR[]` (aka: `cvk_Slice( VkSurfaceFormatKHR )`)
typedef struct cvk_device_swapchain_Formats {
  cvk_size            len;
  VkSurfaceFormatKHR* ptr;
} cvk_device_swapchain_Formats;

/// @description
/// Describes a list of Swapchain Present modes.
/// Typed `cvk_Slice` for clarity of intention:
/// `VkPresentModeKHR[]` (aka: `cvk_Slice( VkPresentModeKHR )`)
typedef struct cvk_device_swapchain_Modes {
  cvk_size          len;
  VkPresentModeKHR* ptr;
} cvk_device_swapchain_Modes;

typedef struct cvk_device_swapchain_Support {
  VkSurfaceCapabilitiesKHR     capabilities;
  cvk_device_swapchain_Formats formats;
  char                         priv_pad[4];
  cvk_device_swapchain_Modes   modes;
} cvk_device_swapchain_Support;


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
cvk_Pure cvk_device_Features cvk_device_Features_default ();


//______________________________________
// @section Device: Logical
//____________________________

struct cvk_device_Logical {
  VkDevice              ct;
  VkDeviceCreateInfo    cfg;
  cvk_device_Features   features;  // FIX: Move to Device.Physical
  cvk_device_Extensions extensions;
};

cvk_Pure VkDeviceCreateInfo cvk_device_logical_options_create ( // clang-format off
  cvk_device_Queue const* const    queue,
  cvk_device_Features const* const features,
  cvk_device_Extensions const      extensions
); // clang-format on

typedef struct cvk_device_logical_create_args {
  cvk_device_Physical const* const physical;
  cvk_device_Queue const* const    queue;
  cvk_Allocator* const             allocator;
} cvk_device_logical_create_args;
cvk_Pure cvk_device_Logical cvk_device_logical_create (cvk_device_logical_create_args* const arg);
void cvk_device_logical_destroy ( // clang-format off
  cvk_device_Logical* const device,
  cvk_Allocator* const      allocator
); // clang-format on


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_device
#endif
#ifdef cvk_Implementation_device
#include "./device/queue.c"
#include "./device/physical.c"
#include "./device/extensions.c"
#include "./device/features.c"
#include "./device/logical.c"
#endif


#endif  // H_cvk_device

