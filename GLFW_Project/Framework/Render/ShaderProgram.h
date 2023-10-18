#pragma once

class ShaderProgram { 
protected:
	uint program_id = -1;
	string program_key = "";
	vector<Shader*> attached_shaders;
	bool dependency = false;

	static unordered_map<string, ShaderProgram*> programs; 
public:
	ShaderProgram(const string& program_key);
	~ShaderProgram();

	void Attach(Shader* shader); 
	void Attach(const string& shader_path, GLenum shader_type = NULL); 

	void Detach(Shader* shader);

	void Link();
	
	static string MakeAutoKey(vector<string> paths);
	static string MakeAutoKey(ShaderProgram*& target);

	static vector<string> DecomposeAutoKey(string target_auto_key);
	
	static void Register(ShaderProgram*& target);

	static bool IsExist(const string& key);

	static ShaderProgram* GetProgram(const string& key);

	static ShaderProgram* Find(vector<string> shaderPaths);
	static ShaderProgram* Find(vector<Shader*> shaders);
	
	static ShaderProgram* Create(
		const string& key,
		const string& v_shader_path, 
		const string& f_shader_path,
		const string& g_shader_path = "");
	static ShaderProgram* Create(
		const string& v_shader_path,
		const string& f_shader_path,
		const string& g_shader_path = "");
	static ShaderProgram* Create(vector<string> shader_paths);

	static void Unload(const string& key);
	static void Clear();

	static void BindAll(GlobalBuffer* target);
	void Bind(GlobalBuffer* target) { target->Bind(program_id); }
	void Bind(UniformBuffer* target, const string& name)
	{ target->UpdatePosition(program_id, name); }

	void Use() { glUseProgram(program_id); }

	const uint& GetProgramID() { return program_id; }
	const string& GetProgramKey() { return program_key; }
	const uint& GetAttachedShadersCount() { return attached_shaders.size(); }
	vector<Shader*>& GetAttachedShaders() { return attached_shaders; }

	virtual void GUIRender();
};