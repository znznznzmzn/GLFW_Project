#version 460 core

out vec4 outColor;

uniform sampler2D rainMap;

in vec2 frag_uv;
in vec4 frag_color;
in float frag_alpha;

void main() {
	vec4 result = texture(rainMap, frag_uv);
	result.rgb *= frag_color.rgb * (1 + frag_alpha) * 2.0;
	result.a *= frag_alpha;
	if(result.a < 0.05) discard;
	outColor = result;
}