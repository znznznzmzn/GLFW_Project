#version 460 core

layout (packed) uniform ViewBuffer {
	mat4 view;
	mat4 inverse_view;
};
layout (packed) uniform ProjectionBuffer { mat4 projection; };

uniform mat4 world;

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec4 in_color;

layout(location = 0) out vec4 frag_color;

void main() {
	gl_Position = projection * view * world * vec4(in_pos, 1.0f);
	frag_color = in_color;
}

