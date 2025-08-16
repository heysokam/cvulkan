//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "./allocator.h"
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


void cvk_allocator_resize_stdlib (
  cvk_Allocator_CPU* const A,
  cvk_Slice* const         trg,
  cvk_size const           len
) {
  cvk_discard(A);
  void* tmp = realloc(trg->ptr, len * trg->itemsize);
  if (!tmp) return;
  trg->len = len;
  trg->ptr = tmp;
}


void cvk_allocator_string_copy_stdlib (
  cvk_Allocator_CPU* const A,
  cvk_String const         src,
  cvk_String               trg
) {
  cvk_discard(A);
  cvk_size const len = strlen(src) + 1;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
  trg = (cvk_String)memcpy((void*)trg, src, len);
#pragma GCC diagnostic pop  // -Wcast-qual
}


cvk_Pure cvk_String cvk_allocator_string_duplicate_stdlib (
  cvk_Allocator_CPU* const A,
  cvk_String const         src
) {
  cvk_size const len    = strlen(src) + 1;
  cvk_String     result = (cvk_String)malloc(len * sizeof(char));
  if (!result) return NULL;
  A->string_copy(A, src, result);
  return result;
}


cvk_Pure cvk_Allocator_CPU cvk_allocator_cpu_stdlib () {
  return (cvk_Allocator_CPU){
    // Allocation
    .alloc     = cvk_allocator_alloc_stdlib,
    .allocZ    = cvk_allocator_allocZ_stdlib,
    .free      = cvk_allocator_free_stdlib,
    .copy      = cvk_allocator_copy_stdlib,
    .duplicate = cvk_allocator_duplicate_stdlib,
    .resize    = cvk_allocator_resize_stdlib,
    // String Allocation
    .string_copy      = cvk_allocator_string_copy_stdlib,
    .string_duplicate = cvk_allocator_string_duplicate_stdlib,
  };
}


cvk_Pure cvk_Allocator cvk_allocator_defaults () {
  return (cvk_Allocator){
    .gpu = cvk_allocator_gpu_defaults(),
    .cpu = cvk_allocator_cpu_defaults(),
  };
}

