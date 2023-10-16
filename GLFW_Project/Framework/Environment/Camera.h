#pragma once

class Camera : public Transform {
private:
	ViewBuffer* vBuffer;
	ProjectionBuffer* pBuffer;

	void MakeProjection();

	void FreeMode();
public:
	Camera();
	~Camera();
	void UpdateWorld() override;
	void Update();

	enum class PROJECTION_TYPE : uint {
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	void SetView();
	void SetProjection();
	// ������������ pBuffer�� MakeProjection �ܿ��� 
	// �߰��� �����͸� ������Ʈ ��ų �ʿ䰡 ���⶧�� 
	void Bind(const uint& targetProgramID); // ī�޶� View, Porjection ���ε�

	Ray ScreenPointToRay(Vector2 screenPosition = mousePos);
	Vector2 WorldToScreenPoint(Vector3 worldPos);
	void LookAt(Vector3 target, Vector3 upVector = Vector3(0, 1, 0));
private:
	//- ���� projection ��� ���� 
	PROJECTION_TYPE projection_type = PROJECTION_TYPE::PERSPECTIVE;

	float fov = 60.0f;
	float width = SCREEN_WIDTH;
	float height = SCREEN_HEIGHT;
	float offset_x = 0.0f;
	float offset_y = 0.0f;
	float near = 0.01f;
	float far = 1000.0f;

	float aspect() { return width / height; }
};