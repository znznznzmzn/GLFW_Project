#pragma once
class Terrain : public GameObject {
private:
	const float MAX_HEIGHT = 50.0f;

	bool model_switch = true;
	Point Size;

	Texture* heightMap = nullptr; // heightMap은 계산을 위한 것
	// 버퍼를 따로 사용할 필요는 없음

	Texture* alphaMap = nullptr;
	TextureBuffer* alphaMapBuffer = nullptr;

	Texture* secondDiffuseMap = nullptr;
	Texture* secondSpecularMap = nullptr;
	Texture* secondNormalMap = nullptr;
	TextureBuffer* secondDiffuseMapBuffer = nullptr;
	TextureBuffer* secondSpecularMapBuffer = nullptr;
	TextureBuffer* secondNormalMapBuffer = nullptr;

	Texture* thirdDiffuseMap = nullptr;
	Texture* thirdSpecularMap = nullptr;
	Texture* thirdNormalMap = nullptr;
	TextureBuffer* thirdDiffuseMapBuffer = nullptr;
	TextureBuffer* thirdSpecularMapBuffer = nullptr;
	TextureBuffer* thirdNormalMapBuffer = nullptr;

	stack<Point> searchList;

	void CreateMesh();
	void UpdateMesh();

	void LoadHeight();
	void LoadHeightDialog();

	void LoadAlphaDialog();

	void LoadTerrain(string path);
	void LoadTerrainDialog();

	void CreateBuffers();
public:
	Terrain(const uint& width = 100, const uint& height = 100);
	Terrain(const string& path);
	~Terrain();
	void Render() override;
	void GUIRender() override;

	void setModels(bool const& value) { model_switch = value; }

	float getHeight(Vector3 position) { return getHeight(position.x, position.z); }
	float getHeight(float x, float z);
	Point getSize() { return Size; }

	Vector3 PointPicking(Camera*& from = MAIN_CAMERA);
	Vector3 PickingPos = Vector3(0.0f, 0.0f, 0.0f);
};