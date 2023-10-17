#include "../Framework.h"

// Shader const
const unordered_map<string, GLenum> EXTENSION_TABLE = {
	{"vs"  , GL_VERTEX_SHADER},
	{"vert", GL_VERTEX_SHADER},
	{"fs"  , GL_FRAGMENT_SHADER},
	{"frag", GL_FRAGMENT_SHADER},
	{"gs"  , GL_GEOMETRY_SHADER},
	{"geom", GL_GEOMETRY_SHADER}
};
const unordered_map<GLenum, string> SHADER_TYPE_NAME_TABLE = {
	{GL_VERTEX_SHADER  , "GL_VERTEX_SHADER"  },
	{GL_FRAGMENT_SHADER, "GL_FRAGMENT_SHADER"  },
	{GL_GEOMETRY_SHADER, "GL_GEOMETRY_SHADER"  }
};

// Shader
unordered_map<string, Shader*> Shader::shaders;
Shader::Shader(const string& shader_path, GLenum shader_type) {
	this->shader_path = shader_path;
	this->shader_type = shader_type;

	BinaryReader* reader = new BinaryReader();
	bool isOK = false;

	reader->OpenFile(shader_path);
	string shaderSource = reader->ReadAll();
	const char* sourcePointer = shaderSource.c_str();
	shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &sourcePointer, NULL);
	glCompileShader(shader_id);

	Utility::CheckOK(shader_id, GL_COMPILE_STATUS, SHADER_TYPE_NAME_TABLE.at(shader_type) + "::COMPILATION");
		
	shaderSource.clear();
	SAFE_DELETE(sourcePointer);
	SAFE_DELETE(reader);
}

GLenum Shader::Identify(const string& shader_path) {
	string extension = Utility::String::GetExtension(shader_path);
	if (EXTENSION_TABLE.count(extension) > 0) return EXTENSION_TABLE.at(extension);
	else return NULL;
}

Shader* Shader::Load(const string& shader_path, GLenum shader_type) {
	if (shader_type == NULL) {
		shader_type = Shader::Identify(shader_path);
		if (shader_type == NULL) {
			cout << "FATAL_ERORR!! unidentified shader_type = " << shader_type << ", returning nullptr" << endl;
			return nullptr;
		}
	}

	Shader* target = Shader::Find(shader_path);
	if (target) return target;
	return new Shader(shader_path, shader_type);
}

void Shader::Unload(const string& shader_path) {
	Shader* target = Shader::Find(shader_path);
	if (target) {
		glDeleteShader(target->GetShaderID());
		SAFE_DELETE(target);
		shaders.erase(shader_path);
	}
}

void Shader::Clear() {
	for (auto& elem : shaders) {
		glDeleteShader(elem.second->GetShaderID());
		SAFE_DELETE(elem.second);
	}
	shaders.clear();
}

void Shader::PrintShader() 
{ printf("SHADER >> id : %d, type : %s, path : %s \n", shader_id, SHADER_TYPE_NAME_TABLE.at(shader_type), shader_path); }



// ShaderProgram
unordered_map<string, ShaderProgram*> ShaderProgram::programs;

ShaderProgram::ShaderProgram(const string& program_key) {
	this->program_key = program_key;
	this->program_id = glCreateProgram();
}
ShaderProgram::~ShaderProgram() { // programs에 대하여 의존성이 없으면 glDeleteProgram 
	if (!dependency) glDeleteProgram(this->program_id);
	// attached_shaders.clear(); 궂이?
}

void ShaderProgram::Attach(Shader* shader) {
	for (Shader* elem : attached_shaders) {
		if (elem->GetShaderID() == shader->GetShaderID()) {
			cout << "Already Attached, shader_id = " << shader->GetShaderID() << endl;
			return;
		}
	}
	glAttachShader(program_id, shader->GetShaderID());
	attached_shaders.emplace_back(shader);
}

void ShaderProgram::Link() {
	glLinkProgram(program_id);
	Utility::CheckOK(program_id, GL_LINK_STATUS, "SHADER::LINK_FAILED");
}

string ShaderProgram::MakeAutoKey(vector<string> paths) {
	string key = "";
	int n = paths.size();
	for (int i = 0; i < n; i++) {
		key += paths[i];
		if (i < n - 1) key += AUTO_KEY_SPLITER;
	}
	return key;
}
string ShaderProgram::MakeAutoKey(ShaderProgram*& target) {
	vector<string> paths;
	for (Shader* shader : target->attached_shaders)
		paths.emplace_back(shader->GetShaderPath());
	return ShaderProgram::MakeAutoKey(paths);
}

void ShaderProgram::Register(ShaderProgram*& target) {
	//- 중복검사?
	programs[target->GetProgramKey()] = target;
	target->dependency = true;
}

ShaderProgram* ShaderProgram::Find(vector<string> shaderPaths) {
	for (auto& program : programs) {
		vector<Shader*>& target_shaders = program.second->attached_shaders;
		if (target_shaders.size() == shaderPaths.size()) {
			int correction = 0;
			for (Shader* shader : target_shaders) {
				for (string shader_path : shaderPaths) {
					if (shader_path.compare(shader->GetShaderPath()) == 0) correction++;
				}
			}
			if (correction == target_shaders.size()) return program.second;
		}
	}
	return nullptr;
}

ShaderProgram* ShaderProgram::Find(vector<Shader*> shaders) {
	vector<string> shaderPaths;
	for (Shader* elem : shaders)
		shaderPaths.emplace_back(elem->GetShaderPath());
	return ShaderProgram::Find(shaderPaths);
}

ShaderProgram* ShaderProgram::Create(
	const string& key, 
	const string& v_shader_path, 
	const string& f_shader_path, 
	const string& g_shader_path) {
	ShaderProgram* result = GetProgram(key);

	if (result == nullptr) {
		result = new ShaderProgram(key);
		result->Attach(Shader::Load_VS(v_shader_path));
		result->Attach(Shader::Load_FS(f_shader_path));
		if(g_shader_path.size() != 0)
			result->Attach(Shader::Load_GS(g_shader_path));
		result->Link();
		ShaderProgram::Register(result);
	}

	return result;
}
ShaderProgram* ShaderProgram::Create(
	const string& v_shader_path, 
	const string& f_shader_path, 
	const string& g_shader_path) {
	vector<string> shader_paths;

	if (v_shader_path.size() != 0) shader_paths.emplace_back(v_shader_path);
	if (f_shader_path.size() != 0) shader_paths.emplace_back(f_shader_path);
	if (g_shader_path.size() != 0) shader_paths.emplace_back(g_shader_path);

	return ShaderProgram::Create(shader_paths);
}
ShaderProgram* ShaderProgram::Create(vector<string> shader_paths) {
	ShaderProgram* result = Find(shader_paths);

	if (result == nullptr) {
		string key = ShaderProgram::MakeAutoKey(shader_paths);
		result = new ShaderProgram(key);

		for (string elem : shader_paths) result->Attach(Shader::Load(elem));
		result->Link();
		ShaderProgram::Register(result);
	}

	return result;
}

void ShaderProgram::Unload(const string& key) {
	ShaderProgram* target = ShaderProgram::GetProgram(key);
	if (target) {
		SAFE_DELETE(target);
		programs.erase(key);
	}
}

void ShaderProgram::Clear() {
	for (auto& elem : programs) {
		glDeleteProgram(elem.second->GetProgramID());
		SAFE_DELETE(elem.second);
	}
	programs.clear();
}
