#pragma once

class ModelAnimatorInstance : public ModelInstance {
protected:
	FrameInstanceBuffer* frameInstanceBuffer = nullptr;

	bool isPlay = false;
	vector<ModelClip*> clips;
	vector<ClipTransform*> clipTransforms;
	vector<ClipTransform*> nodeTransforms;

	ModelAnimator::VirtualTexture *transformMapBuffer = nullptr;

	uint index_buffer_id = -1;
	vector<uint> indices;

	void CreateClipTexture();
	void CreateClipTransform(uint index);

	void EndEvent(void* obj);

	void InitUniformBuffer() override;
	void SetUniformBuffer() override;
	void SetMeshInstance () override;

	void AttachInstanceIndex();
public:
	ModelAnimatorInstance(string model_name, uint instanceCount);
	~ModelAnimatorInstance();

	void UpdateFrames();
	void UpdateFrame(uint instance);

	virtual void Render() override;
	virtual void GUIRender_Instance(uint index) override;

	void ReadClip(string clipName, uint clipNum = 0);
	void PlayClips(uint clip, float speed = 1.0f, float takeTime = 0.2f);
	void PlayClip(uint instance, uint clip, float speed = 1.0f, float takeTime = 0.2f);
	void Pause(const bool& isPause) { isPlay = !isPause; }

	ModelClip* GetClip(uint idx) { return clips[idx]; }
	Matrix GetTransformByNode(int nodeIndex, int instanceIndex);
};