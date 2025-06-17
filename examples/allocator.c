//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
//! @fileoverview
//! Example of how to use the cvulkan Allocator API
//__________________________________________________|
#include <assert.h>
#include <cvulkan/memory.h>

int main () {
  cvk_Allocator std = cvk_Allocator_defaults();

  cvk_Slice result = std.cpu.allocZ(&std.cpu, 42, sizeof(char));
  for (cvk_size id = 0; id < result.len; ++id) {
    char* list = (char*)result.ptr;  // Slices are unaware of their type
    assert(list[id] == 0);
  }

  size_t len = result.len;
  std.cpu.free(&std.cpu, &result);

  return len;
}

