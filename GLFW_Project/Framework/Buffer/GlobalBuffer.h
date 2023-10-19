#pragma once

class GlobalBuffer {
protected:
	string block_name;
	uint buffer_id = 0;
	uint buffer_slot = 0;
	GlobalBuffer(const string& block_name, uint buffer_slot) {
		this->block_name = block_name;
		this->buffer_slot = buffer_slot; // GlobalBuffer 종류별로 다른 슬롯 필요
		glGenBuffers(1, &buffer_id);
	}
	~GlobalBuffer() = default;
public:
	void Bind(const uint& targetProgramID) { // 필요한 쉐이더 프로그램별로 수행해 주어야 함
		uint block_idx = glGetUniformBlockIndex(targetProgramID, block_name.c_str());
		glUniformBlockBinding(targetProgramID, block_idx, buffer_slot);
		//cout << "block_idx = \"" << block_name << "\" = " << block_idx << endl;
	}
	virtual void Set() = 0;
};

// 글로벌 버퍼는 여기서 관리하도록 
// 잘못하다간 꼬인다
// 꼬여서 8시간 날렸다...

class ViewBuffer : public GlobalBuffer { // View 0
public:
	struct Data {
		Matrix View = Matrix(1.0f);
		Matrix InverseView = Matrix(1.0f);
	} data;
	ViewBuffer() : GlobalBuffer("ViewBuffer", 0) { // 각 버퍼 슬롯은 달라야 한다
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
class ProjectionBuffer : public GlobalBuffer { // Projection 1
public:
	struct Data { Matrix Projection = Matrix(1.0f); } data; 
	ProjectionBuffer() : GlobalBuffer("ProjectionBuffer", 1) {
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

// 글로벌 빛
class GlobalLightBuffer : public GlobalBuffer { // GlobalLight 2
public:
	struct Data {
		Vector4 light_color     = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		Vector4 light_direction = Vector4(0.3f, -0.5f, 0.3f, 0.0f);
		Vector4 ambient_light   = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
		Vector4 ambient_ceil    = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
	} data;
	GlobalLightBuffer() : GlobalBuffer("GlobalLightBuffer", 2) {
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
	MaterialBuffer() : GlobalBuffer("MaterialBuffer", 3) {
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

class FrameBuffer : public GlobalBuffer { // ModelAnimator Animation 프레임 4
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

class SkyColorBuffer : public GlobalBuffer { // SkyColor 버퍼 5
public:
	struct Data {
		Vector3 centerColor = Vector3(0.3f, 0.3f, 0.3f);
		float height = 50.0f;
		Vector3 apexColor = Vector3(0.0f, 0.4f, 0.9f);
		float padding = 0.0f;
	} data;
	SkyColorBuffer() : GlobalBuffer("SkyColor", 5) {
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
}; // ; 안쓴거 실화냐?... 
// 이 쌍노메걸로 5시간 잡아먹힌거 실화냐?...

class WeatherBuffer : public GlobalBuffer { // WeatherBuffer 버퍼 6
public: // Rain, Snow버퍼의 인수가 동일하기에 초기화만 다르게 세팅하고 동일하게 사용
	struct Data {
		Vector3 velocity;
		float drawDistance;
		Vector4 particleColor;
		Vector3 origin;
		float time;
		Vector3 size;
		float turbulence;
	} data;
	WeatherBuffer(const string& name) : GlobalBuffer(name, 6) {
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