#pragma once

template <typename T>
class Mesh {
protected:
	uint vertex_array_id = -1; // VAO
	uint vertex_buffer_id = -1; // VBO
	uint index_buffer_id = -1; //EBO
	Mesh() {
		glGenVertexArrays(1, &vertex_array_id);
		glGenBuffers(1, &vertex_buffer_id);
		glGenBuffers(1, &index_buffer_id);
	}
	~Mesh() {
		glDeleteVertexArrays(1, &vertex_array_id);
		glDeleteBuffers(1, &vertex_buffer_id);
		glDeleteBuffers(1, &index_buffer_id); 
	}
public:
	void Set() { glBindVertexArray(vertex_array_id); }
	void UpdateMesh(const vector<T>& vertices, const vector<uint>& indices) {
		glBindVertexArray(vertex_array_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
	void UpdateVertex(const vector<T>& vertices) {
		glBindVertexArray(vertex_array_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
};

class PositionMesh : public Mesh<Vector3> {
public:
	PositionMesh(const vector<Vector3>& vertices, const vector<uint>& indices);
	~PositionMesh() = default;
};

class UVMesh : public Mesh<VertexUV> {
public:
	UVMesh(const vector<VertexUV>& vertices, const vector<uint>& indices);
	~UVMesh() = default;
};

class ColorMesh : public Mesh<VertexColor> {
public:
	ColorMesh(const vector<VertexColor>& vertices, const vector<uint>& indices);
	~ColorMesh() = default;
};

class UniversalMesh : public Mesh<Vertex> {
public: 
	UniversalMesh(const vector<Vertex>& vertices, const vector<uint>& indices);
	~UniversalMesh() = default;
};

class ModelMesh : public Mesh<ModelVertex> {
	uint index_count = 0;
public:
	ModelMesh(const vector<ModelVertex>& vertices, const vector<uint>& indices);
	~ModelMesh() = default;
	const uint& GetIndexCount() { return index_count; }
};