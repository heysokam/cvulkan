//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
//! @fileoverview Vulkan Device: Types & Tools
//_____________________________________________|
#ifndef H_cvk_device
#define H_cvk_device
#include "./base.h"
#include "./types.h"


//______________________________________
// @section Device: Queue Families
//____________________________

/// @description
/// Creates a `Queue.Families` object with all the relevant queue families data for the given `Device.Physical`.
/// The `Queue.Families.Properties` data of the device will be allocated and added to the returning object.
///
/// The `Queue.ID` values are populated using this logic:
/// `.graphics` : has graphics
/// `.present`  : has surface support
/// `.transfer` : has transfer and not (graphics or compute)  [aka dedicated transfer]
/// `.compute`  : has compute and not graphics                [aka dedicated compute]
///
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_device_queue_families_destroy` using the same `allocator`,
cvk_Pure cvk_QueueFamilies cvk_device_queue_families_create ( // clang-format off
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
); // clang-format on

/// @description
/// Releases any memory and handles created by `cvk_device_queue_families_create` for the given families.
void cvk_device_queue_families_destroy ( // clang-format off
  cvk_QueueFamilies* const queueFamilies,
  cvk_Allocator* const     allocator
); // clang-format on


//______________________________________
// @section Device: Swapchain Support
//____________________________

/// @description
/// Returns the Swapchain support information for the given Device+Surface
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_device_swapchain_support_destroy` using the same `allocator`,
cvk_Pure cvk_device_swapchain_Support cvk_device_swapchain_support_create ( // clang-format off
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
); // clang-format on

/// @description
/// Releases any memory and handles created by `cvk_device_swapchain_support_create` using the same `allocator`.
void cvk_device_swapchain_support_destroy ( // clang-format off
  cvk_device_swapchain_Support* const support,
  cvk_Allocator* const                allocator
); // clang-format on


//______________________________________
// @section Device: Physical
//____________________________

/// @description
/// Default function used to decide a Device.Physical suitability during its creation.
/// @returns whether or not the device meets the conditions required for the application.
cvk_Pure cvk_bool cvk_device_physical_isSuitable_default ( // clang-format off
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
); // clang-format on

#ifndef cvk_device_physical_isSuitable
/// @description
/// Default function used to decide a Device.Physical suitability during its creation.
/// @see `cvk_device_physical_isSuitable_default` for more details.
/// @overridable Add `#define cvk_device_physical_isSuitable yourSuitableFunction` to your code to change this default behavior
#define cvk_device_physical_isSuitable cvk_device_physical_isSuitable_default
#endif

/// @description
/// Default function used to rank a Device.Physical desirability/features during its creation.
///
/// @note Devices that do not pass the `isSuitable` suitability check won't be considered for ranking.
/// @note Returning a value of `0` means that the device will not be considered for ranking.
/// @note The default function always returns `1`.
/// @note Tie Break: In case of equal scores, the first device found with that score will be selected.
/// @note Returning a value of `SIZE_MAX`, the first time it is returned, means: Pick this specific device and ignore every other device found after it.
///
/// @returns A score in `range[0..SIZE_MAX]`
cvk_Pure cvk_device_physical_Score cvk_device_physical_getScore_default ( // clang-format off
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_Allocator* const             allocator
); // clang-format on

#ifndef cvk_device_physical_getScore
/// @description
/// Default function used to rank a Device.Physical desirability/features during its creation.
/// @see `cvk_device_physical_getScore_default` for more details.
/// @overridable Add `#define cvk_device_physical_getScore yourRankingFunction` to your code to change this default behavior
#define cvk_device_physical_getScore cvk_device_physical_getScore_default
#endif

/// @description
/// Allocates a `cvk_device_physical_List` that contains all the devices available on the system.
/// The caller owns the memory allocated by this function,
/// and is responsible for freeing the result with the allocator contained in the given `instance`.
cvk_Pure cvk_device_physical_List cvk_device_physical_getAvailable (  // clang-format off
  cvk_Instance* const instance
);  // clang-format on

