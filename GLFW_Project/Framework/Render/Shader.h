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

class ShaderProgram { 
private:
	uint program_id = -1;
	string program_key = "";
	vector<Shader*> attached_shaders;
	bool dependency = false; // programs에 대하여 의존성이 있는가
	// 에 따라서 스스로의 프로그램을 지워버릴지 결정

	static unordered_map<string, ShaderProgram*> programs; // 등록된 프로그램들
	static const char AUTO_KEY_SPLITER = '&';
protected:
public:
	ShaderProgram(const string& program_key);
	~ShaderProgram();

	void Attach(Shader* shader); // 쉐이더를 붙인다
	void Attach(const string& shader_path, GLenum shader_type = NULL) // 쉐이더 shader_path와 type 에 따라 쉐이더를 생성하고 붙임 
	{ Attach(Shader::Load(shader_path, shader_type)); }

	void Link(); // Attach한 쉐이더들을 링크 시키는 함수
	
	// attached_shaders의 path를 AUTO_KEY_SPLITER와 합쳐서 key 생성
	static string MakeAutoKey(vector<string> paths);
	static string MakeAutoKey(ShaderProgram*& target);

	static vector<string> DecomposeAutoKey(string target_auto_key) // MakeAutoKey로 생성된 key값을 shader_path로 분해
	{ return Utility::String::Split(target_auto_key, AUTO_KEY_SPLITER); }

	static void Register(ShaderProgram*& target); // programs에 등록

	// key에 해당하는 program이 있는가
	static bool IsExist(const string& key)
	{ return programs.count(key) > 0; }

	// key에 해당하는 program 반환, 없으면 nullptr
	static ShaderProgram* GetProgram(const string & key) { return IsExist(key) ? programs[key] : nullptr; }

	// shaderKeys(path)로 구성된 program을 찾아주기, 없으면 nullptr
	static ShaderProgram* Find(vector<string> shaderPaths);

	// shaders로 구성된 program을 찾아주기, 없으면 nullptr
	static ShaderProgram* Find(vector<Shader*> shaders);
	

	// 각 쉐이더 path에 해당흐는 쉐이더를 로드하여 프로그램 조합
	// 쉐이더 종류에 따라 다르게 생성할 수 있음
	static ShaderProgram* Create(
		const string& key,
		const string& v_shader_path, 
		const string& f_shader_path,
		const string& g_shader_path = nullptr);
	// key는 자동생성에 맞기고 각 path에 맞게 프로그램 생성
	static ShaderProgram* Create(
		const string& v_shader_path,
		const string& f_shader_path,
		const string& g_shader_path = nullptr);
	static ShaderProgram* Create(vector<string> shader_paths);

	static void Unload(const string& key); // key에 해당하는 program제거
	static void Clear(); // programs 비우기

	static void BindAll(GlobalBuffer*& target); // GlobalBuffer 등록된 programs에 모두 Bind
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