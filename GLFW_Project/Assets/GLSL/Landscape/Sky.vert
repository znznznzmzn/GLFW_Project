#version 460 core

layout (packed) uniform ViewBuffer {
	mat4 view;
	mat4 inverse_view;
};
layout (packed) uniform ProjectionBuffer { mat4 projection; };

layout(location = 0) in vec3 in_pos;

layout(location = 0) out vec4 frag_pos;
layout(location = 1) out vec3 frag_origin;

void main() {
	frag_pos = vec4(mat3(view) * in_pos, 1.0);
	frag_pos = projection * frag_pos;
	
	frag_origin = in_pos;

	gl_Position = frag_pos;
}
