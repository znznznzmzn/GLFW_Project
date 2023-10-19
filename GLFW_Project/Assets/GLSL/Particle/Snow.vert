#version 460 core

layout (packed) uniform SnowBuffer {
	vec3 velocity;
	float drawDistance;
	
	vec4 particleColor;
	
	vec3 origin;
	float time;
	
	vec3 size;
	float turbulence;
};

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_size;
layout(location = 2) in vec2 in_random;

out VS_OUT {
	vec2 gs_size;
	float gs_distance;
	vec4 gs_color;
	vec3 gs_velocity;
} vs_out;

void main() {
	vec3 displace = velocity * time;

	vec3 position = in_pos;
	position.y = origin.y + size.y - mod((in_pos.y - displace.y), size.y);
	
	position.x += cos(time - in_random.x) * turbulence;
	position.z += cos(time - in_random.y) * turbulence;
	
	position = origin + mod((size + mod((position + displace), size)), size) - (size * 0.5);
	
	vs_out.gs_distance = drawDistance;
	vs_out.gs_velocity = velocity;	
	vs_out.gs_color = particleColor;
	
	vs_out.gs_size = in_size;
	
	gl_Position = vec4(position, 1.0);
}