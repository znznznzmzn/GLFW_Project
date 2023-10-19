#pragma once

class GameObject : public Transform {
private:
	GLsizei index_count = 0;
protected:
	vector<Vertex> vertices;
	vector<uint> indices;

	Material* material;
	UniversalMesh* mesh;
	MatrixBuffer* wBuffer;
	virtual void init(
		string vshader_path = "Assets/GLSL/Universal/Universal.vert", 
		string fshader_path = "Assets/GLSL/Universal/Universal.frag");
public:
	~GameObject();
	virtual void Render();
	virtual void GUIRender();

	vector<Vertex>&	GetVertices() { return vertices; }
	vector<uint>& GetIndices() { return indices; }
	Material*& GetMaterial() { return material; }
	void SetMaterial(string file) { material->LoadMaterial(file); };
};