/// @description
/// Configuration options for `cvk_device_physical_create`.
typedef struct cvk_device_physical_create_args {
  cvk_Instance* const                                  instance;
  VkSurfaceKHR const                                   surface;
  cvk_Nullable cvk_Fn_device_physical_getScore const   getScore;    ///< Called on every device found. Will use default when NULL
  cvk_Nullable cvk_Fn_device_physical_isSuitable const isSuitable;  ///< Called on every device found. Will use default when NULL
  cvk_Nullable cvk_bool const                          forceFirst;  ///< Whether device selection should stop when the first device was found
  char                                                 priv_pad[4];
} cvk_device_physical_create_args;

/// @description
/// Searches for a valid `Device.Physical` object handle based on the configuration options at `cvk_device_physical_create_args`.
/// It also populates its `.properties`, `.features`, etc fields for ergonomics.
///
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_device_physical_destroy` using the same `allocator`,
/// All allocations are processed using the allocator at `arg.instance.allocator`.
cvk_Pure cvk_device_Physical cvk_device_physical_create (  // clang-format off
  cvk_device_physical_create_args* const arg
);  // clang-format on

/// @description
/// Releases all memory allocated by `cvk_device_physical_create`.
/// `NULL` handles won't trigger double destroy,
/// and `NULL` data wont cause double release errors
/// as long as the given `allocator.cpu` supports such scenario  (the default allocator supports this).
void cvk_device_physical_destroy ( // clang-format off
  cvk_device_Physical* const device,
  cvk_Allocator* const       allocator
); // clang-format on


//______________________________________
// @section Device: Queue
//____________________________

cvk_Pure VkDeviceQueueCreateInfo cvk_device_queue_options_create ( // clang-format off
  cvk_QueueID  family,
  uint32_t     count,
  float const* priorities,
  cvk_bool     Protected
); // clang-format on

/// @description
/// Configuration options for `cvk_device_queue_create`.
typedef struct cvk_device_queue_create_args {
  cvk_Instance const* const        instance;
  cvk_device_Physical const* const device;
  cvk_QueueID const                id;
  cvk_QueuePriority const          priority;
  cvk_bool const                   Protected;
  char                             priv_pad[4];
} cvk_device_queue_create_args;

/// @description
/// Creates all the data required for creating a Device.Queue, except for its `.ct` context/handle field.
/// Must be called before `cvk_device_queue_create_context`.
///
/// @note:
/// The (`cvk_queue_create_noContext`, `cvk_queue_create_context`) pair of functions exists to solve a cyclic dependency.
/// The Queue's `.cfg` field must be used when creating a Device.Logical.context.
/// But a creating a Queue.context requires a Device.Logical.context.
///
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_device_queue_destroy` using the same `allocator`,
cvk_Pure cvk_device_Queue cvk_device_queue_create_noContext (  // clang-format off
  cvk_device_queue_create_args* const arg
);  // clang-format on

/// @description
/// Assigns the context field of the given `queue`.
/// Must be called after `cvk_device_queue_create_noContext`.
/// @see `cvk_device_queue_create_noContext` for more info.
void cvk_device_queue_create_context ( // clang-format off
  cvk_device_Queue* const         queue,
  cvk_device_Logical const* const device
); // clang-format on

// clang-format off
/// @note Queue objects currently don't hold anything that needs to be destroyed
#define cvk_device_queue_destroy(ct, A) (void)(ct); (void)(A)
// clang-format on

typedef struct cvk_device_queue_submit_args {
  cvk_command_Buffer const* const         command_buffer;
  cvk_Nullable cvk_Semaphore const* const semaphore_wait;
  cvk_Nullable cvk_Semaphore const* const semaphore_signal;
  cvk_Nullable cvk_Fence const* const     fence;
} cvk_device_queue_submit_args;

void cvk_device_queue_submit ( // clang-format off
  cvk_device_Queue const* const             queue,
  cvk_device_queue_submit_args const* const arg
); // clang-format on

