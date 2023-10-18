#pragma once

class Shader {
protected:
	uint shader_id;
	string shader_path;
	GLenum shader_type;

	static unordered_map<string, Shader*> shaders;

	Shader(const string& shader_path, GLenum shader_type);
	~Shader() = default;

	static GLenum Identify(const string& shader_path);
public:
	static Shader* Load(const string& shader_path, GLenum shader_type = NULL);
	static Shader* Load_VS(const string& shader_path);
	static Shader* Load_FS(const string& shader_path);
	static Shader* Load_GS(const string& shader_path);

	static Shader* Find(const string& shader_path);

	static void Unload(const string& shader_path);
	static void Clear();

	uint GetShaderID() { return shader_id; }
	string GetShaderPath() { return shader_path; }
	GLenum GetShaderType() { return shader_type; }

	void PrintShader();
};
