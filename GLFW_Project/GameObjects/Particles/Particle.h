#pragma once

class Particle : public Transform {
protected:
	const uint MAX_COUNT = 4096;
	ShaderProgram* particleProgram = nullptr;

	WorldBuffer* wBuffer = nullptr;
	VertexBuffer* vBuffer = nullptr;

	uint particleCount = 0;

	virtual void SetUniform();

	Particle(); // 불완전한 프로그램이기에 따로 생성 불가
	~Particle();
public:
	virtual void Update() = 0; // 순수가상함수또한 포함
	virtual void Render();
	virtual void GUIRender() = 0;

	virtual void Play(Vector3 pos = Vector3(0, 0, 0));
	void Stop();
};