#pragma once

class Shader {
protected:
	uint shader_id; // 쉐이더 id
	string shader_path; // 쉐이더 경로 == key
	GLenum shader_type; // 쉐이더 종류 (현재까지 vs, fs, gs)

	static unordered_map<string, Shader*> shaders;

	Shader(const string& shader_path, GLenum shader_type);
	~Shader() = default;

	static GLenum Identify(const string& shader_path);
public:
	static Shader* Load(const string& shader_path, GLenum shader_type = NULL); // 쉐이더 로드 if (shader_type == 0) 타입 식별
	static Shader* Load_VS(const string& shader_path) { return Shader::Load(shader_path, GL_VERTEX_SHADER); } // VertexShader
	static Shader* Load_FS(const string& shader_path) { return Shader::Load(shader_path, GL_FRAGMENT_SHADER); } // FragmentShader
	static Shader* Load_GS(const string& shader_path) { return Shader::Load(shader_path, GL_GEOMETRY_SHADER); } // GeometryShader

	static Shader* Find(const string& shader_path) // find by path
	{ return (shaders.count(shader_path) > 0) ? shaders[shader_path] : nullptr; }

	static void Unload(const string& shader_path); // 1개 지우기
	static void Clear(); // 전체 클리어

	uint GetShaderID() { return shader_id; }
	string GetShaderPath() { return shader_path; }
	GLenum GetShaderType() { return shader_type; }

	void PrintShader();
};
