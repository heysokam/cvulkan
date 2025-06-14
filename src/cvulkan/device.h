//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
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


//______________________________________
// @section Device: Generic Create Arguments
//____________________________

typedef cvk_Optional_u32         cvk_QueueID;
typedef struct cvk_QueueFamilies cvk_QueueFamilies;
struct cvk_QueueFamilies {
  cvk_Slice /* VkQueueFamilyProperties[] */ properties;
  cvk_QueueID                               graphics;  ///< ID of the first Graphics queue found on the device
  cvk_QueueID                               present;   ///< ID of the first queue with Surface Present support found on the device
  cvk_QueueID                               compute;   ///< ID of the first Compute queue found on the device
  cvk_QueueID                               transfer;  ///< ID of the first Transfer-only queue found with support on the device
  ///< @note Both Graphics and Compute queues can transfer by default
};

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

cvk_Pure cvk_Slice /* VkPhysicalDevice[] */ cvk_device_physical_getAvailable (cvk_Instance* const instance);

typedef struct cvk_device_physical_create_args {
  cvk_Instance                                         instance;
  VkSurfaceKHR const                                   surface;
  cvk_Nullable cvk_Fn_device_physical_isSuitable const isSuitable;  ///< Called on every device found. Will use default when NULL
  cvk_bool const                                       forceFirst;  ///< Whether device selection should stop when the first device was found
  char                                                 priv_pad[4];
} cvk_device_physical_create_args;
cvk_Pure cvk_device_Physical cvk_device_physical_create (cvk_device_physical_create_args* const arg);
void                         cvk_device_physical_destroy (cvk_device_Physical* const device, cvk_Allocator* const allocator);


//______________________________________
// @section Device: Logical
//____________________________

typedef struct cvk_device_Logical {
  VkDevice           ct;
  VkDeviceCreateInfo cfg;
} cvk_device_Logical;


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_device
#endif
#ifdef cvk_Implementation_device
#include "./device.c"
#endif


#endif  // H_cvk_device

