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
  cvk_Application const* const  application,
  VkInstanceCreateFlags const   flags,
  cvk_instance_Layers const     layers,
  cvk_instance_Extensions const extensions
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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  if (!required.len) return cvk_true;

  cvk_Slice properties = cvk_Slice_empty();
  vkEnumerateInstanceLayerProperties((uint32_t*)&properties.len, NULL);
  if (properties.len) {
    properties = allocator->cpu.allocZ(&allocator->cpu, properties.len, sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties((uint32_t*)&properties.len, properties.ptr);
  }

  cvk_bool found = cvk_false;
  for (size_t layerId = 0; layerId < properties.len; ++layerId) {
    VkLayerProperties const prop = ((VkLayerProperties*)properties.ptr)[layerId];
    for (size_t vlayerID = 0; vlayerID < required.len; ++vlayerID) {
      cvk_String const extension = ((cvk_String*)required.ptr)[vlayerID];
      if (cvk_String_equal(prop.layerName, extension)) {
        found = cvk_true;
        goto end;
      }
    }
  }
end:
  allocator->cpu.free(&allocator->cpu, (cvk_Slice*)&properties);
  return found;
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
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


static cvk_Pure cvk_instance_Layers cvk_instance_layers_defaults (
  cvk_bool const       validation_active,
  cvk_Allocator* const allocator
) {
  // @important
  // Assumes that the user will send us the validation layer,
  // when they request any custom layers at all
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  cvk_instance_Layers result = (cvk_instance_Layers){ 0 };
  if (!validation_active) return result;
  // Add validation layer
  cvk_Slice data             = allocator->cpu.allocZ(&allocator->cpu, 1, sizeof(cvk_StringList));
  ((cvk_String*)data.ptr)[0] = cvk_validation_LayerName;
  // Return the result
  result.ptr = data.ptr;
  result.len = data.len;
  return result;
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
}


//______________________________________
// @section Instance: Extensions
//____________________________

static cvk_instance_Extensions const cvk_instance_extensions_Defaults = (cvk_instance_Extensions){
  .len = 0,
  .ptr = NULL,
};


typedef struct cvk_instance_extensions_merge_args {
  cvk_bool const       debug_active;
  cvk_bool const       portability;
  cvk_Allocator* const allocator;
} cvk_instance_extensions_merge_args;

/// @description
/// Merges the given `list` of extensions into the result, using the given `arg.allocator`.
/// @param `list` Lists of extensions required when creating an instance for the application.
/// @param `arg.debug_active` Will add the DebugMessenger extension when true. Ignored when `list.cvulkan` has a value (aka `list.cvulkan != NULL`)
/// @param `arg.portability` Will add the PortabilityEnumeration extension when true. Ignored when `list.cvulkan` has a value (aka `list.cvulkan != NULL`)
static cvk_Pure cvk_instance_Extensions cvk_instance_extensions_merge (
  cvk_instance_extensions_Required const          list,
  cvk_instance_extensions_merge_args const* const arg
) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  cvk_instance_Extensions result   = (cvk_instance_Extensions){ 0 };
  cvk_instance_Extensions defaults = (list.cvulkan) ? *list.cvulkan : cvk_instance_extensions_Defaults;
  cvk_size                start    = 0;

  // FIX: Filter out extensions not supported by the system

  // Find the final length
  result.len += list.system.len;
  result.len += list.user.len;
  result.len += defaults.len;
  if (!list.cvulkan) {  // Only add the independent defaults when the user did not override the list
    result.len += (arg->debug_active) ? 1 : 0;
    result.len += (arg->portability) ? 1 : 0;
  }

  // Allocate space for all the extensions
  cvk_Slice data = arg->allocator->cpu.alloc(&arg->allocator->cpu, result.len, sizeof(cvk_String));

  // Add the system extensions
  arg->allocator->cpu.copy(
    /* A   */ &arg->allocator->cpu,
    /* src */ &(cvk_Slice){ .ptr = list.system.ptr, .len = list.system.len, .itemsize = data.itemsize },
    /* trg */ &(cvk_Slice){ .ptr = (cvk_String*)data.ptr + start, .len = list.system.len, .itemsize = data.itemsize }
  );
  start += list.system.len;

  // Add the user extensions
  arg->allocator->cpu.copy(
    /* A   */ &arg->allocator->cpu,
    /* src */ &(cvk_Slice){ .ptr = list.user.ptr, .len = list.user.len, .itemsize = data.itemsize },
    /* trg */ &(cvk_Slice){ .ptr = (cvk_String*)data.ptr + start, .len = list.user.len, .itemsize = data.itemsize }
  );
  start += list.user.len;

  // Add the cvulkan extensions
  arg->allocator->cpu.copy(
    /* A   */ &arg->allocator->cpu,
    /* src */ &(cvk_Slice){ .ptr = defaults.ptr, .len = defaults.len, .itemsize = data.itemsize },
    /* trg */ &(cvk_Slice){ .ptr = (cvk_String*)data.ptr + start, .len = defaults.len, .itemsize = data.itemsize }
  );
  start += defaults.len;
  if (!list.cvulkan) {  // Only add the independent defaults when the user did not override the list
    if (arg->debug_active) {
      ((cvk_String*)data.ptr)[start] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
      start += 1;
    }
    if (arg->portability) {
      ((cvk_String*)data.ptr)[start] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
      start += 1;
    }
  }

  // Return the result
  result.ptr = data.ptr;
  result.len = data.len;
  return result;
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
}


//______________________________________
// @section Instance: Initialize/Allocate/Create
//____________________________
cvk_Pure cvk_Instance cvk_instance_create (
  cvk_instance_create_args const* const arg
) {
  cvk_Instance result = (cvk_Instance){ 0 };
  // User options, or defaults when NULL
  result.allocator   = (arg->allocator) ? *arg->allocator : cvk_allocator_defaults();
  result.application = (arg->application) ? *arg->application : cvk_application_defaults();
  // User validation options, or use the defaults when NULL
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

  // Layers
  result.layers = (arg->layers) ? *arg->layers : cvk_instance_layers_defaults(result.validation.layers_active, &result.allocator);
  // Extensions
  result.extensions = /* clang-format off */ cvk_instance_extensions_merge(arg->extensions, &(cvk_instance_extensions_merge_args){
    .debug_active = result.validation.debug_active,
    .portability  = arg->portability,
    .allocator    = &result.allocator,
  }); //clang-format on

  // Create instance.cfg
  //  : with layers
  //  : with extensions
  //  : with debug.cfg as pNext
  result.cfg = cvk_instance_options_create(&result.application, (VkInstanceCreateFlags)arg->portability, result.layers, result.extensions);
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
  instance->allocator.cpu.free(&instance->allocator.cpu, &(cvk_Slice){.len=instance->extensions.len, .itemsize= sizeof(cvk_String), .ptr= instance->extensions.ptr});
  cvk_validation_debug_context_destroy(instance->ct, instance->validation.debug_ct, instance->allocator.gpu);
  instance->cfg = (VkInstanceCreateInfo){ 0 };
  vkDestroyInstance(instance->ct, instance->allocator.gpu);
}  //:: cvk_instance_destroy

