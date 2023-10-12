#include "../../Framework.h"

SphereCollider::SphereCollider() : Collider(TYPE::SPHERE) {
	uint Count = 30;
	float Step = Calc::PI2 / Count;
	//Vertices
	for (uint i = 0; i <= Count; i++) { // x�� ȸ��
		float x = i * Step;
		vertices.push_back(Vector3(0, cos(x), sin(x))); 
	}
	for (uint i = 0; i <= Count; i++) { // y�� ȸ��
		float y = i * Step;
		vertices.push_back(Vector3(cos(y), 0, sin(y))); 
	}
	for (uint i = 0; i <= Count; i++) { // z�� ȸ��
		float z = i * Step;
		vertices.push_back(Vector3(sin(z), cos(z), 0));
	}
	//Indices
	for (uint i = 0; i < 3; i++) { // ������
		for (uint j = 0; j < Count; j++) { // ȸ���� ��ŭ
			indices.push_back(i + i * Count + j); // 0 
			indices.push_back(i + i * Count + j + 1); // 1
		}
	}
	CreateMesh();
}

bool SphereCollider::RayCollision(Ray ray, Contact* contact) {
	UpdateWorld();
	Vector3 P = ray.position;
	Vector3 D = ray.direction;
	Vector3 A = P - GlobalPosition();
	float b = glm::dot(D, A);
	float c = glm::dot(A, A) - Radius() * Radius();
	if (b * b >= c) {
		if (contact != nullptr) {
			float t = -b - sqrt(b * b - c);
			contact->distance = t;
			contact->hitPoint = P + D * t;
		}
		return true;
	}
	return false;
}
bool SphereCollider::BoxCollision(BoxCollider* collider) { return collider->SphereCollision(this); }
bool SphereCollider::SphereCollision(SphereCollider* collider) 
{ return glm::distance(GlobalPosition(), collider->GlobalPosition()) <= this->Radius() + collider->Radius(); }
bool SphereCollider::CapsuleCollision(CapsuleCollider* collider) { return collider->SphereCollision(this); }
