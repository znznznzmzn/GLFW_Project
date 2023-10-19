#version 460 core

layout (packed) uniform RainBuffer {
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

out VS_OUT {
	vec2 gs_size;
	vec2 gs_distance;
	vec4 gs_color;
	vec3 gs_velocity;
} vs_out;

void main() {
	vec3 v = velocity;
	v.xz /= in_size.y * 0.1;
	vs_out.gs_velocity = v;
	
	vec3 displace = v * time;
	vec3 position = in_pos;

	position = origin + mod((size + mod((position + displace), size)), size) - (size * 0.5);
	
	float alpha = clamp(1.5 + cos(time + (position.x + position.z)) / drawDistance * 2.0, 0.0, 1.0);
	
	vs_out.gs_distance.x = drawDistance;
	vs_out.gs_distance.y = alpha;
	
    vs_out.gs_color = particleColor;
	
	vs_out.gs_size = in_size;
	
	gl_Position = vec4(position, 1.0);
}