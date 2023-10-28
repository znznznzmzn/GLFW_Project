#include "../Framework.h"

void ModelBoneInstance::SetBones() {
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

void ModelBoneInstance::InitUniformBuffer() {
	SAFE_DELETE(boneBuffer);
	boneBuffer = new ModelBone::BoneBuffer(data->materials[0]->Get_ProgramID());
}
void ModelBoneInstance::SetUniformBuffer() { boneBuffer->Set(); }

ModelBoneInstance::ModelBoneInstance(string model_name, uint instanceCount) {
	instance_count = instanceCount;
	LoadModel(model_name);
	SetShaders(
		"Assets/GLSL/Universal/ModelBoneInstance.vert",
		"Assets/GLSL/Universal/Universal.frag");
	SetBones();
	AttachInstanceMatrix();
}

ModelBoneInstance::~ModelBoneInstance() { SAFE_DELETE(boneBuffer); }
