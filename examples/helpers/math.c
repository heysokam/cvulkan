//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  CC0-1.0  :
//:___________________________________________________________
//! @fileoverview Math helper code for the examples
//__________________________________________________|
typedef float  f32;
typedef double f64;

// clang-format off
typedef struct Vec2 { union {
  struct { f32 x,y; };
  f32 v[2];
};} Vec2;
// clang-format on

// clang-format off
typedef struct Vec3 { union {
  struct { f32 x,y,z; };
  struct { f32 r,g,b; };
  f32 v[3];
};} Vec3;
// clang-format on

// clang-format off
typedef struct Mat4 { union {
  struct { f32
    m00,m01,m02,m03,
    m10,m11,m12,m13,
    m20,m21,m22,m23,
    m30,m31,m32,m33; };
  f32 m[16];
};} Mat4;
// clang-format on

typedef struct Vertex {
  Vec2 pos;
  Vec3 color;
} Vertex;

