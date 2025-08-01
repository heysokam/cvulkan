//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
//! @fileoverview Math helper code for the examples
//__________________________________________________|
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wimplicit-float-conversion"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
#include <linmath.h>
#pragma GCC diagnostic pop  // -Wdouble-promotion -Wimplicit-float-conversion -Wunsafe-buffer-usage

typedef vec2   Vec2;
typedef vec3   Vec3;
typedef mat4x4 Mat4;

typedef enum AxisID { // clang-format off
  X, Y, Z, W,
  R= X, G= Y, B= Z, A= W,
  U= X, V= Y
} AxisID; // clang-format on

typedef struct Vertex {
  Vec2 pos;
  Vec2 uv;
  Vec3 color;
} Vertex;

static double const PI  = 3.1415926535897932384626433;
static double const TAU = 2 * PI;

static double deg_to_rad (
  double const deg
) {
  return deg * PI / 180;
}

