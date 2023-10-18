#include "../Framework.h"

const char AUTO_KEY_SPLITER = '&';

// ShaderProgram
unordered_map<string, ShaderProgram*> ShaderProgram::programs;
ShaderProgram::ShaderProgram(const string& program_key) {
	this->program_key = program_key;
	this->program_id = glCreateProgram();
}
ShaderProgram::~ShaderProgram() { 
	if (!dependency) glDeleteProgram(this->program_id);
}

void ShaderProgram::Attach(Shader* shader) {
	for (Shader*& elem : attached_shaders) {
		if (elem->GetShaderPath().compare(shader->GetShaderPath()) == 0) {
			cout << "Already Attached, shader = " << shader->GetShaderPath() << endl;
			return;
		}
	}
	attached_shaders.push_back(shader);
	glAttachShader(program_id, shader->GetShaderID());
}
void ShaderProgram::Attach(const string& shader_path, GLenum shader_type) 
{ Attach(Shader::Load(shader_path, shader_type)); }

void ShaderProgram::Detach(Shader* shader) {
	glDetachShader(program_id, shader->GetShaderID());
	bool isfound = false;
	for (uint i = 0; i < attached_shaders.size(); i++) {
		if (attached_shaders[i]->GetShaderPath().compare(shader->GetShaderPath()) == 0) {
			attached_shaders.erase(attached_shaders.begin() + i);
			return; 
		}
	} 
	cout << "The shader dosn,t exist, shader = " << shader->GetShaderID() << endl;
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
	for (Shader*& elem : target->attached_shaders)
		paths.emplace_back(elem->GetShaderPath());
	return ShaderProgram::MakeAutoKey(paths);
}

vector<string> ShaderProgram::DecomposeAutoKey(string target_auto_key)
{ return Utility::String::Split(target_auto_key, AUTO_KEY_SPLITER); }

void ShaderProgram::Register(ShaderProgram*& target) {
	programs[target->GetProgramKey()] = target;
	target->dependency = true;
}

bool ShaderProgram::IsExist(const string& key) { return programs.count(key) > 0; }

ShaderProgram* ShaderProgram::GetProgram(const string& key) 
{ return IsExist(key) ? programs[key] : nullptr; }

ShaderProgram* ShaderProgram::Find(vector<string> shaderPaths) {
	for (auto& program : programs) {
		vector<Shader*>& target_shaders = program.second->attached_shaders;
		if (target_shaders.size() == shaderPaths.size()) {
			int correction = 0;
			for (Shader*& shader : target_shaders) {
				for (string& path : shaderPaths) {
					if (path.compare(shader->GetShaderPath()) == 0) correction++;
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
		if (g_shader_path.size() != 0)
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

void ShaderProgram::BindAll(GlobalBuffer* target) 
{ for (auto& elem : programs) elem.second->Bind(target); }

void ShaderProgram::GUIRender() {
	if (ImGui::TreeNode(("Program : " + program_key).c_str())) {
		//- 쉐이더 조작은 추후 작업
		//- 현재로선 쉐이더 리스트만 보이도록 작성
		//- { // program_key
		//- 	char name_buf[128] = "";
		//- 	if (ImGui::InputTextWithHint("program_key", program_key.c_str(), name_buf, IM_ARRAYSIZE(name_buf))) {
		//- 		if (KEY_DOWN(ImGuiKey_Enter)) {
		//- 			if (dependency) programs.erase(program_key);
		//- 			program_key = string(name_buf);
		//- 			if (dependency) programs[program_key] = this;
		//- 		}
		//- 	}
		//- }
		{ // Shader
			for (Shader*& shader : attached_shaders) {
				ImGui::LabelText("", shader->GetShaderPath().c_str()); 
				
				//- ImGui::SameLine();
				//- if (ImGui::Button("Detach")) Detach(shader);
			}
			//- string shader_path = EditorGUI::Dialog_Button_Pattern("Attach Shader", "AttachShader",
			//- 	"Attach Shader", EditorGUI::filter_shader.c_str(), "Assets\\GLSL\\.");
			//- if (shader_path.size() != 0) Attach(shader_path);
			//- if (ImGui::Button("Try Link")) Link();
		}
		ImGui::TreePop();
	}
}
