#include "../Framework.h"

VertexBuffer::VertexBuffer(void* data, uint count, vector<uint> attributions) {
	stride = 0;

	for (uint attrib : attributions) stride += attrib;

	glGenBuffers(1, &vertex_buffer_id);
	glGenVertexArrays(1, &vertex_array_id);

	glBindVertexArray(vertex_array_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, stride * count, data, GL_STATIC_DRAW);

	uint offset = 0;
	for (uint i = 0; i < attributions.size(); i++) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attributions[i] / sizeof(float), GL_FLOAT, GL_FALSE, stride, (void*)offset);
		offset += attributions[i];
	}
}

VertexBuffer::~VertexBuffer() {
	glDeleteVertexArrays(1, &vertex_array_id);
	glDeleteBuffers(1, &vertex_buffer_id);
}

void VertexBuffer::Set() {
	glBindVertexArray(vertex_array_id);
}

void VertexBuffer::Update(void* data, uint count) {
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, stride * count, data);
}
