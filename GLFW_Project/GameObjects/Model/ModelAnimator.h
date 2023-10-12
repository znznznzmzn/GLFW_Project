#pragma once

class ModelAnimator : public Model {
protected:
	struct Frame {
		int clip = 0;
		uint curFrame = 0;
		float time = 0.0f;
		float speed = 1.0f;
	};
	class FrameBuffer : public GlobalBuffer {
	public:
		struct Data {
			float takeTime = 0.2f;
			float tweenTime = 0.0f;
			float runningTime = 0.0f;
			float padding = 0.0f;
			Frame cur;
			Frame next;
		} data;
		FrameBuffer() : GlobalBuffer("FrameBuffer", 3) {
			glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(data), NULL, GL_STATIC_DRAW);
			glBindBufferRange(GL_UNIFORM_BUFFER, buffer_slot, buffer_id, 0, sizeof(data));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
		void Set() override {
			glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data), &data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	}* fBuffer = nullptr;

	void CreateClipTexture();
	void CreateClipTransform(uint index);

	bool isPlay = false;
	vector<ModelClip*> clips;
	vector<ClipTransform*> clipTransforms;
	vector<ClipTransform*> nodeTransforms;

	class VirtualTexture : public TextureBuffer {
	public:
		uint texture_id = -1;
		uint count = 0;
		uint width = 0;
		uint height = 0;
		VirtualTexture(const uint& targetProgramID)
			: TextureBuffer(targetProgramID, "transformMap") { }
		~VirtualTexture() { glDeleteTextures(1, &texture_id); }
		void Set() override {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
			TextureBuffer::Set();
		}
	}* transformMapBuffer = nullptr;

	void SetUniformBuffer() override;
public:
	ModelAnimator(string model_name);
	~ModelAnimator();

	void UpdateFrame();
	virtual void Render() override;
	virtual void GUIRender() override;

	void ReadClip(string clipName, uint clipNum = 0);
	void PlayClip(uint clip, float speed = 1.0f, float takeTime = 0.2f);
	void Pause(const bool& isPause) { isPlay = !isPause; }

	ModelClip* GetClip(uint idx) { return clips[idx]; }
	Matrix GetTransformByNode(int nodeIndex);
};