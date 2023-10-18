#version 460 core

layout (location = 0) in vec4 frag_pos;
layout (location = 1) in vec3 frag_origin;

out vec4 out_color;

uniform samplerCube cubeMap;

void main() { out_color = vec4(texture(cubeMap, frag_origin).rgb, 1.0); }
