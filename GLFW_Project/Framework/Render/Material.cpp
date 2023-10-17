#include "../../Framework.h"

void Material::init(const int& shader_id) {
	mBuffer = new MaterialBuffer(shader_id);
	diffuseMapBuffer  = new TextureBuffer(shader_id, "diffuseMap" );
	specularMapBuffer = new TextureBuffer(shader_id, "specularMap");
	normalMapBuffer   = new TextureBuffer(shader_id, "normalMap"  );
	diffuseMapBuffer ->slot = 0;
	specularMapBuffer->slot = 1;
	normalMapBuffer  ->slot = 2;
}

Material::Material() { init(0); }
Material::Material(const string& vshader_path, const string& fshader_path) {
	shader = Shader::Load(vshader_path, fshader_path);
	shader->Set();
	init(shader->Get_ProgramID());
}
Material::~Material() { 
	SAFE_DELETE(mBuffer); 
	SAFE_DELETE(diffuseMapBuffer);
	SAFE_DELETE(specularMapBuffer);
	SAFE_DELETE(normalMapBuffer);
}

void Material::Set() {
	shader->Set();
	if (diffuseMap) {
		diffuseMap->Set(diffuseMapBuffer->slot);
		diffuseMapBuffer->Set();
	}
	if (specularMap) {
		specularMap->Set(specularMapBuffer->slot);
		specularMapBuffer->Set();
	}
	if (normalMap) {
		normalMap->Set(normalMapBuffer->slot);
		normalMapBuffer->Set();
	}
	mBuffer->Set();
}

void Material::SetShader(const string& vshader_path, const string& fshader_path) { 
	if (is_shader_locked) return;
	shader = Shader::Load(vshader_path, fshader_path); 
	shader->Set();
	mBuffer->Update(shader->Get_ProgramID());
	diffuseMapBuffer ->UpdatePosition(shader->Get_ProgramID(), "diffuseMap");
	specularMapBuffer->UpdatePosition(shader->Get_ProgramID(), "specularMap");
	normalMapBuffer  ->UpdatePosition(shader->Get_ProgramID(), "normalMap");
}

void Material::SetDiffuseMap(string path) {
	mBuffer->hasDiffuseBuffer->data = (path.length() != 0);
	diffuseMap = (mBuffer->hasDiffuseBuffer->data) ? Texture::Load(path) : nullptr;
}
void Material::SetSpecularMap(string path) {
	mBuffer->hasSpecularBuffer->data = (path.length() != 0); 
	specularMap = (mBuffer->hasSpecularBuffer->data) ? Texture::Load(path) : nullptr; 
}
void Material::SetNormalMap(string path) {
	mBuffer->hasNormalBuffer->data = (path.length() != 0); 
	normalMap = (mBuffer->hasNormalBuffer->data) ? Texture::Load(path) : nullptr;
}

string Material::GetDiffusePath() {
	if (diffuseMap) return diffuseMap->GetPath();
	return string();
}
string Material::GetSpecularPath() {
	if (specularMap) return specularMap->GetPath();
	return string();
}
string Material::GetNormalPath() {
	if (normalMap) return normalMap->GetPath();
	return string();
}

