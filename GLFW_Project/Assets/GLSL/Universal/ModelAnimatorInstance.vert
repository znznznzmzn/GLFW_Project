#version 460 core

const int MAX_FRAME_INSTANCE = 256; 

layout (packed) uniform ViewBuffer {
	mat4 view;
	mat4 inverse_view;
};
layout (packed) uniform ProjectionBuffer { mat4 projection; };

struct Frame {
	int clip;
	uint curFrame;
	float time;
	float speed;
};
struct Motion {
	float takeTime;
	float tweenTime;
	float runningTime;
	float padding;
	
	Frame cur;
	Frame next;
};

layout (packed) uniform FrameInstance { Motion motions[MAX_FRAME_INSTANCE]; };

uniform sampler2DArray transformMap;

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec4 in_indices;
layout(location = 5) in vec4 in_weights;
layout(location = 6) in mat4 in_instance_matrix; // (6 ~ 9 [6, 7, 8, 9])
layout(location = 10) in uint in_instance_index; 

layout(location = 0) out vec4 frag_pos;
layout(location = 1) out vec2 frag_uv;
layout(location = 2) out vec3 frag_normal;
layout(location = 3) out vec3 frag_tangent;
layout(location = 4) out vec3 frag_binormal;
layout(location = 5) out vec3 frag_viewpos;
layout(location = 6) out vec3 frag_worldpos;

mat4 Lerp(mat4 s, mat4 e, float t) { return (s * (1.0 - t)) + (e * t); }
mat4 SkinWorld(uint instanceID, vec4 indices, vec4 weights) {
	mat4 transform = mat4(0);
	mat4 cur, next;
	mat4 curAnim, nextAnim;

	for (int i = 0; i < 4; i += 1) {
		uint curFrame = motions[instanceID].cur.curFrame;
		int clip = motions[instanceID].cur.clip;
		cur = mat4(
			texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 0, int(curFrame), clip), 0), 
			texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 1, int(curFrame), clip), 0), 
			texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 2, int(curFrame), clip), 0), 
			texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 3, int(curFrame), clip), 0));
		next = mat4(
			texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 0, int(curFrame + 1), clip), 0),
			texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 1, int(curFrame + 1), clip), 0),
			texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 2, int(curFrame + 1), clip), 0),
			texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 3, int(curFrame + 1), clip), 0));
		curAnim = Lerp(cur, next, motions[instanceID].cur.time);
		
		curFrame = motions[instanceID].next.curFrame;
		clip = motions[instanceID].next.clip;
		if (clip > -1) {
			cur = mat4(
				texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 0, int(curFrame), clip), 0), 
				texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 1, int(curFrame), clip), 0), 
				texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 2, int(curFrame), clip), 0), 
				texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 3, int(curFrame), clip), 0));
			next = mat4(
				texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 0, int(curFrame + 1), clip), 0),
				texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 1, int(curFrame + 1), clip), 0),
				texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 2, int(curFrame + 1), clip), 0),
				texelFetch(transformMap, ivec3(int(indices[i]) * 4 + 3, int(curFrame + 1), clip), 0));
			nextAnim = Lerp(cur, next, motions[instanceID].next.time);
			curAnim = Lerp(curAnim, nextAnim, motions[instanceID].tweenTime);
		}
		transform += curAnim * weights[i];
	}
	
	return transform;
}

void main() {
	mat4 transform = in_instance_matrix * SkinWorld(in_instance_index, in_indices, in_weights);

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