#include "../Framework.h"

Vector4 Collider::COLOR_NORMAL   = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
Vector4 Collider::COLOR_COLLIDE  = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
Vector4 Collider::COLOR_DISABLED = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

void Collider::CreateMesh() {
	mesh = new PositionMesh(vertices, indices);
}

Collider::Collider(TYPE collider_type) : collider_type(collider_type) {
	colliderShader = ShaderProgram::Create("Assets/GLSL/Collider.vert", "Assets/GLSL/Collider.frag");
	wBuffer = new MatrixBuffer(colliderShader->GetProgramID(), "world");
	collider_colorBuffer = new Vector4Buffer(colliderShader->GetProgramID(), "collider_color");
	collider_colorBuffer->data = COLOR_NORMAL;
}
Collider::~Collider() {
	SAFE_DELETE(mesh);
	SAFE_DELETE(wBuffer);
	SAFE_DELETE(collider_colorBuffer);
}

void Collider::SetActive(bool isActive) {
	Transform::SetActive(isActive);
	collider_colorBuffer->data = isActive ? COLOR_NORMAL : COLOR_DISABLED;
}

void Collider::Render() {
	colliderShader->Use();
	wBuffer->data = world;
	wBuffer->Set();
	collider_colorBuffer->Set(); 
	mesh->Set();
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, (void*)0);
}

bool Collider::Collision(Collider* collider) {
	if (!Active() || !collider->Active()) return false;

	UpdateWorld();
	collider->UpdateWorld();

	switch (collider->collider_type) {
	case Collider::TYPE::POINT:   return collider->Position() == Position();
	case Collider::TYPE::BOX:     return BoxCollision    ((BoxCollider*    )collider);
	case Collider::TYPE::SPHERE:  return SphereCollision ((SphereCollider* )collider);
	case Collider::TYPE::CAPSULE: return CapsuleCollision((CapsuleCollider*)collider);
	default: break;
	}

	return false;
}
