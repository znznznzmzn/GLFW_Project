#pragma once

class Particle : public Transform {
protected:
	const uint MAX_COUNT = 10000;
	ShaderProgram* particleProgram = nullptr;
	WorldBuffer* wBuffer = nullptr;
	uint particleCount = 1000;
	bool isDepthMask = true;
	// bool isBlending = true; 블렌드는 Billboard를 작업한 후에 생각하자
public:
	Particle();
	virtual ~Particle();
	virtual void Update() = 0;
	virtual void Render();
	virtual void GUIRender() = 0;

	virtual void Play(Vector3 pos);
	void Stop();
};