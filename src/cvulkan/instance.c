//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "./result.h"
#include "./application.h"
#include "./allocator.h"
#include "./validation.h"
#include "./instance.h"


//______________________________________
// @section Instance: Options
//____________________________

cvk_Pure VkInstanceCreateInfo cvk_instance_options_create (
  cvk_Application const* const application,
  VkInstanceCreateFlags const  flags,
  cvk_Slice const              layers,
  cvk_Slice const              extensions
) {
  return (VkInstanceCreateInfo){
    .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext                   = NULL,
    .flags                   = flags,
    .pApplicationInfo        = application,
    .enabledLayerCount       = (uint32_t)layers.len,
    .ppEnabledLayerNames     = layers.ptr,
    .enabledExtensionCount   = (uint32_t)extensions.len,
    .ppEnabledExtensionNames = extensions.ptr,
  };
}


cvk_bool cvk_instance_layers_checkSupport (
  cvk_Slice const      required,
  cvk_Allocator* const allocator
) {
  if (!required.len) return cvk_true;

  cvk_Slice properties = cvk_Slice_empty();
  vkEnumerateInstanceLayerProperties((uint32_t*)&properties.len, NULL);
  if (properties.len) {
    properties = allocator->cpu.allocZ(&allocator->cpu, properties.len, sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties((uint32_t*)&properties.len, properties.ptr);
  }

  cvk_bool found = cvk_false;
  for (size_t layerId = 0; layerId < properties.len; ++layerId) {
    // clang-format off
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    VkLayerProperties const prop = ((VkLayerProperties*)properties.ptr)[layerId];
    #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
    // clang-format on
    for (size_t vlayerID = 0; vlayerID < required.len; ++vlayerID) {
      // clang-format off
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
      cvk_String const extension = ((cvk_String*)required.ptr)[vlayerID];
      #pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
      // clang-format on
      if (cvk_String_equal(prop.layerName, extension)) {
        found = cvk_true;
        goto end;
      }
    }
  }
end:
  allocator->cpu.free(&allocator->cpu, (cvk_Slice*)&properties);
  return found;
}


static void cvk_instance_layers_checkValidation (
  cvk_Allocator* const allocator
) {
  cvk_String      validation_layers[1] = { [0] = cvk_validation_LayerName };
  cvk_Slice const layers               = (cvk_Slice){ .ptr = &validation_layers, .len = 1 };  // clang-format off
  cvk_assert(cvk_instance_layers_checkSupport(layers, allocator),
    "Validation layers are not available in this system."
  );  // clang-format on
}


//______________________________________
// @section Instance: Initialize/Allocate/Create
//____________________________
cvk_Pure cvk_Instance cvk_instance_create (
  cvk_instance_create_args const* const arg
) {
  cvk_Instance result = (cvk_Instance){ 0 };
  // Create allocator
  result.allocator = (arg->allocator) ? *arg->allocator : cvk_Allocator_defaults();
  // Create appinfo
  result.application                    = cvk_Application_defaults();
  result.application.pApplicationName   = arg->app_name;
  result.application.applicationVersion = arg->app_version;
  result.application.pEngineName        = arg->engine_name;
  result.application.engineVersion      = arg->engine_version;
  // Create validation options
  //  : User options, or defaults when NULL
  cvk_validation_Options validation = (arg->validation) ? *arg->validation : cvk_validation_options_defaults();
  //  : Assign to the result
  result.validation = (cvk_Validation){
    .layers_active = validation.layers_active,
    .debug_active  = validation.debug_active,
    .debug_ct      = NULL,
    .debug_cfg     = cvk_validation_debug_options_defaults(),
  };
  if (result.validation.debug_active) {
    result.validation.debug_cfg.messageSeverity = validation.debug_severity;
    result.validation.debug_cfg.messageType     = validation.debug_msgType;
    result.validation.debug_cfg.pfnUserCallback = validation.debug_callback;
    result.validation.debug_cfg.pUserData       = validation.debug_userdata;
  }
  //  : Check that validation layers are supported when required
  if (result.validation.layers_active) cvk_instance_layers_checkValidation(&result.allocator);

  //
  // TODO: Layers
  //
  cvk_String layer_list[1] = { [0] = cvk_validation_LayerName };
  cvk_Slice  layers        = cvk_Slice_empty();
  layers.ptr               = &layer_list;
  layers.len               = 1;

  //
  // TODO:  Extensions
  //
  cvk_String extension_list[3] = {
    [0] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    // FIX: Should come from the user-provided extensions_getRequired function (eg: glfw)
    [1] = VK_KHR_SURFACE_EXTENSION_NAME,
    [2] = "VK_KHR_xcb_surface",  // VK_KHR_XCB_SURFACE_EXTENSION_NAME
    // TODO: Portability for mac
  };
  cvk_Slice extensions = cvk_Slice_empty();
  extensions.ptr       = &extension_list;
  extensions.len       = 3;

  // Create instance.cfg
  //  : with layers
  //  : with extensions
  //  : with debug.cfg as pNext
  result.cfg = cvk_instance_options_create(&result.application, 0, layers, extensions);
  if (result.validation.debug_active) result.cfg.pNext = (void*)&result.validation.debug_cfg;
  // Create instance.ct
  cvk_result_check(vkCreateInstance(&result.cfg, result.allocator.gpu, &result.ct), "Failed to create the Vulkan Instance.");
  // Create debug.ct
  if (result.validation.debug_active)
    result.validation.debug_ct = cvk_validation_debug_context_create(
      /* instance */ result.ct,
      /* cfg      */ &result.validation.debug_cfg,
      /* active   */ result.allocator.gpu
    );
  // Return the result
  return result;
}  //:: cvk_instance_create


//______________________________________
// @section Instance: Terminate/Deallocate/Destroy
//____________________________

void cvk_instance_destroy (
  cvk_Instance* const instance
) {
  // TODO: Free the Allocator's data
  // TODO: Deallocate the extensions
  cvk_validation_debug_context_destroy(instance->ct, instance->validation.debug_ct, instance->allocator.gpu);
  instance->cfg = (VkInstanceCreateInfo){ 0 };
  vkDestroyInstance(instance->ct, instance->allocator.gpu);
}  //:: cvk_instance_destroy

