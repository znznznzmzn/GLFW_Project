#pragma once

class GameObjectInstance : public Instance {
private:
	int controller_index = 0;
	GLsizei indices_count = 0;
protected:
	vector<Vertex> vertices;
	vector<uint> indices;

	Material* material;
	UniversalMesh* mesh;

	virtual void AttachInstanceMatrix(uint start_index = 6); // layout(location = 6) in mat4 in_instance_matrix;

	GameObjectInstance() = default;
public:
	GameObjectInstance(GameObject*& targetObject, uint instanceCount = 100)
		: GameObjectInstance(targetObject->GetVertices(), targetObject->GetIndices(), instanceCount) { }
	GameObjectInstance(vector<Vertex>& vertices, vector<uint>& indices, uint instanceCount = 100);
	~GameObjectInstance();

	virtual void Render() override; // ±×¸®±â
	virtual void GUIRender();

	vector<Vertex>& GetVertices() { return vertices; }
	vector<uint>& GetIndices() { return indices; }
	Material*& GetMaterial() { return material; }
	void SetMaterial(string file)   { material->LoadMaterial(file); }
	void SetMaterial(Material* mat) { material->CopyMaterial(mat ); }
};
