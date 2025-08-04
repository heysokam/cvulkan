#version 450

layout(binding = 0) uniform WVP {
  mat4 world;
  mat4 view;
  mat4 projection;
} in_WVP;

layout(location = 0) in  vec3 in_Position;
layout(location = 1) in  vec3 in_Color;
layout(location = 2) in  vec2 in_UV;

layout(location = 0) out vec3 out_Color;
layout(location = 1) out vec2 out_UV;

void main () {
  out_Color   = in_Color;
  out_UV      = in_UV;
  gl_Position = in_WVP.projection * in_WVP.view * in_WVP.world * vec4(in_Position, 1.0);
}

