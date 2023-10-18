#version 460 core

const int MAX_BONES = 256;  

layout (packed) uniform ViewBuffer {
	mat4 view;
	mat4 inverse_view;
};
layout (packed) uniform ProjectionBuffer { mat4 projection; };

uniform mat4 world;
uniform mat4 bones[MAX_BONES];

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec4 in_indices;
layout(location = 5) in vec4 in_weights;

layout(location = 0) out vec4 frag_pos;
layout(location = 1) out vec2 frag_uv;
layout(location = 2) out vec3 frag_normal;
layout(location = 3) out vec3 frag_tangent;
layout(location = 4) out vec3 frag_binormal;
layout(location = 5) out vec3 frag_viewpos;
layout(location = 6) out vec3 frag_worldpos;

mat4 BoneWorld(vec4 indices, vec4 weights) {
	mat4 transform = mat4(0);
	transform += bones[uint(indices.x)] * weights.x;
	transform += bones[uint(indices.y)] * weights.y;
	transform += bones[uint(indices.z)] * weights.z;
	transform += bones[uint(indices.w)] * weights.w;
    return world * transform;
}

void main() {
    mat4 transform = BoneWorld(in_indices, in_weights);

	frag_pos = transform * vec4(in_pos, 1.0);
	frag_worldpos = frag_pos.xyz;
	frag_viewpos = inverse_view[3].xyz;
	frag_pos = view * frag_pos;
	frag_pos = projection * frag_pos;
	frag_uv = in_uv;
	frag_normal  = mat3(transform) * in_normal;
	frag_tangent = mat3(transform) * in_tangent;
	frag_binormal = cross(frag_normal, frag_tangent);

	gl_Position = frag_pos;
}