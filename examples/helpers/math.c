//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
//! @fileoverview Math helper code for the examples
//__________________________________________________|
/*
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wimplicit-float-conversion"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
#include <linmath.h>
#pragma GCC diagnostic pop  // -Wdouble-promotion -Wimplicit-float-conversion -Wunsafe-buffer-usage

typedef vec2   Vec2;
typedef vec3   Vec3;
typedef mat4x4 Mat4;
*/

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreserved-macro-identifier"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wpre-c23-compat"
#pragma GCC diagnostic ignored "-Wdocumentation"
#pragma GCC diagnostic ignored "-Wdocumentation-pedantic"
#pragma GCC diagnostic ignored "-Wextra-semi-stmt"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
#define CGLM_FORCE_DEPTH_ZERO_TO_ONE  // Required for vulkan. Not default
// #define CGLM_FORCE_LEFT_HANDED        // Required for vulkan. Not default
#include <cglm.h>
#pragma GCC diagnostic pop  // -Wreserved-macro-identifier -Wfloat-equal -Wpre-c23-compat -Wdocumentation -Wdocumentation-pedantic -Wextra-semi-stmt -Wswitch-default -Wbad-function-cast -Wunsafe-buffer-usage

typedef vec2 Vec2;
typedef vec3 Vec3;
typedef mat4 Mat4;
#define mat4x4_identity(m)                              glm_mat4_identity(m)
#define mat4x4_look_at(m, eye, center, up)              glm_lookat(eye, center, up, m)
#define mat4x4_perspective(m, y_fov, aspect, near, far) glm_perspective(y_fov, aspect, near, far, m)
#define mat4x4_rotate_Z(m, Q, angle)                    glm_rotate_z(Q, angle, m)


typedef enum AxisID {  // clang-format off
  X, Y, Z, W,
  R= X, G= Y, B= Z, A= W,
  U= X, V= Y
} AxisID;  // clang-format on

typedef struct Vertex {
  Vec2 uv;
  Vec3 pos;
  Vec3 color;
} Vertex;

static double const PI  = 3.1415926535897932384626433;
static double const TAU = 2 * PI;

static double deg_to_rad (
  double const deg
) {
  return deg * PI / 180;
}

