//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
#include "../result.h"
#include "../device.h"


cvk_Pure cvk_device_Extensions cvk_device_Extensions_default (
  cvk_Allocator* const allocator
) {
  cvk_device_Extensions result = (cvk_device_Extensions){ .len = 1 };
  cvk_Slice             data   = allocator->cpu.allocZ(&allocator->cpu, result.len, sizeof(cvk_String));
  result.ptr                   = (cvk_String*)data.ptr;
  result.ptr[0]                = (void*)VK_KHR_SWAPCHAIN_EXTENSION_NAME;
  return result;
}


cvk_Pure cvk_device_extensions_Properties cvk_device_extensions_properties_create (
  cvk_device_Physical const* const device,
  cvk_Allocator* const             allocator
) {
  cvk_device_extensions_Properties result = (cvk_device_extensions_Properties){ 0 };
  // clang-format off
  cvk_result_check(vkEnumerateDeviceExtensionProperties(device->ct, NULL, (uint32_t*)&result.len, NULL),
    "Failed when searching for Physical Device properties.");
  if (result.len) {
    cvk_Slice data = allocator->cpu.allocZ(&allocator->cpu, result.len, sizeof(VkExtensionProperties));
    cvk_result_check(vkEnumerateDeviceExtensionProperties(device->ct, NULL, (uint32_t*)&data.len, data.ptr),
      "Failed to retrieve the list of Physical Device properties.");
    result.ptr = (VkExtensionProperties*)data.ptr;
  }  // clang-format on
  return result;
}


void cvk_device_extensions_properties_destroy ( // clang-format off
  cvk_device_extensions_Properties properties,
  cvk_Allocator* const             allocator
); // clang-format on

void cvk_device_extensions_properties_destroy (
  cvk_device_extensions_Properties properties,
  cvk_Allocator* const             allocator
) {
  allocator->cpu.free(&allocator->cpu, (cvk_Slice*)&properties);
}


cvk_Pure cvk_bool cvk_device_extensions_supported (
  cvk_device_Physical const* const device,
  cvk_device_Extensions const      extensions,
  cvk_Allocator* const             allocator
) {
  cvk_bool result = cvk_true;
  // Get the extension properties of the device
  cvk_device_extensions_Properties properties = cvk_device_extensions_properties_create(device, allocator);
  // For every available extension, check if any of the extension properties has an entry with the same `.extensionName`
  for (cvk_size ext_id = 0; ext_id < extensions.len; ++ext_id) {
    cvk_bool found = cvk_false;
    for (size_t prop_id = 0; prop_id < properties.len; ++prop_id) {  // clang-format off
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
      cvk_String ext_name  = ((cvk_String*)extensions.ptr)[ext_id];
      cvk_String prop_name = ((VkExtensionProperties*)properties.ptr)[prop_id].extensionName;
      #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
      // clang-format on
      if (cvk_String_equal(ext_name, prop_name)) {
        found = cvk_true;
        break;  // Stop iterating properties when the extension has been found
      }
      // Hasn't been found yet, keep searching
    }
    if (!found) {
      // One of the extensions was not found. Stop searching other extensions and return false
      result = cvk_false;
      break;
    }
  }
  // Cleanup and return
  cvk_device_extensions_properties_destroy(properties, allocator);
  return result;
}

