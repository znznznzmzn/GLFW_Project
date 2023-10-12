#version 460 core

uniform mat4 world;
layout (packed) uniform ViewBuffer {
    mat4 view;
    mat4 inverse_view;
};
layout (packed) uniform ProjectionBuffer {  mat4 projection; };

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 GridColor;

void main() {
	gl_Position = projection * view * world * vec4(aPos, 1.0);
    GridColor = aColor;
}
