//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
//! @fileoverview
//!  Most minimal cvulkan example possible.
//!  Creates+destroys an instance with validation layers & debug messenger.
//__________________________________________________________________________|
#define cvk_Implementation
#include <cvulkan.h>

int main () {
  cvk_Instance instance = (cvk_Instance){ 0 };  // Zero Initialize

  //________________________________________________
  // Validate that nothing is initialized before running
  cvk_assert(instance.ct == NULL, "The instance context should be null (not initialized yet).");
  cvk_assert(instance.validation.debug_active == cvk_false, "Debug Messenger should be inactive (not initialized yet).");
  cvk_assert(instance.validation.layers_active == cvk_false, "Validation layers should be inactive (not initialized yet).");
  cvk_assert(instance.allocator.cpu.free == NULL, "CPU Allocator should not contain valid functions (not initialized yet).");

  //________________________________________________
  // Create the instance with all options at their defaults
  // @note Validation Layers and Debug Messenger will be active
  instance = cvk_instance_create(&(cvk_instance_create_args){ 0 });

  //________________________________________________
  // Check that everything was initialized correctly
  cvk_assert(instance.ct != NULL, "The instance context should be not be null.");
  cvk_assert(instance.validation.debug_active == cvk_true, "Debug Messenger should be active (default).");
  cvk_assert(instance.validation.layers_active == cvk_true, "Validation layers should be active (default).");
  cvk_assert(instance.allocator.cpu.free != NULL, "CPU Allocator should contain valid functions (default: stdlib).");

  //________________________________________________
  // Destroy the instance and all its contents
  cvk_instance_destroy(&instance);
}

