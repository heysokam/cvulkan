#version 450

layout(location = 0) in  vec2 in_Position;
layout(location = 1) in  vec3 in_Color;
layout(location = 0) out vec3 out_Color;

void main () {
  out_Color   = in_Color;
  gl_Position = vec4(in_Position, 0.0, 1.0);
}

