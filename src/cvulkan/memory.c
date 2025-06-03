//:____________________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  LGPLv3 or higher  :
//:____________________________________________________________________
#include "./memory.h"
// TODO:
cvk_Pure cvk_Allocator cvk_Allocator_defaults () { return (cvk_Allocator){ .gpu = NULL }; }
