#pragma once

class Material {
private:
	string path = "";
	string name = "Default";

	Shader* shader = nullptr;
	bool is_shader_locked = false;

	Texture* diffuseMap = nullptr;
	Texture* specularMap = nullptr;
	Texture* normalMap = nullptr;

	TextureBuffer* diffuseMapBuffer = nullptr;
	TextureBuffer* specularMapBuffer = nullptr;
	TextureBuffer* normalMapBuffer = nullptr;

	class MaterialBuffer {
	public:
		MaterialBuffer(const uint& targetProgramID);
		~MaterialBuffer();
		void Set();
		void Update(const uint& targetProgramID);
		Vector4Buffer* diffuseBuffer = nullptr;
		Vector4Buffer* specularBuffer = nullptr;
		Vector4Buffer* emissiveBuffer = nullptr;
		FloatBuffer* shininessBuffer = nullptr;
		IntBuffer* hasDiffuseBuffer = nullptr;
		IntBuffer* hasSpecularBuffer = nullptr;
		IntBuffer* hasNormalBuffer = nullptr;
	}* mBuffer = nullptr;

	void init(const int& shader_id);
public:
	Material();
	Material(const string& vshader_path, const string& fshader_path);
	~Material();

	void Set_Shader() { shader->Set(); }
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

	const uint& Get_ProgramID() { return shader->Get_ProgramID(); }

	void GUIRender();

	void SaveMaterial(string path);
	void LoadMaterial(string path);

	void LockShader(bool isLock = true) { is_shader_locked = isLock; }

	Vector4& GetData_Diffuse () { return mBuffer->diffuseBuffer ->data; }
	Vector4& GetData_Specular() { return mBuffer->specularBuffer->data; }
	Vector4& GetData_Emissive() { return mBuffer->emissiveBuffer->data; }
	float& GetData_Shininess () { return mBuffer->shininessBuffer->data; }
};