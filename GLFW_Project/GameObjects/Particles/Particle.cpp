#include "../Framework.h"

void Particle::SetUniform() {
	wBuffer->world = world;
	wBuffer->Set();
	vBuffer->Set();
}

Particle::Particle() { particleCount = MAX_COUNT; }
Particle::~Particle() { 
	SAFE_DELETE(wBuffer);
	SAFE_DELETE(vBuffer);
}

void Particle::Render() {
	if (!Active()) return;
	particleProgram->Use();
	SetUniform();
	glDrawArrays(GL_POINTS, 0, particleCount);
}

void Particle::Play(Vector3 pos) {
	SetActive(true);
	Position() = pos;
	UpdateWorld();
}
void Particle::Stop() { SetActive(false); }
