//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wundef"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include <tinyobj_loader_c.h>
#pragma GCC diagnostic pop  // -Wmissing-prototypes -Wundef -Wunsafe-buffer-usage


static void tinyobj_cb_reader (
  void*       ctx,
  char const* filename,
  int const   is_mtl,
  char const* obj_filename,
  char**      result_ptr,
  size_t*     result_len
) {
  /* discard */ (void)ctx;
  /* discard */ (void)is_mtl;
  /* discard */ (void)obj_filename;

  FILE* file = fopen(filename, "rb");
  if (!file) {
    result_ptr = NULL;
    return;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  if (file_size < 0) {
    fclose(file);
    result_ptr = NULL;
    return;
  }

  // Allocate buffer
  char* buffer = (char*)malloc((size_t)file_size + 1);
  if (!buffer) {
    fclose(file);
    result_ptr = NULL;
    return;
  }

  // Read file
  size_t bytes_read = fread(buffer, 1, (size_t)file_size, file);
  fclose(file);
  if (bytes_read != (size_t)file_size) {
    free(buffer);
    result_ptr = NULL;
    return;
  }
  buffer[file_size] = '\0';  // Null terminate for safety

  // Assign to the result
  *result_len = (size_t)file_size;
  *result_ptr = buffer;
}

