#pragma once

class Model : public Transform { // 뼈대, 노드 없는 단순 모델 (모든 노드들의 위치 = 0)
	bool is_shader_locked = false;
protected:
	int commonProgramID = 0;
	ModelData* data = nullptr;
	map<Material*, vector<ModelMesh*>> render_map;
	MatrixBuffer* wBuffer = nullptr;

	virtual void InitUniformBuffer();
	virtual void SetUniformBuffer();

	void ClearModelMap();
	void LoadModel(string model_name);
public:
	Model() = default;
	Model(string model_name);
	~Model();
	virtual void Render();
	virtual void GUIRender() override;

	void LockShader(bool isLock = true) { is_shader_locked = isLock; }

	void SetShaders(string vshaderpath, string fshaderpath);
};
