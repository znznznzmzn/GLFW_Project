#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (packed) uniform ViewBuffer {
	mat4 view;
	mat4 inverse_view;
};
layout (packed) uniform ProjectionBuffer { mat4 projection; };

const vec2 TEXCOORD[4] = {
	vec2(0.0, 1.0),
	vec2(0.0, 0.0),
	vec2(1.0, 1.0),
	vec2(1.0, 0.0)
};

in VS_OUT {
	vec2 gs_size;
	float gs_distance;
	vec4 gs_color;
	vec3 gs_velocity;
} gs_in[];

out vec2 frag_uv;
out vec4 frag_color;
out float frag_alpha;

void main() {
	vec4 position = gl_in[0].gl_Position;

	vec3 up = normalize(-gs_in[0].gs_velocity);
    vec3 forward = position.xyz - inverse_view[3].xyz;
    forward = normalize(forward);
    vec3 right = normalize(cross(up, forward));
	
    vec2 halfSize = gs_in[0].gs_size * 0.5;
	
	vec4 vertices[4] = {
		vec4(position.xyz - halfSize.x * right - halfSize.y * up, 1.0),
		vec4(position.xyz - halfSize.x * right + halfSize.y * up, 1.0),
		vec4(position.xyz + halfSize.x * right - halfSize.y * up, 1.0),
		vec4(position.xyz + halfSize.x * right + halfSize.y * up, 1.0)
	};
	
    frag_color = gs_in[0].gs_color;
	
	for (int i = 0; i < 4; i++) {
        vec4 vertex = projection * view * vertices[i];
        frag_uv = TEXCOORD[i];
        frag_alpha = clamp(1 - vertex.z / gs_in[0].gs_distance, 0.0, 1.0) * 0.5;
		gl_Position = vertex;
		EmitVertex();
    }
    EndPrimitive();
}