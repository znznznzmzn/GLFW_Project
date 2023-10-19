#pragma once

class UniformBuffer {
protected:
	uint target_position = 0;
	UniformBuffer(const uint& targetProgramID, const string& name) { UpdatePosition(targetProgramID, name); }
	UniformBuffer() = default;
	~UniformBuffer() = default;
	virtual void Set() = 0;
public:
	void UpdatePosition(const uint& targetProgramID, const string& name) {
		target_position = glGetUniformLocation(targetProgramID, name.c_str());
		//cout << "target_position Updated to \"" << name << "\" = " << target_position << endl;
	}
};

class IntBuffer : public UniformBuffer {
public:
	int data;
	IntBuffer(const uint& targetProgramID, const string& int_name)
		: UniformBuffer(targetProgramID, int_name) { data = 0; }
	void Set() override 
	{ glUniform1i(target_position, data); }
};
class FloatBuffer : public UniformBuffer {
public:
	float data;
	FloatBuffer(const uint& targetProgramID, const string& float_name)
		: UniformBuffer(targetProgramID, float_name) { data = 0.0f; }
	void Set() override 
	{ glUniform1f(target_position, data); }
};
class Vector3Buffer : public UniformBuffer {
public:
	Vector3 data;
	Vector3Buffer(const uint& targetProgramID, const string& vector3_name)
		: UniformBuffer(targetProgramID, vector3_name)
	{ data = Vector3(1.0f, 1.0f, 1.0f); }
	void Set() override 
	{ glUniform3fv(target_position, 1, &data[0]); }
};
class Vector4Buffer : public UniformBuffer {
public:
	Vector4 data;
	Vector4Buffer(const uint& targetProgramID, const string& vector4_name)
		: UniformBuffer(targetProgramID, vector4_name)
	{ data = Vector4(1.0f, 1.0f, 1.0f, 1.0f); }
	void Set() override 
	{ glUniform4fv(target_position, 1, &data[0]); }
};
typedef Vector4Buffer ColorBuffer;
class MatrixBuffer : public UniformBuffer {
public:
	Matrix data;
	MatrixBuffer(const uint& targetProgramID, const string& matrix_name)
		: UniformBuffer(targetProgramID, matrix_name) 
	{ data = Matrix(1.0f); }
	void Set() override 
	{ glUniformMatrix4fv(target_position, 1, GL_FALSE, &data[0][0]); }
};

class WorldBuffer : protected UniformBuffer { // Matrix 버퍼 고정형 "world"
public:
	Matrix world;
	WorldBuffer(const uint& targetProgramID): UniformBuffer(targetProgramID, "world")
	{ world = Matrix(1.0f); }
	void Set() override 
	{ glUniformMatrix4fv(target_position, 1, GL_FALSE, &world[0][0]); }

	void UpdatePosition(const uint& targetProgramID) {
		target_position = glGetUniformLocation(targetProgramID, "world");
		//cout << "target_position Updated to \"" << name << "\" = " << target_position << endl;
	}
};

class TextureBuffer : public UniformBuffer {
public:
	int slot = 0;
	TextureBuffer(const uint& targetProgramID, const string& texture_name)
		: UniformBuffer(targetProgramID, texture_name) {  }
	void Set() override { glUniform1i(target_position, slot); }
	void Set(int slot) {
		this->slot = slot;
		Set();
	}
};