#pragma once

class ModelNode : public Transform { // »À´ëx ³ëµå¸¸ ÀÖ´Â ¸ðµ¨
protected:
	class ModelParts : public Transform {
		ModelMesh* mesh = nullptr;
		MatrixBuffer* wBuffer = nullptr;
	public:
		// string parts_name; >> Transform->tag

		ModelParts(MeshData*& mesh_data, const uint& ProgramID);
		~ModelParts();

		void Render();
		void UpdateProgram(const uint& ProgramID);
	};

	int commonProgramID = 0;
	ModelData* data = nullptr;
	map<Material*, vector<ModelParts*>> render_map;
	MatrixBuffer* wBuffer = nullptr;

	void ClearModelMap();
	void LoadModel(string model_name);
public:
	ModelNode(string model_name);
	~ModelNode();

	virtual void UpdateWorld() override;

	virtual void Render();
	virtual void GUIRender() override;

	void SetShaders(string vshaderpath, string fshaderpath);
};
