#include "../Framework.h"

void Material::bind() {
	if (materialProgram == nullptr) {
		cout << "CAUTION >> Material Shader Program = nullptr" << endl;
		return;
	}
	materialProgram->Use();
	materialProgram->Bind(mBuffer);
	materialProgram->Bind(diffuseMapBuffer , "diffuseMap");
	materialProgram->Bind(specularMapBuffer, "specularMap");
	materialProgram->Bind(normalMapBuffer  , "normalMap");
}

void Material::init(const int& shader_id) {
	mBuffer = new MaterialBuffer();
	diffuseMapBuffer  = new TextureBuffer(shader_id, "diffuseMap" );
	specularMapBuffer = new TextureBuffer(shader_id, "specularMap");
	normalMapBuffer   = new TextureBuffer(shader_id, "normalMap"  );
	diffuseMapBuffer ->slot = 0;
	specularMapBuffer->slot = 1;
	normalMapBuffer  ->slot = 2;
}

Material::Material() { init(0); }
Material::Material(const string& vshader_path, const string& fshader_path) {
	materialProgram = ShaderProgram::Create(vshader_path, fshader_path);
	materialProgram->Use();
	init(materialProgram->GetProgramID());
	bind();
}
Material::~Material() { 
	SAFE_DELETE(mBuffer); 
	SAFE_DELETE(diffuseMapBuffer);
	SAFE_DELETE(specularMapBuffer);
	SAFE_DELETE(normalMapBuffer);
}

void Material::Set() {
	materialProgram->Use();
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
	materialProgram = ShaderProgram::Create(vshader_path, fshader_path); 
	bind();
}

void Material::SetDiffuseMap(string path) {
	mBuffer->data.hasDiffuse = (path.length() != 0);
	diffuseMap = (mBuffer->data.hasDiffuse) ? Texture::Load(path) : nullptr;
}
void Material::SetSpecularMap(string path) {
	mBuffer->data.hasSpecular = (path.length() != 0);
	specularMap = (mBuffer->data.hasSpecular) ? Texture::Load(path) : nullptr;
}
void Material::SetNormalMap(string path) {
	mBuffer->data.hasNormal = (path.length() != 0);
	normalMap = (mBuffer->data.hasNormal) ? Texture::Load(path) : nullptr;
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

		// Shader
		ImGui::Checkbox("ShaderLock", &is_shader_locked);
		if(!is_shader_locked)
			materialProgram->GUIRender();

		// Material Color
		ImGui::ColorEdit4 ("Diffuse" , (float*)&mBuffer->data.diffuse );
		ImGui::ColorEdit4 ("Specular", (float*)&mBuffer->data.specular);
		ImGui::ColorEdit4 ("Emissive", (float*)&mBuffer->data.emissive);
		ImGui::SliderFloat("Shiness" , (float*)&mBuffer->data.shininess, 1.0f, 50.0f);

		// Material has Diffuse Specular Normal 
		ImGui::Checkbox("Is Diffuse   ", (bool*)&mBuffer->data.diffuse);
		ImGui::SameLine();
		ImGui::Checkbox("Is Specular  ", (bool*)&mBuffer->data.specular);
		ImGui::SameLine();
		ImGui::Checkbox("Is Normal    ", (bool*)&mBuffer->data.hasNormal);

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

	{
		if (materialProgram != nullptr) { // shader
			w->WriteLine(materialProgram->GetProgramKey()); // shader_key
			w->WriteLine(Utility::String::From(materialProgram->GetAttachedShadersCount())); // shader_count
			for (Shader*& elem : materialProgram->GetAttachedShaders()) // shaders
				w->WriteLine(elem->GetShaderPath());
		}
		else {
			w->WriteLine(""); // shader_key = nullptr
			w->WriteLine(0); // shader_count = 0
		}
	}

	w->WriteLine(Utility::String::From(mBuffer->data.diffuse )); // material_buffer datas
	w->WriteLine(Utility::String::From(mBuffer->data.specular));
	w->WriteLine(Utility::String::From(mBuffer->data.emissive));

	w->WriteLine(Utility::String::From(mBuffer->data.hasDiffuse )); 
	w->WriteLine(Utility::String::From(mBuffer->data.hasSpecular)); 
	w->WriteLine(Utility::String::From(mBuffer->data.hasNormal  )); 
	w->WriteLine(Utility::String::From(mBuffer->data.shininess  ));

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

	name = r->ReadLine(); // material name

	{ // shader
		string shader_key = r->ReadLine(); // shader_key
		uint shader_count = Utility::String::ToUint(r->ReadLine()); // shader_count
		vector<string> shader_paths;
		for (uint i = 0; i < shader_count; i++) // shaders
			shader_paths.emplace_back(r->ReadLine());
		if (shader_count != 0) 
			materialProgram = ShaderProgram::Create(shader_paths);
	}

	mBuffer->data.diffuse  = Utility::String::ToVector4(r->ReadLine());
	mBuffer->data.specular = Utility::String::ToVector4(r->ReadLine());
	mBuffer->data.emissive = Utility::String::ToVector4(r->ReadLine());

	mBuffer->data.hasDiffuse  = Utility::String::ToInt  (r->ReadLine());
	mBuffer->data.hasSpecular = Utility::String::ToInt  (r->ReadLine());
	mBuffer->data.hasNormal   = Utility::String::ToInt  (r->ReadLine());
	mBuffer->data.shininess   = Utility::String::ToFloat(r->ReadLine());

	SetDiffuseMap (r->ReadLine());
	SetSpecularMap(r->ReadLine());
	SetNormalMap  (r->ReadLine());

	SAFE_DELETE(r);

	this->path = path;
}

