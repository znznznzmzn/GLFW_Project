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
	// 나눠둔이유는 pBuffer가 MakeProjection 외에는 
	// 추가로 데이터를 업데이트 시킬 필요가 없기때문 
	void Bind(const uint& targetProgramID); // 카메라 View, Porjection 바인딩

	Ray ScreenPointToRay(Vector2 screenPosition = mousePos);
	Vector2 WorldToScreenPoint(Vector3 worldPos);
	void LookAt(Vector3 target, Vector3 upVector = Vector3(0, 1, 0));


	ViewBuffer*& GetViewBuffer() { return vBuffer; }
	ProjectionBuffer*& GetProjectionBuffer() { return pBuffer; }

private:
	//- 추후 projection 모드 설정 
	PROJECTION_TYPE projection_type = PROJECTION_TYPE::PERSPECTIVE;

	float Fov = 60.0f;
	float Width = SCREEN_WIDTH;
	float Height = SCREEN_HEIGHT;
	// float offset_x = 0.0f; -> Pivot
	// float Offset_y = 0.0f; 
	float Near = 0.01f;
	float Far = 1000.0f;

	float aspect() { return Width / Height; }
};