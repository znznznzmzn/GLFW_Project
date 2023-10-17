#pragma once

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
	void Detach(Shader* shader);

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
	
	// 각 쉐이더 path에 해당하는 쉐이더를 로드하여 프로그램 조합
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

	static void BindAll(GlobalBuffer* target); // GlobalBuffer 등록된 programs에 모두 Bind
	void Bind(GlobalBuffer* target) { target->Bind(program_id); } // GlobalBuffer bind
	void Bind(UniformBuffer* target, const string& name) // UniformBuffer bind
	{ target->UpdatePosition(program_id, name); }

	void Use() { glUseProgram(program_id); }

	uint GetProgramID() { return program_id; }
	string GetProgramKey() { return program_key; }
	uint GetAttachedShadersCount() { return attached_shaders.size(); }
	vector<Shader*>& GetAttachedShaders() { return attached_shaders; }

	virtual void GUIRender();
};