#version 460 core

layout (packed) uniform SkyColor {
	vec3 centerColor;
	float height;
	vec3 apexColor;
	float padding;
};

layout(location = 0) in vec4 frag_pos;
layout(location = 1) in vec3 frag_origin;

out vec4 out_color;

void main() 
{ out_color = vec4(mix(centerColor, apexColor, clamp(frag_origin.y * height, 0.0, 1.0)), 1.0); }