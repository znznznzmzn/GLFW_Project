#pragma once

class Shader {
private:
	uint program_id;
	string vshader_path;
	string fshader_path;

	static unordered_map<string, Shader*> shaders;
	static void Delete(Shader*& id);

	Shader(string const& vertex_shader_path, string const& fragment_shader_path);
	~Shader() = default;
public:
	static string MakeKeyByPath(string const& vshader_path, string const& fshader_path)
	{ return vshader_path + "&" + fshader_path; }

	static Shader* Load(string const& vertex_shader_path, string const& fragment_shader_path);

	static void Erase(string const& vshader_path, string const& fshader_path)
	{ Erase(MakeKeyByPath(vshader_path, fshader_path)); }
	static void Erase(string const& key);
	static void Clear();

	void Set() { glUseProgram(program_id); }

	const uint& Get_ProgramID() { return program_id; }
	const string& Get_VertexShaderPath() { return vshader_path; }
	const string& Get_FragmentShaderPath() { return fshader_path; }
	string Get_Key() { return Shader::MakeKeyByPath(vshader_path, fshader_path); }

	static unordered_map<string, Shader*>& GetShaders() { return shaders; }
	static Shader*& Get_Shader(string const& key) 
	{ return shaders[key]; }
	static Shader*& Get_Shader(string const& vshader_path, string const& fshader_path) 
	{ return Get_Shader(MakeKeyByPath(vshader_path, fshader_path)); }
};