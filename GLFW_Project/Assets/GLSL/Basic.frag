#version 460 core

layout(location = 0) in vec4 frag_pos;
layout(location = 1) in vec4 frag_color;

out vec4 FragColor;

void main() { FragColor = frag_color; }