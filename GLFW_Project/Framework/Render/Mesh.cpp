#include "../Framework.h"

// PositionMesh
PositionMesh::PositionMesh(const vector<Vector3>& vertices, const vector<uint>& indices) : Mesh<Vector3>() {
	glBindVertexArray(vertex_array_id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// UVMesh
UVMesh::UVMesh(
	const vector<VertexUV>& vertices,
	const vector<uint>& indices) :
	Mesh<VertexUV>() {
	glBindVertexArray(vertex_array_id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexUV), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

	uint stride = sizeof(VertexUV);
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	// uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// ColorMesh
ColorMesh::ColorMesh(
	const vector<VertexColor>& vertices, 
	const vector<uint>& indices) : 
	Mesh<VertexColor>() {
	glBindVertexArray(vertex_array_id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexColor), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

	uint stride = sizeof(VertexColor);
	// pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


// UniversalMesh
UniversalMesh::UniversalMesh(
	const vector<Vertex>& vertices, 
	const vector<uint>& indices) : 
	Mesh<Vertex>() {
	glBindVertexArray(vertex_array_id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

	uint stride = sizeof(Vertex);
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride // 0
		, (void*)0);
	glEnableVertexAttribArray(0); 
	// uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride // pos(Vector3)
		, (void*)(sizeof(Vector3)));
	glEnableVertexAttribArray(1); 
	// normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride // pos(Vector3) + uv(Vector2)
		, (void*)(sizeof(Vector3) + sizeof(Vector2)));
	glEnableVertexAttribArray(2);
	// tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride // pos(Vector3) + uv(Vector2) + normal(Vector3)
		, (void*)(sizeof(Vector3) + sizeof(Vector2) + sizeof(Vector3))); 
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ModelMesh::ModelMesh(
	const vector<ModelVertex>& vertices, 
	const vector<uint>& indices) :
	Mesh<ModelVertex>() {
	index_count = indices.size();

	glBindVertexArray(vertex_array_id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ModelVertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

	uint stride = sizeof(ModelVertex);
	// pos
	uint pointer = 0; // 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)pointer);
	glEnableVertexAttribArray(0);
	// uv
	pointer += sizeof(Vector3); // pos(Vector3)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)pointer);
	glEnableVertexAttribArray(1);
	// normal
	pointer += sizeof(Vector2); // pos(Vector3) + uv(Vector2)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)pointer);
	glEnableVertexAttribArray(2);
	// tangent
	pointer += sizeof(Vector3); // pos(Vector3) + uv(Vector2) + normal(Vector3)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)pointer);
	glEnableVertexAttribArray(3);
	// indices
	pointer += sizeof(Vector3); // pos(Vector3) + uv(Vector2) + normal(Vector3) + tangent(Vector3)
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)pointer);
	glEnableVertexAttribArray(4);
	// weights
	pointer += sizeof(Vector4); // pos(Vector3) + uv(Vector2) + normal(Vector3) + tangent(Vector3) + indices(Vector4)
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (void*)pointer);
	glEnableVertexAttribArray(5);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
