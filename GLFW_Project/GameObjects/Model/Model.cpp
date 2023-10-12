#include "../Framework.h"

void Model::InitUniformBuffer() {
	SAFE_DELETE(wBuffer);
	wBuffer = new MatrixBuffer(commonProgramID, "world");
}
void Model::SetUniformBuffer() { wBuffer->Set(); }

void Model::ClearModelMap() {
	for (auto elem : render_map) {
		for (ModelMesh*& mesh : elem.second) { SAFE_DELETE(mesh); }
		elem.second.clear();
	}
	render_map.clear();
}
void Model::LoadModel(string model_name) {
	ClearModelMap();
	ModelReader* reader = new ModelReader();
	data = reader->ReadModel(model_name);
	for (MeshData*& elem : data->meshes) {
		ModelMesh* mesh = new ModelMesh(elem->vertices, elem->indices);
		render_map[data->materials[elem->meterialIndex]].emplace_back(mesh);
	}
	SAFE_DELETE(reader);
}

Model::Model(string model_name) {
	LoadModel(model_name);
	SetShaders("Assets/GLSL/Universal.vert", "Assets/GLSL/Universal.frag");
	LockShader();
}
Model::~Model() {
	ClearModelMap();
	SAFE_DELETE(wBuffer);
	SAFE_DELETE(data);
}

void Model::Render() {
	if (!Active()) return;
	wBuffer->data = world;
	for (auto elem : render_map) {
		elem.first->Set();
		SetUniformBuffer();
		for (ModelMesh*& mesh : elem.second) {
			mesh->Set();
			glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
		}
	}
}

void Model::GUIRender() {
	if (ImGui::TreeNode(tag.c_str())) {
		ImGui::Text(("model_name : " + data->model_name).c_str());
		char name_buf[128] = "";
		if (ImGui::InputTextWithHint("Tag", tag.c_str(), name_buf, IM_ARRAYSIZE(name_buf)))
			if (KEY_DOWN(ImGuiKey_Enter)) tag = string(name_buf);
		Transform::GUIRender();
		for (auto elem : render_map) elem.first->GUIRender();
		ImGui::TreePop();
	}
}

void Model::SetShaders(string vshaderpath, string fshaderpath) {
	if (is_shader_locked) return;
	// 전체 마테리얼의 쉐이더를 동일하게 적용
	for (Material*& elem : data->materials) 
		elem->SetShader(vshaderpath, fshaderpath);
	// 0번 마테리얼의 쉐이더를 commonProgramID 로 등록
	commonProgramID = data->materials[0]->Get_ProgramID();
	InitUniformBuffer();
}