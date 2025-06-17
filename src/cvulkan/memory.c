//:__________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0 :
//:__________________________________________________________
#include "./memory.h"
#include <stdlib.h>


cvk_Pure cvk_Slice cvk_allocator_alloc_stdlib (
  cvk_Allocator_CPU* const A,
  cvk_size const           count,
  cvk_size const           itemsize
) {
  cvk_discard(A);
  return (cvk_Slice){ .len = count, .itemsize = itemsize, .ptr = malloc(count * itemsize) };
}


cvk_Pure cvk_Slice cvk_allocator_allocZ_stdlib (
  cvk_Allocator_CPU* const A,
  cvk_size const           count,
  cvk_size const           itemsize
) {
  cvk_discard(A);
  return (cvk_Slice){ .len = count, .itemsize = itemsize, .ptr = calloc(count, itemsize) };
}


void cvk_allocator_free_stdlib (
  cvk_Allocator_CPU* const A,
  cvk_Slice* const         data
) {
  if (data->ptr == NULL) return;
  cvk_discard(A);
  data->itemsize = 0;
  data->len      = 0;
  free(data->ptr);
}


void cvk_allocator_copy_stdlib (
  cvk_Allocator_CPU* const A,
  cvk_Slice const* const   src,
  cvk_Slice* const         trg
) {
  if (src->ptr == NULL) return;
  if (src->len > trg->len) return;
  cvk_discard(A);
  memcpy(trg->ptr, src->ptr, src->len * src->itemsize);
}


cvk_Pure cvk_Slice cvk_allocator_duplicate_stdlib (
  cvk_Allocator_CPU* const A,
  cvk_Slice const* const   data
) {
  if (data->ptr == NULL) return cvk_Slice_empty();
  cvk_discard(A);
  cvk_Slice result = A->allocZ(A, data->len, data->itemsize);
  A->copy(A, data, &result);
  return result;
}


cvk_Pure cvk_Allocator_CPU cvk_allocator_cpu_stdlib () {
  return (cvk_Allocator_CPU){
    .alloc     = cvk_allocator_alloc_stdlib,
    .allocZ    = cvk_allocator_allocZ_stdlib,
    .free      = cvk_allocator_free_stdlib,
    .copy      = cvk_allocator_copy_stdlib,
    .duplicate = cvk_allocator_duplicate_stdlib,
  };
}


cvk_Pure cvk_Allocator cvk_Allocator_defaults () {
  return (cvk_Allocator){
    .gpu = cvk_allocator_gpu_defaults(),
    .cpu = cvk_allocator_cpu_defaults(),
  };
}

