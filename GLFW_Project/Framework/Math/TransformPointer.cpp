#include "../Framework.h"

void TransformPointer::UpdateWorld() {
	if (!Active()) return;
	local.Rotation = Calc::RepeatPI(local.Rotation);
	local.GetMatrix(*targetWorld, pivot);
	if (parent != nullptr) (*targetWorld) = (*parent) * (*targetWorld);
	global.Decompose(*targetWorld);
}

Vector3 TransformPointer::Forward() { return glm::normalize(*targetWorld * Vector4(0, 0, 1, 0)); }
Vector3 TransformPointer::Up     () { return glm::normalize(*targetWorld * Vector4(0, 1, 0, 0)); }
Vector3 TransformPointer::Right  () { return glm::normalize(*targetWorld * Vector4(1, 0, 0, 0)); }

bool TransformPointer::Active() {
	if (targetWorld == nullptr) return false; // targetWorld�� nullptr�̸� not_active ���
	if (parentTransform == nullptr) return is_active; // �θ� ������ �״�� ��ȯ
	if (!is_active) return false; // �θ� Active���� �˻��ϱ� ���� �ڽ��� is_active�� �ƴ����� ���� �˻�
	return parentTransform->Active(); // �θ� Active���� �˻�
}
