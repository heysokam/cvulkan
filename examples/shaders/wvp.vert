#version 450

layout(binding = 0) uniform WVP {
  mat4 world;
  mat4 view;
  mat4 projection;
} in_WVP;

layout(location = 0) in  vec2 in_Position;
layout(location = 1) in  vec3 in_Color;
layout(location = 0) out vec3 out_Color;

void main () {
  out_Color   = in_Color;
  gl_Position = in_WVP.projection * in_WVP.view * in_WVP.world * vec4(in_Position, 0.0, 1.0);
}

