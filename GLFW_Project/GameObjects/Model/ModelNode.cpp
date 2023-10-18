#include "../Framework.h"

ModelNode::ModelParts::ModelParts(MeshData*& mesh_data, const uint& ProgramID) {
	tag = mesh_data->name;
	mesh = new ModelMesh(mesh_data->vertices, mesh_data->indices);
	UpdateProgram(ProgramID);
}
ModelNode::ModelParts::~ModelParts() {
	SAFE_DELETE(mesh);
	SAFE_DELETE(wBuffer);
}
void ModelNode::ModelParts::Render() {
	wBuffer->data = world;
	wBuffer->Set();
	mesh->Set();
	glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
}
void ModelNode::ModelParts::UpdateProgram(const uint& ProgramID) {
	SAFE_DELETE(wBuffer);
	wBuffer = new MatrixBuffer(ProgramID, "world");
}

// ModelNode
void ModelNode::ClearModelMap() {
	for (auto& elem : render_map)
		for (ModelParts*& parts : elem.second) { SAFE_T_DELETE(parts); }
}
void ModelNode::LoadModel(string model_name) {
	ClearModelMap();
	ModelReader* reader = new ModelReader();
	data = reader->ReadModel(model_name);
	map<string, ModelParts*> parts_map;
	for (MeshData*& elem : data->meshes) { // ModelParts 생성 및 render_map 구성
		Material*& material = data->materials[elem->meterialIndex];
		ModelParts* parts = new ModelParts(elem, commonProgramID);
		render_map[material].emplace_back(parts);
		parts_map[elem->name] = parts;
	}

	uint n = data->nodes.size();
	vector<Matrix> nodeTransforms;
	nodeTransforms.resize(n);
	for (uint i = 0; i < n; i++) { // 노드대로 transform 설정 및 parents 구성
		NodeData*& node = data->nodes[i];
		if (node->index < 0) SetLocal(node->transform); // 루트노드의 경우
		else {
			ModelParts*& parts = parts_map[node->name];
			if (parts == nullptr) cout << "node : " << node->name << " was nullptr" << endl;
			else {
				Matrix parentMatrix = Matrix(1);
				if (node->parent < 0) parts->SetParent(this);
				else {
					ModelParts*& parent_parts = parts_map[data->nodes[node->parent]->name];
					parts->SetParent(parent_parts);
					parentMatrix = nodeTransforms[node->parent];
				}
				nodeTransforms[i] = parentMatrix * node->transform;
				parts->SetLocal(nodeTransforms[i]);
			}
		}
	}
	nodeTransforms.clear();
	parts_map.clear();

	SAFE_DELETE(reader);
}

ModelNode::ModelNode(string model_name) {
	LoadModel(model_name);
	SetShaders(
		"Assets/GLSL/Universal/Universal.vert", 
		"Assets/GLSL/Universal/Universal.frag");
}
ModelNode::~ModelNode() {
	SAFE_DELETE(wBuffer);
	SAFE_DELETE(data);
	ClearModelMap();
}

void ModelNode::UpdateWorld() {
	Transform::UpdateWorld();
	for (auto& elem : render_map)
		for (ModelParts*& parts : elem.second) parts->UpdateWorld();
}

void ModelNode::Render() {
	if (!Active()) return;
	wBuffer->data = world;
	for (auto elem : render_map) {
		elem.first->Set();
		for (ModelParts*& parts : elem.second) parts->Render();
	}
}
void ModelNode::GUIRender() {
	if (ImGui::TreeNode(tag.c_str())) {
		ImGui::Text(("model_name : " + data->model_name).c_str());
		char name_buf[128] = "";
		if (ImGui::InputTextWithHint("Tag", tag.c_str(), name_buf, IM_ARRAYSIZE(name_buf)))
			if (KEY_DOWN(ImGuiKey_Enter)) tag = string(name_buf);
		Transform::GUIRender();
		for (auto& elem : render_map) {
			elem.first->GUIRender();
			for (ModelParts*& parts : elem.second)
				parts->GUIRender();
		}
		//- 각 쉐이더 mesh(정확히는 node transforms) 저장?
		ImGui::TreePop();
	}
}

void ModelNode::SetShaders(string vshaderpath, string fshaderpath) {
	for (auto& elem : render_map) {
		elem.first->SetShader(vshaderpath, fshaderpath);
		for (ModelParts*& parts : elem.second)
			parts->UpdateProgram(elem.first->Get_ProgramID());
	}
	commonProgramID = data->materials[0]->Get_ProgramID();

	SAFE_DELETE(wBuffer);
	wBuffer = new MatrixBuffer(commonProgramID, "world");
}
