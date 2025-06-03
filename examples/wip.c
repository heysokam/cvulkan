//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
// @deps std
#include <stdio.h>
// @deps cdk
#define csys_Implementation
#include <csys.h>

int main () {
  printf("hello cvk_wip_example\n");

  // Initialize
  csys_System sys = csys_init(csys_init_Options_defaults());
  // Update Loop
  while (!csys_close(&sys)) {
    csys_update(&sys);
  }

  csys_term(&sys);
  return 0;
}

