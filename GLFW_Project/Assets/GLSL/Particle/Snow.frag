#version 460 core

out vec4 outColor;

uniform sampler2D snowMap;

in vec2 frag_uv;
in vec4 frag_color;
in float frag_alpha;

void main() {
	vec4 result = texture(snowMap, frag_uv);
	result.rgb *= frag_color.rgb * (1 + frag_alpha) * 2.0;
	result.a *= (1.0 - frag_alpha) * 1.5;
	outColor = result;
}