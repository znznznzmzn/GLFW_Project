#pragma once

class VertexBuffer {
private:
	uint vertex_array_id = -1; // VAO
	uint vertex_buffer_id = -1; // VBO

	uint stride = 0;
public:
	VertexBuffer(void* data, uint count, vector<uint> attributions);
	~VertexBuffer();

	void Set();
	void Update(void* data, uint count);
};