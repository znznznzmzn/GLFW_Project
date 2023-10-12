#include "../../Framework.h"

unordered_map<string, Shader*> Shader::shaders;

Shader::Shader(string const& vertex_shader_path, string const& fragment_shader_path) {
	vshader_path = vertex_shader_path;
	fshader_path = fragment_shader_path;
	cout << "SHADER::CREATE::REQUESTED >> " << vshader_path << " & " << fshader_path << endl;
	BinaryReader* reader = new BinaryReader();
	bool isOK = false;

	int vshader_id;
	int fshader_id;
	{ // Vertex
		reader->OpenFile(vshader_path);
		string vertexShaderSource = reader->ReadAll();
		const char* sourcePointer = vertexShaderSource.c_str();
		vshader_id = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vshader_id, 1, &sourcePointer, NULL);
		glCompileShader(vshader_id);
		vertexShaderSource.clear();
	}
	isOK = Utility::CheckOK(vshader_id, GL_COMPILE_STATUS, "SHADER::VERTEX::COMPILATION_FAILED");

	{ // Fragment
		reader->OpenFile(fshader_path);
		string fragmentShaderSource = reader->ReadAll();
		const char* sourcePointer = fragmentShaderSource.c_str();
		fshader_id = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fshader_id, 1, &sourcePointer, NULL);
		glCompileShader(fshader_id);
		fragmentShaderSource.clear();
	}
	isOK = Utility::CheckOK(fshader_id, GL_COMPILE_STATUS, "SHADER::FRAGMENT::COMPILATION_FAILED");

	SAFE_DELETE(reader);

	// Link
	program_id = glCreateProgram();
	glAttachShader(program_id, vshader_id);
	glAttachShader(program_id, fshader_id);
	glLinkProgram(program_id);
	isOK = Utility::CheckOK(program_id, GL_LINK_STATUS, "SHADER::LINK_FAILED");

	// Delete 
	glDeleteShader(vshader_id); 
	glDeleteShader(fshader_id);

	if (isOK) cout << "SHADER::CREATE::WELL_DONE >> " << Get_Key() << endl;
}

Shader* Shader::Load(string const& vertex_shader_path, string const& fragment_shader_path) {
	string key = Shader::MakeKeyByPath(vertex_shader_path, fragment_shader_path); 
	if (shaders.count(key) > 0) return shaders[key];
	else {
		Shader* newShader = new Shader(vertex_shader_path, fragment_shader_path);
		shaders[key] = newShader;
		return newShader;
	}
}

void Shader::Erase(string const& key) {
	Delete(shaders[key]);
	shaders.erase(key);
}
void Shader::Clear() {
	for (pair<string, Shader*> elem : shaders) Delete(elem.second);
	shaders.clear();
}

void Shader::Delete(Shader*& target) { glDeleteProgram(target->program_id); }
