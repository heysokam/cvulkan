//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../result.h"
#include "../device.h"
#include <stdio.h>

static cvk_String const cvk_device_extensions_defaults[1] = {
  [0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

cvk_Pure cvk_device_extensions_Required cvk_device_extensions_required_defaults () {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
  return (cvk_device_extensions_Required){
    .user    = (cvk_device_Extensions){ 0 },
    .cvulkan = &(cvk_device_Extensions){ .ptr = (cvk_String*)cvk_device_extensions_defaults, .len = 1 },
  };
#pragma GCC diagnostic pop  // -Wcast-qual
}


cvk_Pure cvk_device_extensions_Properties cvk_device_extensions_properties_create (
  cvk_device_Physical const* const device,
  cvk_Allocator* const             allocator
) {
  cvk_device_extensions_Properties result = (cvk_device_extensions_Properties){ .itemsize = sizeof(VkExtensionProperties) };
  // clang-format off
  cvk_result_check(vkEnumerateDeviceExtensionProperties(device->ct, NULL, (uint32_t*)&result.len, NULL),
    "Failed when searching for Physical Device properties.");
  if (result.len) {
    cvk_Slice data = allocator->cpu.allocZ(&allocator->cpu, result.len, result.itemsize);
    cvk_result_check(vkEnumerateDeviceExtensionProperties(device->ct, NULL, (uint32_t*)&data.len, data.ptr),
      "Failed to retrieve the list of Physical Device properties.");
    result.ptr = (VkExtensionProperties*)data.ptr;
  }  // clang-format on
  return result;
}


void cvk_device_extensions_properties_destroy (
  cvk_device_extensions_Properties properties,
  cvk_Allocator* const             allocator
) {
  allocator->cpu.free(&allocator->cpu, (cvk_Slice*)&properties);
}


cvk_Pure cvk_bool cvk_device_extensions_supported (
  cvk_device_Physical const* const   device,
  cvk_device_Extensions const* const required,
  cvk_Allocator* const               allocator
) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  cvk_bool result = cvk_true;
  if (!required || !required->ptr) return result;  // Always true for no extensions
  // Get the extension properties of the device
  cvk_device_extensions_Properties properties = cvk_device_extensions_properties_create(device, allocator);
  // For every available extension, check if any of the extension properties has an entry with the same `.extensionName`
  for (cvk_size ext_id = 0; ext_id < required->len; ++ext_id) {
    cvk_bool found = cvk_false;
    for (size_t prop_id = 0; prop_id < properties.len; ++prop_id) {
      cvk_String name_ext  = ((cvk_String*)required->ptr)[ext_id];
      cvk_String name_prop = ((VkExtensionProperties*)properties.ptr)[prop_id].extensionName;
      if (cvk_String_equal(name_ext, name_prop)) {
        found = cvk_true;
        break;  // We found the extension. Stop iterating properties. Break the inner loop, and go to the next extension
      }  //:: if property.name = extension.name
      // else: Not found yet, keep searching
    }  //:: for every property
    if (!found) {
      // One of the extensions was not found. Stop searching other extensions and return false
      result = cvk_false;
      break;
    }
  }
  // Cleanup and return
  cvk_device_extensions_properties_destroy(properties, allocator);
  return result;
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
}


cvk_Pure cvk_device_Extensions cvk_device_extensions_create (
  cvk_device_extensions_Required const* const    required,
  cvk_device_extensions_create_args const* const arg
) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
#pragma GCC diagnostic ignored "-Wcast-qual"
  cvk_device_Extensions          result   = (cvk_device_Extensions){ 0 };
  cvk_device_extensions_Required list     = (required) ? *required : (cvk_device_extensions_Required){ 0 };
  cvk_device_Extensions          defaults = /* clang-format off */ (list.cvulkan) ? *list.cvulkan : (cvk_device_Extensions){
    .ptr = (cvk_String*)cvk_device_extensions_defaults,
    .len = sizeof(cvk_device_extensions_defaults) / sizeof(*cvk_device_extensions_defaults),
  };  // clang-format on
  cvk_size current = 0;

  // WARN:
  // Does not filter out extensions that are not supported by the device
  // They are meant to be checked in the `isSuitable` Device.Physical callback

  // Find the maximum length
  result.len += list.user.len;
  result.len += defaults.len;

  // Allocate space for all the extensions
  cvk_Slice data = arg->allocator->cpu.allocZ(&arg->allocator->cpu, result.len, sizeof(cvk_String));

  // Add the user extensions
  for (cvk_size id = 0; id < list.user.len; ++id) {
    ((cvk_StringList)data.ptr)[current] = arg->allocator->cpu.string_duplicate(&arg->allocator->cpu, list.user.ptr[id]);
    current += 1;
  }

  // Add the cvulkan extensions
  for (cvk_size id = 0; id < defaults.len; ++id) {
    ((cvk_StringList)data.ptr)[current] = arg->allocator->cpu.string_duplicate(&arg->allocator->cpu, defaults.ptr[id]);
    current += 1;
  }

  // Cleanup and Return the result
  arg->allocator->cpu.resize(&arg->allocator->cpu, &data, current);  // Resize down to the final length
  result.ptr = data.ptr;
  result.len = data.len;
  return result;
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
}


void cvk_device_extensions_destroy (
  cvk_device_Extensions* const extensions,
  cvk_Allocator* const         allocator
) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
#pragma GCC diagnostic ignored "-Wcast-qual"
  for (cvk_size id = 0; id < extensions->len; ++id) {
    cvk_size len = strlen(extensions->ptr[id]);
    char*    ptr = (char*)(extensions->ptr)[id];
    allocator->cpu.free(&allocator->cpu, &(cvk_Slice){ .len = len, .itemsize = sizeof(char), .ptr = (cvk_pointer)ptr });
  }
  allocator->cpu.free(&allocator->cpu, &(cvk_Slice){ .ptr = (cvk_pointer)extensions->ptr, .len = extensions->len, .itemsize = sizeof(cvk_String) });
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage -Wcast-qual
}


