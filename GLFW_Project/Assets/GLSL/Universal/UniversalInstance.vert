#version 460 core

layout (packed) uniform ViewBuffer {
	mat4 view;
	mat4 inverse_view;
};
layout (packed) uniform ProjectionBuffer { mat4 projection; };

// uniform mat4 world; // world는 layout으로 받음

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_tangent;

// Model.vert에서 in_indices, in_weights가 4, 5번을 차지하고 있기에
layout(location = 6) in mat4 in_instance_matrix; // 6번에서 받는다.
// in_instance_matrix는 되도록 6번에서 받기로 (6 ~ 9 [6, 7, 8, 9])

layout(location = 0) out vec4 frag_pos;
layout(location = 1) out vec2 frag_uv;
layout(location = 2) out vec3 frag_normal;
layout(location = 3) out vec3 frag_tangent;
layout(location = 4) out vec3 frag_binormal;
layout(location = 5) out vec3 frag_viewpos;
layout(location = 6) out vec3 frag_worldpos;

void main() {
	frag_pos = in_instance_matrix * vec4(in_pos, 1.0);

	frag_worldpos = frag_pos.xyz;
	frag_viewpos = inverse_view[3].xyz;

	frag_pos = view * frag_pos;
	frag_pos = projection * frag_pos;
	
	frag_uv = in_uv;
	
	frag_normal = mat3(in_instance_matrix) * in_normal;
	frag_tangent = mat3(in_instance_matrix) * in_tangent;
	frag_binormal = cross(frag_normal, frag_tangent);

	gl_Position = frag_pos;
}