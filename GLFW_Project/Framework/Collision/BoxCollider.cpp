#include "../Framework.h"

BoxCollider::BoxCollider(Vector3 size) : Collider(TYPE::BOX) {
	Tag() = "BoxCollider";
	this->size = size;
	Vector3 halfSize = size / 2.0f;
	vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z);
	vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z);
	vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z);
	vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z);

	vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z);
	vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z);
	vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z);
	vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z);

	indices = {
		0, 1, 1, 3, 3, 2, 2, 0,
		4, 5, 5, 7, 7, 6, 6, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};
	CreateMesh();
}

void BoxCollider::GetObb(ObbDesc& obbDesc) {
	obbDesc.position = GlobalPosition();
	obbDesc.axis[0] = Right();
	obbDesc.axis[1] = Up();
	obbDesc.axis[2] = Forward();
	obbDesc.halfSize = size * 0.5f * GlobalScale();
}

bool BoxCollider::RayCollision(Ray ray, Contact* contact) {
	UpdateWorld();
	GetObb(thisBox);
	float tMin = 0.0f;
	float tMax = FLT_MAX;
	Vector3 min = thisBox.halfSize * -1.0f;
	Vector3 max = thisBox.halfSize;
	Vector3 delta = thisBox.position - ray.position;
	Vector3 D = glm::normalize(ray.direction);
	for (uint i = 0; i < 3; i++) {
		Vector3 axis = thisBox.axis[i];
		float e = glm::dot(axis, delta);
		float f = glm::dot(D, axis);
		if (abs(f) < FLT_EPSILON) {
			if (min[i] > e || max[i] < e) return false;
		} else {
			float t1 = (e + min[i]) / f;
			float t2 = (e + max[i]) / f;
			if (t1 > t2) swap(t1, t2);
			if (t1 > tMin) tMin = t1;
			if (t2 < tMax) tMax = t2;
			if (tMin > tMax) return false;
		}
	}
	if (contact != nullptr) {
		contact->distance = tMin;
		contact->hitPoint = ray.position + ray.direction * tMin;
	}
	return true;
}

bool BoxCollider::IsSeperateAxis(Vector3 D, Vector3 axis, ObbDesc thisBox, ObbDesc targetBox) {
	float a = 0.0f;
	float b = 0.0f;
	for (uint i = 0; i < 3; i++) {
		a += abs(glm::dot(thisBox.axis[i] * thisBox.halfSize[i], axis));
		b += abs(glm::dot(targetBox.axis[i] * targetBox.halfSize[i], axis));
	}
	return abs(glm::dot(D, axis)) > a + b;
}
bool BoxCollider::BoxCollision(BoxCollider* collider) {
	GetObb(thisBox);
	collider->GetObb(targetBox);
	Vector3 D = targetBox.position - thisBox.position;
	if (IsSeperateAxis(D, targetBox.axis[0], thisBox, targetBox) || 
		IsSeperateAxis(D, targetBox.axis[0], thisBox, targetBox)) return false;
	if (IsSeperateAxis(D, targetBox.axis[1], thisBox, targetBox) ||
		IsSeperateAxis(D, targetBox.axis[1], thisBox, targetBox)) return false;
	if (IsSeperateAxis(D, targetBox.axis[2], thisBox, targetBox) ||
		IsSeperateAxis(D, targetBox.axis[2], thisBox, targetBox)) return false;
	for (uint i = 0; i < 3; i++) {
		for (uint j = 0; j < 3; j++) {
			if (thisBox.axis[i] == targetBox.axis[j]) return true;
			if (IsSeperateAxis(D, 
				glm::cross(thisBox.axis[i], targetBox.axis[j]), 
				thisBox, targetBox)) 
				return false;
		}
	}

	return true;
}
bool BoxCollider::SphereCollision(SphereCollider* collider) {
	GetObb(thisBox);

	Vector3 pos = thisBox.position;

	for (uint i = 0; i < 3; i++) {
		float length = glm::dot(thisBox.axis[i], collider->GlobalPosition() - thisBox.position);
		float mult = (length < 0.0f) ? -1.0f : 1.0f;
		length = min(abs(length), thisBox.halfSize[i]);
		pos += thisBox.axis[i] * length * mult;
	}
	return glm::distance(collider->GlobalPosition(), pos) <= collider->Radius();
}
bool BoxCollider::CapsuleCollision(CapsuleCollider* collider) {
	GetObb(thisBox);
	Vector3 tmpBoxPos = thisBox.position;
	Vector3 CapsuleUpVector = collider->Up();
	Vector3 SphereA = collider->GlobalPosition() - CapsuleUpVector * collider->Height() / 2.0f;
	Vector3 SphereB = collider->GlobalPosition() + CapsuleUpVector * collider->Height() / 2.0f;
	Vector3 pointOnLine = Calc::ClosestPointOnLine(SphereA, SphereB, tmpBoxPos);
	for (uint i = 0; i < 3; i++) {
		float length = glm::dot(thisBox.axis[i], pointOnLine - thisBox.position);
		float mult = (length < 0.0f) ? -1.0f : 1.0f;
		length = min(abs(length), thisBox.halfSize[i]);
		tmpBoxPos += thisBox.axis[i] * length * mult;
	}
	return glm::distance(pointOnLine, tmpBoxPos) <= collider->Radius();
}
