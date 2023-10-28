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
	if (targetWorld == nullptr) return false; // targetWorld가 nullptr이면 not_active 취급
	if (parentTransform == nullptr) return is_active; // 부모가 없으면 그대로 반환
	if (!is_active) return false; // 부모가 Active인지 검사하기 전에 자신이 is_active가 아닌지를 먼저 검사
	return parentTransform->Active(); // 부모가 Active인지 검사
}
