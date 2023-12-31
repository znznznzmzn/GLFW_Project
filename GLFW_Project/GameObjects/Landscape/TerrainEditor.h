#pragma once

class TerrainEditor : public Transform {
private:
#pragma region ObjectData
	Material* material;
	MatrixBuffer* wBuffer;
	struct VertexAlpha {
		Vector3 pos;
		Vector2 uv;
		Vector3 normal;
		Vector3 tangent;
		Vector4 alpha;
		VertexAlpha() : pos(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0), alpha(0, 0, 0, 0) { }
	};
	class AlphaMesh : public Mesh<VertexAlpha> {
	public:
		AlphaMesh(const vector<VertexAlpha>& vertices, const vector<uint>& indices);
		~AlphaMesh() = default;
	}* mesh;

	class BrushBuffer {
	public:
		IntBuffer* brush_typeBuffer;
		Vector3Buffer* brush_colorBuffer;
		Vector3Buffer* picking_posBuffer;
		FloatBuffer* rangeBuffer;
		BrushBuffer(const uint& targetProgramID);
		~BrushBuffer();
		void Set();
	}* brushBuffer;
#pragma endregion

#pragma region TerrainData
	Texture* heightMap = nullptr; 

	Texture* alphaMap  = nullptr;
	TextureBuffer* alphaMapBuffer = nullptr;

	Texture* secondDiffuseMap = nullptr;
	Texture* secondSpecularMap = nullptr;
	Texture* secondNormalMap = nullptr;
	TextureBuffer* secondDiffuseMapBuffer  = nullptr;
	TextureBuffer* secondSpecularMapBuffer = nullptr;
	TextureBuffer* secondNormalMapBuffer   = nullptr;

	Texture* thirdDiffuseMap  = nullptr;
	Texture* thirdSpecularMap = nullptr;
	Texture* thirdNormalMap   = nullptr;
	TextureBuffer* thirdDiffuseMapBuffer  = nullptr;
	TextureBuffer* thirdSpecularMapBuffer = nullptr;
	TextureBuffer* thirdNormalMapBuffer   = nullptr;

	vector<VertexAlpha> vertices;
	vector<uint> indices;
	void CalcNormal();
	void CalcTangent();
	void ResetNormalTangent();

	Point Size;
	const float MAX_HEIGHT = 50.0f;
	float highest = MAX_HEIGHT;
	BoxCollider* pre_ray_checker = nullptr;
#pragma endregion

#pragma region EditorController
	Ray ray;
	stack<Point> searchList;
	enum class EditType { NONE, HEIGHT, EVERAGE, ALPHA };
	EditType editType = EditType::NONE;
	float AdjustStrength = 10.0f;
	int selectMap = 0;
	void Brush();
	void Brushing(VertexAlpha& target);

	void CreateMesh();

	Vector3 Picking();
	Vector3 PointPicking();
#pragma endregion

	void UpdateRayChecker();
	void FindHighest();

	void LoadHeight(string path);
	void SaveHeight(string path);
	void SaveHeightDialog();
	void LoadHeightDialog();

	void UpdateAlpha(); // 알파값의 전체 데이터를 수정
	void SaveAlpha(string path);
	void SaveAlphaDialog();
	void LoadAlphaDialog();

	void SaveTerrain(string path);
	void LoadTerrain(string path);
	void SaveTerrainDialog();
	void LoadTerrainDialog();

public:
	TerrainEditor(uint width = 100, uint height = 100);
	~TerrainEditor();

	void Update();
	void Render();
	void GUIRender();

	float getHeight(float const& x, float const& z);

	Vector3 PickingPos = Vector3(0.0f, 0.0f, 0.0f);
};