#include "../Framework.h"

Particle::Particle() {
	particleCount = MAX_COUNT;
}

Particle::~Particle() {
}

void Particle::Render() {
	if (!Active()) return;

}

void Particle::Play(Vector3 pos) {
	SetActive(true);
	Position() = pos;
	UpdateWorld();
}
void Particle::Stop() { SetActive(false); }
