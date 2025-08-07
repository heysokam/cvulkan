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
// Generic boilerplate shared by all Examples
#include "./helpers/bootstrap.c"
//______________________________________

typedef struct Example {
  example_Bootstrap gpu;
  ctime_Time        time;
} Example;


static Example example_create (
  csys_System const* const system
) {
  Example result = (Example){ 0 };
  result.time    = ctime_start();
  result.gpu     = example_bootstrap_create(system->window.ct, (cvk_Size2D){ .width = system->window.width, .height = system->window.height });
  return result;
}


static void example_update (
  Example* const           example,
  csys_System const* const system
) {
  cvk_discard(example);  // FIX: Remove
  cvk_discard(system);   // FIX: Remove
}


static void example_destroy (
  Example* const example
) {
  cvk_device_logical_wait(&example->gpu.device_logical);
  example_bootstrap_destroy(&example->gpu);
}


int main () {
  // Initialize: Window/Input
  csys_System system = csys_init(csys_init_Options_defaults());

  // Initialize: Example Data
  Example example = example_create(
    /* system */ &system  // clang-format off
  );

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

