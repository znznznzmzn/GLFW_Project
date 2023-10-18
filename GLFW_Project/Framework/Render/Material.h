#pragma once

class Material {
private:
	string path = "";
	string name = "Default";

	ShaderProgram* materialProgram = nullptr;
	bool is_shader_locked = false;

	Texture* diffuseMap = nullptr;
	Texture* specularMap = nullptr;
	Texture* normalMap = nullptr;

	TextureBuffer* diffuseMapBuffer = nullptr;
	TextureBuffer* specularMapBuffer = nullptr;
	TextureBuffer* normalMapBuffer = nullptr;

	MaterialBuffer* mBuffer = nullptr;

	void bind();
	void init(const int& shader_id);
public:
	Material();
	Material(const string& vshader_path, const string& fshader_path);
	~Material();

	void UseProgram() { materialProgram->Use(); }
	void Set();

	void SetDiffuseMap (string path);
	void SetSpecularMap(string path);
	void SetNormalMap  (string path);

	void SetShader(const string& vshader_path, const string& fshader_path);

	void SetName(string name) { this->name = name; }
	string GetName() { return name; }

	string GetDiffusePath();
	string GetSpecularPath();
	string GetNormalPath();

	const uint& Get_ProgramID() { return materialProgram->GetProgramID(); }

	void GUIRender();

	void SaveMaterial(string path);
	void LoadMaterial(string path);

	void LockShader(bool isLock = true) { is_shader_locked = isLock; }

	Vector4& GetData_Diffuse () { return mBuffer->data.diffuse  ; }
	Vector4& GetData_Specular() { return mBuffer->data.specular ; }
	Vector4& GetData_Emissive() { return mBuffer->data.emissive ; }
	float& GetData_Shininess () { return mBuffer->data.shininess; }
};