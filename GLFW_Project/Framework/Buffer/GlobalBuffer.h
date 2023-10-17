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
	bool Bind(const uint& targetProgramID) { // 필요한 쉐이더 프로그램별로 수행해 주어야 함
		uint block_idx = glGetUniformBlockIndex(targetProgramID, block_name.c_str());
		glUniformBlockBinding(targetProgramID, block_idx, buffer_slot);
		//cout << "block_idx = \"" << block_name << "\" = " << block_idx << endl;
	}
	virtual void Set() = 0;
};

class ViewBuffer : public GlobalBuffer {
public:
	struct Data {
		Matrix View = Matrix(1.0f);
		Matrix InverseView = Matrix(1.0f);
	} data;
	ViewBuffer() : GlobalBuffer("ViewBuffer", 0) {
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
class ProjectionBuffer : public GlobalBuffer {
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

class GlobalLightBuffer : public GlobalBuffer {
public:
	struct Data {
		Vector4 light_color     = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		Vector4 light_direction = Vector4(0.0f, -0.5f, 0.3f, 0.0f);
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