void Material::GUIRender() {
	if (ImGui::TreeNode(("Material : " + name).c_str())) {
		{ // Material Name
			char name_buf[128] = "";
			if (ImGui::InputTextWithHint("Material Name", name.c_str(), name_buf, IM_ARRAYSIZE(name_buf)))
				if (KEY_DOWN(ImGuiKey_Enter)) name = string(name_buf);
		}

		{ // Shader
			string shader_path = EditorGUI::Dialog_Button_Pattern("Load Shader", "LoadShader", 
				"Load Shader", "ShaderFiles{.frag,.vert}", "Assets\\GLSL\\."); 
			if (shader_path.size() != 0) { 
				string vpath, fpath; 
				if (shader_path.find(".frag") == string::npos) { // 선택한게 .frag가 아니라면
					vpath = shader_path; 
					fpath = shader_path.substr(0, shader_path.find_last_of('.')) + ".frag";
				}
				else {
					vpath = shader_path.substr(0, shader_path.find_last_of('.')) + ".vert";
					fpath = shader_path; 
				}
				SetShader(vpath, fpath); 
			}
			ImGui::LabelText("Vertex Shader", shader->Get_VertexShaderPath().c_str());
			ImGui::LabelText("Fragment Shader", shader->Get_FragmentShaderPath().c_str());
		}

		// Material Color
		ImGui::ColorEdit4 ("Diffuse" , (float*)&mBuffer->diffuseBuffer ->data);
		ImGui::ColorEdit4 ("Specular", (float*)&mBuffer->specularBuffer->data);
		ImGui::ColorEdit4 ("Emissive", (float*)&mBuffer->emissiveBuffer->data);

		ImGui::SliderFloat("Shiness" , (float*)&mBuffer->shininessBuffer->data, 1.0f, 50.0f);

		// Material has Diffuse Specular Normal 
		ImGui::Checkbox("Is Diffuse   ", (bool*)&mBuffer->hasDiffuseBuffer->data);
		ImGui::SameLine();
		ImGui::Checkbox("Is Specular  ", (bool*)&mBuffer->hasSpecularBuffer->data);
		ImGui::SameLine();
		ImGui::Checkbox("Is Normal    ", (bool*)&mBuffer->hasNormalBuffer->data);

		{ // Material Images
			ImVec2 xbox = { 24, 24 };
			if (EditorGUI::Dialog_ImageButton_Pattern("Diffuse", "LoadDiffuse", diffuseMap))
				SetDiffuseMap(diffuseMap->GetPath());
			ImGui::SameLine();
			if (ImGui::Button("DX", xbox)) SetDiffuseMap("");
			ImGui::SameLine();
			if (EditorGUI::Dialog_ImageButton_Pattern("Specular", "LoadSpecular", specularMap))
				SetSpecularMap(specularMap->GetPath());
			ImGui::SameLine();
			if (ImGui::Button("SX", xbox)) SetSpecularMap("");
			ImGui::SameLine();
			if (EditorGUI::Dialog_ImageButton_Pattern("Normal", "LoadNormal",normalMap))
				SetNormalMap(normalMap->GetPath());
			ImGui::SameLine();
			if (ImGui::Button("NX", xbox)) SetNormalMap("");
		}

		// Save Load Material
		if (ImGui::Button("Save Material", { 160, 25 })) SaveMaterial(path);
		ImGui::SameLine();
		ImGui::LabelText("%s", path.c_str()); 

		if (ImGui::Button("Save Material At...", { 160, 25 }))
			IMGUI_DIALOG->OpenDialog("SaveMaterial", "Save Material At", ".mat", "Assets\\Data\\Materials\\.");
		ImGui::SameLine(); // SameLine 때문에 EditorGUI::Dialog_Button_Pattern 사용 불가
		if (ImGui::Button("Load Material From...", { 160, 25 })) 
			IMGUI_DIALOG->OpenDialog("LoadMaterial", "Load Material From", ".mat", "Assets\\Data\\Materials\\.");

		if (IMGUI_DIALOG->Display("LoadMaterial")) { 
			if (IMGUI_DIALOG->IsOk()) {
				string mat_path = IMGUI_DIALOG->GetFilePathName();
				string::size_type n = mat_path.find("Assets");
				if (n == string::npos) std::cout << "Wrong mat_path = " << mat_path << endl;
				else {
					LoadMaterial(mat_path.substr(n));
					IMGUI_DIALOG->Close(); 
				}
			} IMGUI_DIALOG->Close();
		}
		if (IMGUI_DIALOG->Display("SaveMaterial")) { 
			if (IMGUI_DIALOG->IsOk()) {
				string mat_path = IMGUI_DIALOG->GetFilePathName();
				string::size_type n = mat_path.find("Assets");
				if (n == string::npos) std::cout << "Wrong mat_path = " << mat_path << endl;
				else {
					SaveMaterial(mat_path.substr(n));
					IMGUI_DIALOG->Close();
				}
			} IMGUI_DIALOG->Close(); 
		}
		ImGui::TreePop();
	}
}

