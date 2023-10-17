#pragma once

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
	void Detach(Shader* shader);

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
	
	// �� ���̴� path�� �ش��ϴ� ���̴��� �ε��Ͽ� ���α׷� ����
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

	static void BindAll(GlobalBuffer* target); // GlobalBuffer ��ϵ� programs�� ��� Bind
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