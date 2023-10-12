#pragma once

class ModelBone : public Model {
private:
	class BoneBuffer : public UniformBuffer {
	public:
		Matrix bones[MAX_BONES];
		BoneBuffer(const uint& targetProgramID) : UniformBuffer(targetProgramID, "bones") 
		{ for (int i = 0; i < MAX_BONES; i++) bones[i] = Matrix(1); }
		void Set() override 
		{ glUniformMatrix4fv(target_position, MAX_BONES, GL_FALSE, glm::value_ptr(bones[0])); }
		void SetBone(Matrix matrix, uint index) { bones[index] = glm::transpose(matrix);  }
	}* boneBuffer = nullptr;
	void SetBones();
protected:
	virtual void InitUniformBuffer() override;
	virtual void SetUniformBuffer() override;
public:
	ModelBone(string model_name);
	~ModelBone();

	virtual void GUIRender() override;
};
