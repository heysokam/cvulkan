//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
#include "../device.h"

cvk_Pure cvk_device_Extensions cvk_device_Extensions_default(
  cvk_Allocator*const allocator
) {
  cvk_Slice result = cvk_Slice_empty();
  cvk_discard(allocator);  // FIX: Allocate default extensions used by cvulkan
  return result;
}

