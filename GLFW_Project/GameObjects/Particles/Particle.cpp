#include "../Framework.h"

Particle::Particle() {
	particleCount = MAX_COUNT;
	wBuffer = new WorldBuffer(0);
}

Particle::~Particle() { SAFE_DELETE(wBuffer); }

void Particle::Render() {
	if (!Active()) return;
	particleProgram->Use();
	glDepthMask(GL_FALSE);
	//-
	glDepthMask(GL_TRUE);
}

void Particle::Play(Vector3 pos) {
	SetActive(true);
	Position() = pos;
	UpdateWorld();
}
void Particle::Stop() { SetActive(false); }
