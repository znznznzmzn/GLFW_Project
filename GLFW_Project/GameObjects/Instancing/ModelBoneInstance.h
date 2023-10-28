#pragma once

class ModelBoneInstance : public ModelInstance {
protected:
	ModelBone::BoneBuffer* boneBuffer = nullptr;
	void SetBones();
	virtual void InitUniformBuffer() override;
	virtual void SetUniformBuffer() override;
public:
	ModelBoneInstance(string model_name, uint instanceCount = 100);
	~ModelBoneInstance();
};