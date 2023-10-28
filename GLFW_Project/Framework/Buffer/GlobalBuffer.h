#pragma once

class GlobalBuffer {
protected:
	string block_name;
	uint buffer_id = 0;
	uint buffer_slot = 0;
	GlobalBuffer(const string& block_name, uint buffer_slot) {
		this->block_name = block_name;
		this->buffer_slot = buffer_slot; // GlobalBuffer �������� �ٸ� ���� �ʿ�
		glGenBuffers(1, &buffer_id);
	}
	~GlobalBuffer() = default;
public:
	void Bind(const uint& targetProgramID) { // �ʿ��� ���̴� ���α׷����� ������ �־�� ��
		uint block_idx = glGetUniformBlockIndex(targetProgramID, block_name.c_str());
		glUniformBlockBinding(targetProgramID, block_idx, buffer_slot);
		//cout << "block_idx = \"" << block_name << "\" = " << block_idx << endl;
	}
	virtual void Set() = 0;
};

/*
GL_STATIC_DRAW
	���� �����͸� CPU���� GPU�� �� ���� �����Ͽ� ���� ���, ���� ��ü�� ��쿡 ���

GL_DYNAMIC_DRAW
	���� �����Ͱ� �߰� ������ �󵵷� ������Ʈ�� ������ ����, �÷��̾� ĳ������ ��ġ �� ����

GL_STREAM_DRAW
	���� �����Ͱ� �ſ� ���� ������Ʈ�� ������ ����, ȭ���� �ȼ� ������
*/

// �۷ι� ���۴� ���⼭ �����ϵ��� 
// �߸��ϴٰ� ���δ�
// ������ 8�ð� ���ȴ�...

class ViewBuffer : public GlobalBuffer { // View 0
public:
	struct Data {
		Matrix View = Matrix(1.0f);
		Matrix InverseView = Matrix(1.0f);
	} data;
	ViewBuffer() : GlobalBuffer("ViewBuffer", 0) { // �� ���� ������ �޶�� �Ѵ�
		glBindBuffer(GL_UNIFORM_BUFFER, buffer_id); 
		glBufferData(GL_UNIFORM_BUFFER, sizeof(data), NULL, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, buffer_slot, buffer_id, 0, sizeof(data));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void Set() override {
		glBindBuffer(GL_UNIFORM_BUFFER, buffer_id); 
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data), &data); 
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
};
class ProjectionBuffer : public GlobalBuffer { // Projection 1
public:
	struct Data { Matrix Projection = Matrix(1.0f); } data; 
	ProjectionBuffer() : GlobalBuffer("ProjectionBuffer", 1) {
		glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(data), NULL, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, buffer_slot, buffer_id, 0, sizeof(data));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void Set() override {
		glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data), &data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
};

// �۷ι� ��
class GlobalLightBuffer : public GlobalBuffer { // GlobalLight 2
public:
	struct Data {
		Vector4 light_color     = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		Vector4 light_direction = Vector4(0.3f, -0.5f, 0.3f, 0.0f);
		Vector4 ambient_light   = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
		Vector4 ambient_ceil    = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
	} data;
	GlobalLightBuffer() : GlobalBuffer("GlobalLightBuffer", 2) { //- Data�� ����ɶ��� Set��Ű��
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
};

class MaterialBuffer : public GlobalBuffer { // Universal Material 3
public:
	struct Data {
		Vector4 diffuse  = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector4 emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
		float shininess = 24.0f;
		int hasDiffuse  = false;
		int hasSpecular = false;
		int hasNormal   = false;
	} data;
	MaterialBuffer() : GlobalBuffer("MaterialBuffer", 3) {  //- Data�� ����ɶ��� Set��Ű��
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
};

class FrameBuffer : public GlobalBuffer { // ModelAnimator Animation ������ 4
public:
	struct Frame {
		int clip = 0;
		uint curFrame = 0;
		float time = 0.0f;
		float speed = 1.0f;
	};
	struct Data {
		float takeTime = 0.2f;
		float tweenTime = 0.0f;
		float runningTime = 0.0f;
		float padding = 0.0f;
		Frame cur;
		Frame next;
	} data;
	FrameBuffer() : GlobalBuffer("FrameBuffer", 4) {
		glBindBuffer(GL_UNIFORM_BUFFER, buffer_id); // time ���� ��� �����ϹǷ� GL_STREAM_DRAW
		glBufferData(GL_UNIFORM_BUFFER, sizeof(data), NULL, GL_STREAM_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, buffer_slot, buffer_id, 0, sizeof(data));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void Set() override {
		glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data), &data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
};

class SkyColorBuffer : public GlobalBuffer { // SkyColor ���� 5
public:
	struct Data {
		Vector3 centerColor = Vector3(0.3f, 0.3f, 0.3f);
		float height = 50.0f;
		Vector3 apexColor = Vector3(0.0f, 0.4f, 0.9f);
		float padding = 0.0f;
	} data;
	SkyColorBuffer() : GlobalBuffer("SkyColor", 5) { //- Data�� ����ɶ��� Set��Ű��
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
}; // ; �Ⱦ��� ��ȭ��?... 
// �� �ֳ�ްɷ� 5�ð� ��Ƹ����� ��ȭ��?...

class WeatherBuffer : public GlobalBuffer { // WeatherBuffer ���� 6
public: // Rain, Snow������ �μ��� �����ϱ⿡ �ʱ�ȭ�� �ٸ��� �����ϰ� �����ϰ� ���
	struct Data {
		Vector3 velocity = Vector3(0, 0, 0);
		float drawDistance = 0;
		Vector4 particleColor = Vector4(0, 0, 0, 0);
		Vector3 origin = Vector3(0, 0, 0);
		float time = 0;
		Vector3 size = Vector3(0, 0, 0);
		float turbulence = 0;
	} data;
	WeatherBuffer(const string& name) : GlobalBuffer(name, 6) { //- Data�� ����ɶ��� Set��Ű��
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
};