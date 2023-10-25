#include "../Framework.h"

void Camera::FreeMode() {
	if (MOUSE_PRESS(1)) { // 우클릭
		// 마우스 회전
		Rotation().x -= ImGui::GetIO().MouseDelta.y * 3.0f * delta;
		Rotation().y -= ImGui::GetIO().MouseDelta.x * 3.0f * delta;
		Rotation().x = Calc::Clamp(Rotation().x, -Calc::PI / 2.0f, Calc::PI / 2.0f);
	
		// 키보드 이동
		Vector3 direction = Vector3(0.0f, 0.0f, 0.0f);
		float move_speed = (KEY_PRESS(ImGuiKey_LeftShift)) ? 10.0f : 5.0f;
		if (KEY_PRESS(ImGuiKey_W)) direction -= Forward();
		if (KEY_PRESS(ImGuiKey_S)) direction += Forward();
		if (KEY_PRESS(ImGuiKey_A)) direction -= Right();
		if (KEY_PRESS(ImGuiKey_D)) direction += Right();
		if (KEY_PRESS(ImGuiKey_Q)) direction -= Up();
		if (KEY_PRESS(ImGuiKey_E)) direction += Up();
		if (direction != Vector3(0.0f, 0.0f, 0.0f))
			Position() += glm::normalize(direction) * move_speed * delta;
	}
}
void Camera::UpdateWorld() {
	Transform::UpdateWorld();
	vBuffer->data.InverseView = world;
	vBuffer->data.View = glm::inverse(world);
}

void Camera::Update() {
	if (!Active()) return;
	FreeMode();
	UpdateWorld(); 
}

void Camera::MakeProjection() { 
	switch (projection_type) {
	case Camera::PROJECTION_TYPE::PERSPECTIVE:
		pBuffer->data.Projection = glm::perspective(glm::radians(Fov), aspect(), Near, Far);
		break;
	case Camera::PROJECTION_TYPE::ORTHOGRAPHIC:
		pBuffer->data.Projection = glm::ortho(pivot.x, Width, pivot.y, Height, Near, Far);
		break;
	default:
		cout << "Warning on Camera, Wrong projection_type = " << (uint)projection_type << endl;
		break;
	}
	SetProjection();
}

void Camera::SetView() { vBuffer->Set(); }
void Camera::SetProjection() { pBuffer->Set(); }

void Camera::Bind(const uint& targetProgramID) {
	vBuffer->Bind(targetProgramID);
	pBuffer->Bind(targetProgramID);
}

Ray Camera::ScreenPointToRay(Vector2 screenPosition) {
	Vector2 point = { // -1~1 스크린 좌표 -> NDC좌표
		(2.0f * (screenPosition.x / (float)SCREEN_WIDTH)) - 1.0f,
		1.0f - (2.0f * (screenPosition.y / (float)SCREEN_HEIGHT))
	};
	Vector4 ray_eye = glm::inverse(pBuffer->data.Projection) * Vector4(point, -1.0f, 1.0f);
	ray_eye = Vector4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

	return Ray{ Position(), glm::normalize(world * ray_eye) };
}

Vector2 Camera::WorldToScreenPoint(Vector3 worldPos) {
	Vector4 clipSpacePos = pBuffer->data.Projection * (vBuffer->data.View * Vector4(worldPos, 1));
	Vector3 ndcSpacePos = Vector3(clipSpacePos) / clipSpacePos.w;
	return Vector2(
		SCREEN_WIDTH  * ((ndcSpacePos.x + 1.0f) / 2.0f), // NDC좌표 = -1 ~ 1 >> 변환 >> 0 ~ SCREEN_WIDTH
		SCREEN_HEIGHT * ((ndcSpacePos.y + 1.0f) / 2.0f));
}

void Camera::LookAt(Vector3 target, Vector3 upVector) 
{ world = glm::lookAtLH(Position(), target, upVector); }

Camera::Camera() {
	vBuffer = new ViewBuffer();
	pBuffer = new ProjectionBuffer();
	MakeProjection();
}
Camera::~Camera() {
	SAFE_DELETE(vBuffer);
	SAFE_DELETE(pBuffer);
}
