#pragma once

template <typename T>
class Mesh {
protected:
	uint VertexArrayBuffer = -1; // VAO
	uint VertexBuffer = -1; // VBO
	uint IndexBuffer = -1; //EBO
	Mesh() {
		glGenVertexArrays(1, &VertexArrayBuffer);
		glGenBuffers(1, &VertexBuffer);
		glGenBuffers(1, &IndexBuffer);
	}
	~Mesh() {
		glDeleteVertexArrays(1, &VertexArrayBuffer);
		glDeleteBuffers(1, &VertexBuffer);
		glDeleteBuffers(1, &IndexBuffer); 
	}
public:
	void Set() { glBindVertexArray(VertexArrayBuffer); }
	void UpdateMesh(const vector<T>& vertices, const vector<uint>& indices) {
		glBindVertexArray(VertexArrayBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
	void UpdateVertex(const vector<T>& vertices) {
		glBindVertexArray(VertexArrayBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
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