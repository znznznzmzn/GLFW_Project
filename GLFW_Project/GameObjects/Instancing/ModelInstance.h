#pragma once

class ModelInstance : public Instance {
protected:
	int controller_index = 0;
	ModelData* data = nullptr;
	map<Material*, vector<ModelMesh*>> render_map;

	void LoadModel(string model_name);
	void SetShaders(string vshader_path, string fshader_path);

	void ClearModelMap();

	virtual void AttachInstanceMatrix(uint start_index = 6); // layout(location = 6) in mat4 in_instance_matrix;

	virtual void InitUniformBuffer() { }
	virtual void SetUniformBuffer () { }

	ModelInstance() = default;
public:
	ModelInstance(string model_name, uint instanceCount = 100);

	virtual void Render() override; // ±×¸®±â
	virtual void GUIRender();
};

