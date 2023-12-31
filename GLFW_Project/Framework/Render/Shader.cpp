#include "../Framework.h"

// Shader const
const unordered_map<string, GLenum> EXTENSION_TABLE = {
	{"vs"  , GL_VERTEX_SHADER  },
	{"vert", GL_VERTEX_SHADER  },
	{"fs"  , GL_FRAGMENT_SHADER},
	{"frag", GL_FRAGMENT_SHADER},
	{"gs"  , GL_GEOMETRY_SHADER},
	{"geom", GL_GEOMETRY_SHADER}
};
const unordered_map<GLenum, string> SHADER_TYPE_NAME_TABLE = {
	{GL_VERTEX_SHADER  , "GL_VERTEX_SHADER"  },
	{GL_FRAGMENT_SHADER, "GL_FRAGMENT_SHADER"},
	{GL_GEOMETRY_SHADER, "GL_GEOMETRY_SHADER"}
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

	if (Utility::CheckOK(shader_id, GL_COMPILE_STATUS, SHADER_TYPE_NAME_TABLE.at(shader_type) + "::COMPILATION"))
		cout << "SHADER::COMPILATION::SUCCESS >> " << SHADER_TYPE_NAME_TABLE.at(shader_type) << " " << shader_path << endl;
	
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
			cout << "Path = " << shader_path  << endl;
			return nullptr;
		}
	}

	Shader* result = Shader::Find(shader_path);
	if (result) return result;
	else result = new Shader(shader_path, shader_type);
	shaders[shader_path] = result;
	return result;
}

Shader* Shader::Load_VS(const string& shader_path)
{ return Shader::Load(shader_path, GL_VERTEX_SHADER); } // VertexShader
Shader* Shader::Load_FS(const string& shader_path)
{ return Shader::Load(shader_path, GL_FRAGMENT_SHADER); } // FragmentShader
Shader* Shader::Load_GS(const string& shader_path)
{ return Shader::Load(shader_path, GL_GEOMETRY_SHADER); } // GeometryShader

Shader* Shader::Find(const string & shader_path)
{ return (shaders.count(shader_path) > 0) ? shaders[shader_path] : nullptr; }

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

void Shader::PrintShader() {
	printf("SHADER >> id : %d, type : %s, path : %s \n", 
		shader_id, 
		SHADER_TYPE_NAME_TABLE.at(shader_type).c_str(), 
		shader_path.c_str()); 
}
