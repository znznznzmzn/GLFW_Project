#pragma once

class Shader {
protected:
	uint shader_id; // ���̴� id
	string shader_path; // ���̴� ��� == key
	GLenum shader_type; // ���̴� ���� (������� vs, fs, gs)

	static unordered_map<string, Shader*> shaders;

	Shader(const string& shader_path, GLenum shader_type);
	~Shader() = default;

	static GLenum Identify(const string& shader_path);
public:
	static Shader* Load(const string& shader_path, GLenum shader_type = NULL); // ���̴� �ε� if (shader_type == 0) Ÿ�� �ĺ�
	static Shader* Load_VS(const string& shader_path) { return Shader::Load(shader_path, GL_VERTEX_SHADER); } // VertexShader
	static Shader* Load_FS(const string& shader_path) { return Shader::Load(shader_path, GL_FRAGMENT_SHADER); } // FragmentShader
	static Shader* Load_GS(const string& shader_path) { return Shader::Load(shader_path, GL_GEOMETRY_SHADER); } // GeometryShader

	static Shader* Find(const string& shader_path) // find by path
	{ return (shaders.count(shader_path) > 0) ? shaders[shader_path] : nullptr; }

	static void Unload(const string& shader_path); // 1�� �����
	static void Clear(); // ��ü Ŭ����

	uint GetShaderID() { return shader_id; }
	string GetShaderPath() { return shader_path; }
	GLenum GetShaderType() { return shader_type; }

	void PrintShader();
};

class ShaderProgram { 
private:
	uint program_id = -1;
	string program_key = "";
	vector<Shader*> attached_shaders;
	bool dependency = false; // programs�� ���Ͽ� �������� �ִ°�
	// �� ���� �������� ���α׷��� ���������� ����

	static unordered_map<string, ShaderProgram*> programs; // ��ϵ� ���α׷���
	static const char AUTO_KEY_SPLITER = '&';
protected:
public:
	ShaderProgram(const string& program_key);
	~ShaderProgram();

	void Attach(Shader* shader); // ���̴��� ���δ�
	void Attach(const string& shader_path, GLenum shader_type = NULL) // ���̴� shader_path�� type �� ���� ���̴��� �����ϰ� ���� 
	{ Attach(Shader::Load(shader_path, shader_type)); }

	void Link(); // Attach�� ���̴����� ��ũ ��Ű�� �Լ�
	
	// attached_shaders�� path�� AUTO_KEY_SPLITER�� ���ļ� key ����
	static string MakeAutoKey(vector<string> paths);
	static string MakeAutoKey(ShaderProgram*& target);

	static vector<string> DecomposeAutoKey(string target_auto_key) // MakeAutoKey�� ������ key���� shader_path�� ����
	{ return Utility::String::Split(target_auto_key, AUTO_KEY_SPLITER); }

	static void Register(ShaderProgram*& target); // programs�� ���

	// key�� �ش��ϴ� program�� �ִ°�
	static bool IsExist(const string& key)
	{ return programs.count(key) > 0; }

	// key�� �ش��ϴ� program ��ȯ, ������ nullptr
	static ShaderProgram* GetProgram(const string & key) { return IsExist(key) ? programs[key] : nullptr; }

	// shaderKeys(path)�� ������ program�� ã���ֱ�, ������ nullptr
	static ShaderProgram* Find(vector<string> shaderPaths);

	// shaders�� ������ program�� ã���ֱ�, ������ nullptr
	static ShaderProgram* Find(vector<Shader*> shaders);
	

	// �� ���̴� path�� �ش���� ���̴��� �ε��Ͽ� ���α׷� ����
	// ���̴� ������ ���� �ٸ��� ������ �� ����
	static ShaderProgram* Create(
		const string& key,
		const string& v_shader_path, 
		const string& f_shader_path,
		const string& g_shader_path = nullptr);
	// key�� �ڵ������� �±�� �� path�� �°� ���α׷� ����
	static ShaderProgram* Create(
		const string& v_shader_path,
		const string& f_shader_path,
		const string& g_shader_path = nullptr);
	static ShaderProgram* Create(vector<string> shader_paths);

	static void Unload(const string& key); // key�� �ش��ϴ� program����
	static void Clear(); // programs ����

	static void BindAll(GlobalBuffer*& target); // GlobalBuffer ��ϵ� programs�� ��� Bind
	void Bind(GlobalBuffer*& target) { target->Bind(program_id); } // GlobalBuffer bind
	void Bind(UniformBuffer*& target, const string& name) // UniformBuffer bind
	{ target->UpdatePosition(program_id, name); }

	void Use() { glUseProgram(program_id); }

	uint GetProgramID() { return program_id; }
	string GetProgramKey() { return program_key; }
	uint GetAttachedShadersCount() { return attached_shaders.size(); }
	vector<Shader*> GetAttachedShaders() { return attached_shaders; }
};

//class Shader {
//private:
//	uint program_id;
//	string vshader_path;
//	string fshader_path;
//
//	static unordered_map<string, Shader*> shaders;
//	static void Delete(Shader*& id);
//
//	Shader(string const& vertex_shader_path, string const& fragment_shader_path);
//	~Shader() = default;
//public:
//	static string MakeKeyByPath(string const& vshader_path, string const& fshader_path)
//	{ return vshader_path + "&" + fshader_path; }
//
//	static Shader* Load(string const& vertex_shader_path, string const& fragment_shader_path);
//
//	static void Erase(string const& vshader_path, string const& fshader_path)
//	{ Erase(MakeKeyByPath(vshader_path, fshader_path)); }
//	static void Erase(string const& key);
//	static void Clear();
//
//	void Set() { glUseProgram(program_id); }
//
//	const uint& Get_ProgramID() { return program_id; }
//	const string& Get_VertexShaderPath() { return vshader_path; }
//	const string& Get_FragmentShaderPath() { return fshader_path; }
//	string Get_Key() { return Shader::MakeKeyByPath(vshader_path, fshader_path); }
//
//	static unordered_map<string, Shader*>& GetShaders() { return shaders; }
//	static Shader*& Get_Shader(string const& key) 
//	{ return shaders[key]; }
//	static Shader*& Get_Shader(string const& vshader_path, string const& fshader_path) 
//	{ return Get_Shader(MakeKeyByPath(vshader_path, fshader_path)); }
//};