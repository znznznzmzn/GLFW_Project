#pragma once

class Particle : public Transform {
protected:
	const uint MAX_COUNT = 10000;
	uint particleCount;
public:
	Particle();
	virtual ~Particle();
	virtual void Update() = 0;
	virtual void Render();
	virtual void GUIRender() = 0;

	virtual void Play(Vector3 pos);
	void Stop();
};