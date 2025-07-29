#version 450
layout(binding = 1) uniform sampler2D in_Texture;

layout(location = 0) in  vec3 in_Color;
layout(location = 1) in  vec2 in_UV;
layout(location = 0) out vec4 out_Color;

void main () { out_Color = texture(in_Texture, in_UV); }