void cvk_device_queue_wait (  // clang-format off
  cvk_device_Queue const* const queue
);  // clang-format on


//______________________________________
// @section Device: Extensions
//____________________________

/// @description
/// Allocates the list of Device.Logical extensions used by this library by default.
/// The caller is responsible for freeing the result with the given {@arg allocator}.
cvk_Pure cvk_device_Extensions cvk_device_Extensions_default (cvk_Allocator* const allocator);

/// @description
/// Allocates the list of Device.Physical Extension Properties available on this system.
/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_device_extensions_properties_destroy` using the same `allocator`,
cvk_Pure cvk_device_extensions_Properties cvk_device_extensions_properties_create ( // clang-format off
  cvk_device_Physical const* const device,
  cvk_Allocator* const             allocator
); // clang-format on

void cvk_device_extensions_properties_destroy ( // clang-format off
  cvk_device_extensions_Properties properties,
  cvk_Allocator* const             allocator
); // clang-format on

cvk_Pure cvk_bool cvk_device_extensions_supported ( // clang-format off
  cvk_device_Physical const* const device,
  cvk_device_Extensions const      extensions,
  cvk_Allocator* const             allocator
); // clang-format on


//______________________________________
// @section Device: Swapchain
//____________________________

/// @description
/// Returns the preferred format for the Swapchain Surface
cvk_Pure VkSurfaceFormatKHR cvk_device_swapchain_select_format (  // clang-format off
  cvk_device_swapchain_Support const* const support
);  // clang-format on

/// @description
/// Returns the preferred present mode for the Swapchain Surface
/// @note Searches for Mailbox support first, and returns FIFO if not found
cvk_Pure VkPresentModeKHR cvk_device_swapchain_select_mode (  // clang-format off
  cvk_device_swapchain_Support const* const support
);  // clang-format on

/// @description
/// Returns the size of the Swapchain Surface
/// @note Measurements in pixels/units are compared, in case they don't match
cvk_Pure VkExtent2D cvk_device_swapchain_select_size ( // clang-format off
  cvk_device_swapchain_Support const* const support,
  cvk_Size2D const                          size
); // clang-format on

/// @description
/// Returns the minimum number of images that the Swapchain will contain
cvk_Pure cvk_size cvk_device_swapchain_select_imgMin (  // clang-format off
  cvk_device_swapchain_Support const* const support
);  // clang-format on

cvk_Pure VkSwapchainCreateInfoKHR cvk_device_swapchain_options_create (
  cvk_device_Physical const* const device,
  cvk_Surface const                surface,
  cvk_size const                   img_min,
  VkSurfaceFormatKHR const         format,
  cvk_Size2D const                 size,
  VkSurfaceCapabilitiesKHR* const  capabilities,
  VkPresentModeKHR const           mode,
  cvk_Allocator* const             allocator
);

cvk_Pure VkAttachmentDescription cvk_device_swapchain_attachment_cfg (  // clang-format off
  cvk_device_Swapchain const* const swapchain
);  // clang-format on

/// @description
/// Configuration options for `cvk_device_swapchain_image_view_create`.
typedef struct cvk_device_swapchain_image_view_create_args {
  cvk_device_Logical const* const device_logical;
  VkImage const                   image;
  cvk_Allocator const* const      allocator;
} cvk_device_swapchain_image_view_create_args;

cvk_Pure VkImageView cvk_device_swapchain_image_view_create ( // clang-format off
  cvk_device_Swapchain const* const                        swapchain,
  cvk_device_swapchain_image_view_create_args const* const arg
); // clang-format on

/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_device_swapchain_image_list_destroy` using the same `allocator`,
cvk_Pure cvk_device_swapchain_image_List cvk_device_swapchain_image_list_create ( // clang-format off
  cvk_device_Swapchain const* const swapchain,
  cvk_device_Logical* const         device_logical,
  cvk_Allocator* const              allocator
); // clang-format on