void Material::SaveMaterial(string path) {
	if (path.size() != 0) path = "Assets/Data/Materials/" + name + ".mat";
	BinaryWriter* w = new BinaryWriter(path);

	w->WriteLine(name); // 이름

	if (shader != nullptr) {
		w->WriteLine(shader->Get_VertexShaderPath());
		w->WriteLine(shader->Get_FragmentShaderPath());
	}
	else {
		w->WriteLine("");
		w->WriteLine("");
	}

	w->WriteLine(Utility::String::From(mBuffer->diffuseBuffer ->data));
	w->WriteLine(Utility::String::From(mBuffer->specularBuffer->data));
	w->WriteLine(Utility::String::From(mBuffer->emissiveBuffer->data));

	w->WriteLine(Utility::String::From(mBuffer->hasDiffuseBuffer ->data)); 
	w->WriteLine(Utility::String::From(mBuffer->hasSpecularBuffer->data)); 
	w->WriteLine(Utility::String::From(mBuffer->hasNormalBuffer  ->data)); 
	w->WriteLine(Utility::String::From(mBuffer->shininessBuffer  ->data));

	w->WriteLine((diffuseMap ) ? diffuseMap ->GetPath() : ""); // 각 텍스쳐 path
	w->WriteLine((specularMap) ? specularMap->GetPath() : "");
	w->WriteLine((normalMap  ) ? normalMap  ->GetPath() : "");

	SAFE_DELETE(w);

	this->path = path;
}
void Material::LoadMaterial(string path) {
	if (path.size() == 0) {
		cout << "Warning no material path = " << path << endl;
		return;
	}
	BinaryReader* r = new BinaryReader(path);

	name = r->ReadLine();

	{
		string vpath = r->ReadLine();
		string fpath = r->ReadLine();
		if(vpath.length() != 0 && fpath.length() != 0) SetShader(vpath, fpath);
	}

	mBuffer->diffuseBuffer ->data = Utility::String::ToVector4(r->ReadLine());
	mBuffer->specularBuffer->data = Utility::String::ToVector4(r->ReadLine());
	mBuffer->emissiveBuffer->data = Utility::String::ToVector4(r->ReadLine());

	mBuffer->hasDiffuseBuffer ->data = Utility::String::ToInt  (r->ReadLine());
	mBuffer->hasSpecularBuffer->data = Utility::String::ToInt  (r->ReadLine());
	mBuffer->hasNormalBuffer  ->data = Utility::String::ToInt  (r->ReadLine());
	mBuffer->shininessBuffer  ->data = Utility::String::ToFloat(r->ReadLine());

	SetDiffuseMap (r->ReadLine());
	SetSpecularMap(r->ReadLine());
	SetNormalMap  (r->ReadLine());

	SAFE_DELETE(r);

	this->path = path;
}

// Material InnerClass -> MaterialBuffer (UniformCollections 유니폼버퍼 모음집)
Material::MaterialBuffer::MaterialBuffer(const uint& targetProgramID) {
	diffuseBuffer  = new Vector4Buffer(targetProgramID, "diffuse");
	specularBuffer = new Vector4Buffer(targetProgramID, "specular");
	emissiveBuffer = new Vector4Buffer(targetProgramID, "emissive");
	shininessBuffer = new FloatBuffer(targetProgramID, "shininess");
	hasDiffuseBuffer  = new IntBuffer(targetProgramID, "hasDiffuse");
	hasSpecularBuffer = new IntBuffer(targetProgramID, "hasSpecular");
	hasNormalBuffer   = new IntBuffer(targetProgramID, "hasNormal");

	diffuseBuffer ->data = { 1.0f, 1.0f, 1.0f, 1.0f };
	specularBuffer->data = { 1.0f, 1.0f, 1.0f, 1.0f };
	emissiveBuffer->data = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininessBuffer->data = 24.0f;
	hasDiffuseBuffer ->data = false;
	hasSpecularBuffer->data = false;
	hasNormalBuffer  ->data = false;
}
Material::MaterialBuffer::~MaterialBuffer() {
	SAFE_DELETE(diffuseBuffer);
	SAFE_DELETE(specularBuffer);
	SAFE_DELETE(emissiveBuffer);
	SAFE_DELETE(shininessBuffer);
	SAFE_DELETE(hasDiffuseBuffer);
	SAFE_DELETE(hasSpecularBuffer);
	SAFE_DELETE(hasNormalBuffer);
}
void Material::MaterialBuffer::Set() {
	diffuseBuffer  ->Set();
	specularBuffer ->Set();
	emissiveBuffer ->Set();
	shininessBuffer->Set();
	hasDiffuseBuffer ->Set();
	hasSpecularBuffer->Set();
	hasNormalBuffer  ->Set();
}
void Material::MaterialBuffer::Update(const uint& targetProgramID) {
	diffuseBuffer  ->UpdatePosition(targetProgramID, "diffuse");
	specularBuffer ->UpdatePosition(targetProgramID, "specular");
	emissiveBuffer ->UpdatePosition(targetProgramID, "emissive");
	shininessBuffer->UpdatePosition(targetProgramID, "shininess");
	hasDiffuseBuffer ->UpdatePosition(targetProgramID, "hasDiffuse");
	hasSpecularBuffer->UpdatePosition(targetProgramID, "hasSpecular");
	hasNormalBuffer  ->UpdatePosition(targetProgramID, "hasNormal");
}
