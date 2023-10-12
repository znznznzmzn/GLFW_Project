#include "../Framework.h"

void ModelBone::SetBones() {
	uint n = data->nodes.size();
	vector<Matrix> nodeTransforms;
	nodeTransforms.resize(n);
	for (uint i = 0; i < n; i++) {
		NodeData*& node = data->nodes[i];
		Matrix parent = (node->parent < 0) ? Matrix(1) : nodeTransforms[node->parent];
		nodeTransforms[i] = parent * node->transform;
		int boneIndex = (data->boneMap.count(node->name) == 0) ? -1 : data->boneMap[node->name];
		if (boneIndex >= 0)
			boneBuffer->SetBone(nodeTransforms[i] * data->bones[boneIndex]->offset, boneIndex);
	}
}

void ModelBone::InitUniformBuffer() {
	Model::InitUniformBuffer();
	SAFE_DELETE(boneBuffer);
	boneBuffer = new BoneBuffer(commonProgramID);
}
void ModelBone::SetUniformBuffer() { // Shader가 설정된 후의 지점
	Model::SetUniformBuffer(); // 기본 WorldBffer Set
	boneBuffer->Set();
}

ModelBone::ModelBone(string model_name) {
	LoadModel(model_name);
	SetShaders("Assets/GLSL/ModelBone.vert", "Assets/GLSL/Universal.frag");
	LockShader();
	SetBones();
}
ModelBone::~ModelBone() { SAFE_DELETE(boneBuffer); }

void ModelBone::GUIRender() {
	if (ImGui::TreeNode(tag.c_str())) {
		ImGui::Text(("model_name : " + data->model_name).c_str());
		char name_buf[128] = "";
		if (ImGui::InputTextWithHint("Tag", tag.c_str(), name_buf, IM_ARRAYSIZE(name_buf)))
			if (KEY_DOWN(ImGuiKey_Enter)) tag = string(name_buf);
		Transform::GUIRender();
		for (auto elem : render_map) elem.first->GUIRender();
		//- 가능하다면 BoneBuffer Data 조작?
		ImGui::TreePop();
	}
}