void cvk_device_swapchain_image_list_destroy ( // clang-format off
  cvk_device_swapchain_image_List* const images,
  cvk_device_Logical* const              device_logical,
  cvk_Allocator* const                   allocator
); // clang-format on

/// @description
/// Configuration options for `cvk_device_swapchain_create`.
typedef struct cvk_device_swapchain_create_args {
  cvk_device_Physical* const device_physical;
  cvk_device_Logical* const  device_logical;
  cvk_Surface const          surface;
  cvk_Size2D const           size;
  cvk_Allocator* const       allocator;
} cvk_device_swapchain_create_args;

/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_device_swapchain_destroy` using the same `allocator`,
cvk_Pure cvk_device_Swapchain cvk_device_swapchain_create (  // clang-format off
  cvk_device_swapchain_create_args const*const arg
);  // clang-format on

void cvk_device_swapchain_destroy ( // clang-format off
  cvk_device_Swapchain* const swapchain,
  cvk_device_Logical* const   device,
  cvk_Allocator* const        allocator
); // clang-format on

typedef struct cvk_device_swapchain_recreate_args {
  cvk_device_Logical* const device_logical;
  cvk_Allocator* const      allocator;
} cvk_device_swapchain_recreate_args;

void cvk_device_swapchain_recreate ( // clang-format off
  cvk_device_Swapchain* const                     swapchain,
  cvk_device_swapchain_recreate_args const* const arg
); // clang-format on

typedef struct cvk_device_swapchain_nextImageID_args {
  cvk_device_Logical const* const         device_logical;
  cvk_device_Swapchain const* const       swapchain;
  cvk_Nullable cvk_Semaphore const* const semaphore;
  cvk_Nullable cvk_Fence const* const     fence;
  cvk_Nullable VkResult* const            status;
  cvk_Nullable cvk_bool const             log_disable;  ///< Disables internal cvulkan logging inside the function.
  char                                    priv_pad[4];
} cvk_device_swapchain_nextImageID_args;

cvk_Pure cvk_size cvk_device_swapchain_nextImageID (  // clang-format off
  cvk_device_swapchain_nextImageID_args const* const arg
);  // clang-format on

void cvk_device_swapchain_present ( // clang-format off
  cvk_device_Swapchain const* const swapchain,
  cvk_size const                    imageID,
  cvk_device_Queue const* const     queue
); // clang-format on


//______________________________________
// @section Device: Features
//____________________________

cvk_Pure cvk_device_Features cvk_device_Features_default ();


//______________________________________
// @section Device: Logical
//____________________________

cvk_Pure VkDeviceCreateInfo cvk_device_logical_options_create ( // clang-format off
  cvk_device_Queue const* const    queue,
  cvk_device_Features const* const features,
  cvk_device_Extensions const      extensions
); // clang-format on

/// @description
/// Configuration options for `cvk_device_logical_create`.
typedef struct cvk_device_logical_create_args {
  cvk_device_Physical const* const physical;
  cvk_device_Queue const* const    queue;
  cvk_Allocator* const             allocator;
} cvk_device_logical_create_args;

/// The caller owns the memory allocated by this function,
/// and is responsible for calling `cvk_device_logical_destroy` using the same `allocator`,
cvk_Pure cvk_device_Logical cvk_device_logical_create (  // clang-format off
  cvk_device_logical_create_args* const arg
);  // clang-format on

void cvk_device_logical_destroy (  // clang-format off
  cvk_device_Logical* const device,
  cvk_Allocator* const      allocator
);  // clang-format on

void cvk_device_logical_wait (  // clang-format off
  cvk_device_Logical const* const device_logical
);  // clang-format on


//______________________________________
// @section Single Header Support
//____________________________
#ifdef cvk_Implementation
#define cvk_Implementation_device
#endif
#ifdef cvk_Implementation_device
#include "./device/queue.c"
#include "./device/swapchain.c"
#include "./device/physical.c"
#include "./device/extensions.c"
#include "./device/features.c"
#include "./device/logical.c"
#endif


#endif  // H_cvk_device

