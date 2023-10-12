#version 460 core

out vec4 outColor;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

uniform sampler2D alphaMap;

uniform sampler2D secondDiffuseMap;
uniform sampler2D secondSpecularMap;
uniform sampler2D secondNormalMap;

uniform sampler2D thirdDiffuseMap; 
uniform sampler2D thirdSpecularMap; 
uniform sampler2D thirdNormalMap;

uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emissive;

uniform bool hasDiffuse;
uniform bool hasSpecular;
uniform bool hasNormal;
uniform float shininess;

layout (packed) uniform GlobalLightBuffer {
	vec4 light_color;
	vec4 light_direction; 
	vec4 ambient_light;
	vec4 ambient_ceil;
};

layout(location = 0) in vec4 frag_pos;
layout(location = 1) in vec2 frag_uv;
layout(location = 2) in vec3 frag_normal;
layout(location = 3) in vec3 frag_tangent;
layout(location = 4) in vec3 frag_binormal;
layout(location = 5) in vec3 frag_viewpos;
layout(location = 6) in vec3 frag_worldpos;

struct Material {
	vec3 normal;
	vec4 diffuseColor;
	vec4 emissive;
	vec4 specularIntensity;
	float shininess;
	vec3 viewPos;
	vec3 worldPos;
};

 // Mapping
vec4 Mapping_Diffuse(vec2 uv, vec4 alpha) {
	vec4 albedo = { 1, 1, 1, 1 };
	if (hasDiffuse) {
		albedo = texture(diffuseMap, uv);
		albedo = mix(albedo, texture(secondDiffuseMap, uv), alpha.r);
		albedo = mix(albedo, texture(thirdDiffuseMap , uv), alpha.g);
	}
	return albedo;
}
vec4 Mapping_Specular(vec2 uv, vec4 alpha) {
	vec4 intensity = vec4(0, 0, 0, 0);
	if(hasSpecular) {
		intensity = texture(specularMap, uv) * specular;
		intensity = mix(intensity, texture(secondSpecularMap, uv), alpha.r);
		intensity = mix(intensity, texture(thirdSpecularMap , uv), alpha.g);
	}
	return intensity;
}
vec3 Mapping_Normal(vec3 T, vec3 B, vec3 N, vec2 uv, vec4 alpha) {
	T = normalize(T);
	B = normalize(B);
	N = normalize(N);
	if(hasNormal) {
		mat3x3 TBN = mat3x3(T, B, N);

		vec4 normalMapping = texture(normalMap, uv);
		normalMapping = mix(normalMapping, texture(secondNormalMap, uv), alpha.r);
		normalMapping = mix(normalMapping, texture(thirdNormalMap , uv), alpha.g);

		N = normalMapping.rgb * 2.0 - 1.0;
		N = normalize(TBN * N);
	}
	return N;
}

 // Light
vec4 Calc_Ambient(Material material) 
{ return (ambient_light + (material.normal.y * 0.5 + 0.5) * ambient_ceil) * material.diffuseColor; }

vec4 Calc_Emissive(Material material) {
	float emissive_step = 0;
	if (emissive.a > 0) {
		float NdotE = dot(material.normal, normalize(material.viewPos - material.worldPos));
		emissive_step = smoothstep(1.0 - emissive.a, 1.0, 1.0 - clamp(NdotE, 0.0, 1.0));
	}
	return emissive * emissive_step;
}

vec4 Lighting_Directional(Material material) {
	vec3 toLight = -normalize(light_direction.xyz);
	float NDotL = clamp(dot(toLight, material.normal), 0.0, 1.0);
	vec4 finalColor = light_color * NDotL * diffuse;
	if (NDotL > 0) {
		vec3 toEye = normalize(material.viewPos - material.worldPos);
		vec3 halfWay = normalize(toEye + toLight);
		float NDotH = clamp(dot(material.normal, halfWay), 0.0, 1.0);
		finalColor += light_color * pow(NDotH, shininess) * material.specularIntensity;
	}
	return finalColor * material.diffuseColor;
}

void main() {
	vec4 alpha = texture(alphaMap, frag_uv);

	Material material;
	material.diffuseColor = Mapping_Diffuse(frag_uv, alpha);
	material.specularIntensity = Mapping_Specular(frag_uv, alpha);
	material.normal = Mapping_Normal(frag_tangent, frag_binormal, frag_normal, frag_uv, alpha);
	material.viewPos = frag_viewpos;    
	material.worldPos = frag_worldpos;
	
	outColor = Lighting_Directional(material) + Calc_Ambient (material) + Calc_Emissive(material);
}