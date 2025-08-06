//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
#include "./cmath.h"
#include <cvulkan.h>


//__________________________________________________________________________________________________
// @section cgltf
//____________________________
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wextra-semi-stmt"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#pragma GCC diagnostic pop  // -Wextra-semi-stmt -Wsign-conversion -Wbad-function-cast -Wswitch-default -Wswitch-enum -Wcast-align -Wcast-qual -Wpadded -Wunsafe-buffer-usage


char const* cgltf_result_toString (cgltf_result const status);

char const* cgltf_result_toString (
  cgltf_result const status
) {
  switch (status) {
    case cgltf_result_success         : return "[cgltf.Ok]";
    case cgltf_result_data_too_short  : return "[cgltf.DataTooShort]";
    case cgltf_result_unknown_format  : return "[cgltf.UnknownFormat]";
    case cgltf_result_invalid_json    : return "[cgltf.InvalidJSON]";
    case cgltf_result_invalid_gltf    : return "[cgltf.InvalidGLTF]";
    case cgltf_result_invalid_options : return "[cgltf.InvalidOptions]";
    case cgltf_result_file_not_found  : return "[cgltf.FileNotFound]";
    case cgltf_result_io_error        : return "[cgltf.IOError]";
    case cgltf_result_out_of_memory   : return "[cgltf.OutOfMemory]";
    case cgltf_result_legacy_gltf     : return "[cgltf.LegacyGLTF]";
    case cgltf_result_max_enum        : /* fall-through */
    default                           : return "[cgltf.UnknownStatus]";
  }
}

#ifndef cgltf_result_check
#include <stdio.h>  // clang-format off
#define cgltf_result_check(status, message)                                         \
  do {                                                                              \
    cgltf_result const code = status;                                               \
    if (code) {                                                                     \
      printf("%s (%s:%d) %s: " message "\n",                                        \
        cgltf_result_toString((cgltf_result)status), __FILE__, __LINE__, __func__); \
      exit((int)code);                                                              \
    }                                                                               \
  } while (0)
#endif  // cgltf_result_check
// clang-format on


static void gltf_get_attribute (
  cgltf_data const* const gltf,
  char const* const       name,
  cvk_Slice* const        result /* ptr Vertex[] */
) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"

  // Debug the loaded data
  // printf("[gltf].Type:   %u\n", gltf->file_type);
  // printf("[gltf].Meshes: %u\n", (unsigned)gltf->meshes_count);
  // printf("[gltf].Position : %d:%s : accessor:%d\n", 0, gltf->meshes[0].primitives[0].attributes[0].name, pos_accessor_id);
  // printf("[gltf].UV       : %d:%s : accessor:%d\n", 1, gltf->meshes[0].primitives[0].attributes[1].name, uv_accessor_id);
  // for (cgltf_size id_attribute = 0; id_attribute < gltf->meshes[0].primitives[0].attributes_count; ++id_attribute) {
  //   char const* const          name_current = gltf->meshes[0].primitives[0].attributes[id_attribute].name;
  //   cgltf_attribute_type const type_attr    = gltf->meshes[0].primitives[0].attributes[id_attribute].type;
  //   cgltf_component_type const type_comp    = gltf->meshes[0].primitives[0].attributes[id_attribute].data->component_type;
  //   printf("[gltf].Inspect: %zu:%s : T(%u,%u)\n", id_attribute, name_current, type_attr, type_comp);
  // }

  // Find the attribute
  cgltf_int attribute_id = INT_MAX;
  for (cgltf_size id = 0; id < gltf->meshes[0].primitives[0].attributes_count; ++id) {
    char const* const name_current = gltf->meshes[0].primitives[0].attributes[id].name;
    if (strcmp(name_current, name) != 0) continue;
    attribute_id = (cgltf_int)id;
    break;
  }
  assert(attribute_id != INT_MAX);

  // Find the accessor
  cgltf_accessor const* const accessor = gltf->meshes[0].primitives[0].attributes[attribute_id].data;

  // Number of items that we will find
  if (result->len == 0) result->len = accessor->count;
  // Allocate the necessary memory
  if (result->ptr == NULL) result->ptr = /* Vertex* */ calloc(result->len, result->itemsize);

  // Find a pointer to the raw data
  char const* const buffer_bytes_ptr = accessor->buffer_view->buffer->data;
  // cgltf_size const  buffer_bytes_len = gltf->meshes[0].primitives[0].attributes[0].data->buffer_view->buffer->size;
  // for (cgltf_size id = 0; id < buffer_bytes_len; ++id) { printf("[gltf].bytes.ptr: %zu : %d\n", id, buffer_bytes_ptr[id]); }

  // How to interpret the raw data
  cgltf_size const offset_accessor = accessor->offset;
  cgltf_size const offset_view     = accessor->buffer_view->offset;
  cgltf_size const component_count = cgltf_num_components(accessor->type);
  cgltf_size const component_size  = cgltf_component_size(accessor->component_type);
  cgltf_size const itemsize        = component_size * component_count;
  cgltf_size const offset          = offset_view + offset_accessor;

  // Copy the real data to the result
  for (cgltf_size id = 0; id < accessor->count; ++id) {
    // Find the target location/pointer that will store the data inside the result
    void* target = NULL;  // clang-format off
    if      (strcmp(name, "POSITION"  ) == 0) target = &(((Vertex*)result->ptr)[id].pos);
    else if (strcmp(name, "TEXCOORD_0") == 0) target = &(((Vertex*)result->ptr)[id].uv);
    else assert("Attribute Not Implemented" == 0);  // clang-format on

    // Copy the buffer memory into the result
    cgltf_size const start = offset + itemsize * id;
    memcpy(target, &buffer_bytes_ptr[start], itemsize);
  }
#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
}

static void gltf_get_indices (
  cgltf_data const* const gltf,
  cvk_Slice* const        result /* ptr uint32_t[] */
) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
  // Find the accessor
  cgltf_accessor const* const accessor = gltf->meshes[0].primitives[0].indices;

  // Number of items that we will find
  if (result->len == 0) result->len = accessor->count;
  // Allocate the necessary memory
  if (result->ptr == NULL) result->ptr = /* uint32_t* */ calloc(result->len, result->itemsize);

  // Find a pointer to the raw data
  char const* const buffer_bytes_ptr = accessor->buffer_view->buffer->data;

  // How to interpret the raw data
  cgltf_size const offset_accessor = accessor->offset;
  cgltf_size const offset_view     = accessor->buffer_view->offset;
  cgltf_size const component_count = cgltf_num_components(accessor->type);
  cgltf_size const component_size  = cgltf_component_size(accessor->component_type);
  cgltf_size const itemsize        = component_size * component_count;
  cgltf_size const offset          = offset_view + offset_accessor;
  // Assert that size is what we expect (uint32 vs uint16)
  assert(result->itemsize == itemsize);

  // Copy the buffer memory into the result
  cgltf_size const start = offset;
  memcpy(result->ptr, &buffer_bytes_ptr[start], itemsize * result->len);

#pragma GCC diagnostic pop  // -Wunsafe-buffer-usage
}

