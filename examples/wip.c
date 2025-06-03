//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
// @deps std
#include <stdio.h>
// @deps cdk
#define csys_Implementation
#include <csys.h>
// @deps cvulkan
#define cvk_Implementation
#include <cvulkan.h>

typedef struct Example {
  cvk_Instance instance;
} Example;

static Example example_create () {
  Example result = (Example){
    .instance = cvk_instance_create(&(cvk_instance_create_args){ 0 }),
  };
  return result;
}

static void example_destroy (
  Example* gpu
) {
  cvk_instance_destroy(&gpu->instance);
}

int main () {
  printf("hello cvk_wip_example\n");

  // Initialize
  csys_System sys = csys_init(csys_init_Options_defaults());
  Example     gpu = example_create();
  // Update Loop
  while (!csys_close(&sys)) { csys_update(&sys); }

  example_destroy(&gpu);
  csys_term(&sys);
  return 0;
}

