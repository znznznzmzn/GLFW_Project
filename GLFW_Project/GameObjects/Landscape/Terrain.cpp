#include "../Framework.h"

void Terrain::CreateMesh() {
	vector<Vector4> pixels(Size.x * Size.y, Vector4(0, 0, 0, 0));
	if (heightMap) {
		Size.x = heightMap->Width();
		Size.y = heightMap->Height();
		heightMap->ReadPixel4(pixels);
	}
	vertices.clear();
	indices.clear();

	// Vertices
	for (int z = 0; z < Size.y; z++) {
		for (int x = 0; x < Size.x; x++) {
			Vertex elem;
			elem.pos = { (float)x, 0.0f, (float)z };
			int index = Size.x * z + x;
			elem.pos.y = pixels[index].x * MAX_HEIGHT;

			elem.uv.x = 1.0f - (x / (float)(Size.x - 1));
			elem.uv.y = 1.0f - (z / (float)(Size.y - 1));
			
			elem.normal = { 0.0f, 0.0f, 0.0f };

			vertices.emplace_back(elem);
		}
	}
	// Indices
	for (int z = 0; z < Size.y - 1; z++) {
		for (int x = 0; x < Size.x - 1; x++) {
			indices.emplace_back(x +     ((z + 1) * Size.x));
			indices.emplace_back(x + 1 + ((z + 1) * Size.x));
			indices.emplace_back(x +     ( z      * Size.x));

			indices.emplace_back(x +     ( z      * Size.x));
			indices.emplace_back(x + 1 + ((z + 1) * Size.x));
			indices.emplace_back(x + 1 + ( z      * Size.x));
		}
	}

	Calc::Vertexing::Normal(vertices, indices);
	Calc::Vertexing::Tangent(vertices, indices);
}
void Terrain::UpdateMesh() { mesh->UpdateMesh(vertices, indices); }

void Terrain::CreateBuffers() {
	alphaMapBuffer = new TextureBuffer(material->Get_ProgramID(), "alphaMap");
	secondDiffuseMapBuffer  = new TextureBuffer(material->Get_ProgramID(), "secondDiffuseMap");
	secondSpecularMapBuffer = new TextureBuffer(material->Get_ProgramID(), "secondSpecularMap");
	secondNormalMapBuffer   = new TextureBuffer(material->Get_ProgramID(), "secondNormalMap");
	thirdDiffuseMapBuffer  = new TextureBuffer(material->Get_ProgramID(), "thirdDiffuseMap");
	thirdSpecularMapBuffer = new TextureBuffer(material->Get_ProgramID(), "thirdSpecularMap");
	thirdNormalMapBuffer   = new TextureBuffer(material->Get_ProgramID(), "thirdNormalMap");

	alphaMapBuffer->slot = 3; // Material이 0 ~ 2를 차지하고 있음
	secondDiffuseMapBuffer ->slot = 4;
	secondSpecularMapBuffer->slot = 5;
	secondNormalMapBuffer  ->slot = 6;
	thirdDiffuseMapBuffer ->slot = 7;
	thirdSpecularMapBuffer->slot = 8;
	thirdNormalMapBuffer  ->slot = 9;
}

Terrain::Terrain(const uint& width, const uint& height) : Size(width, height) {
	init(
		"Assets/GLSL/Landscape/Terrain.vert", 
		"Assets/GLSL/Landscape/Terrain.frag");
	material->LockShader();
	CreateMesh();
	UpdateWorld();
	Tag() = "terrain";
}
Terrain::Terrain(const string& path) {
	init(
		"Assets/GLSL/Landscape/Terrain.vert", 
		"Assets/GLSL/Landscape/Terrain.frag");
	material->LockShader();
	LoadTerrain(path);
	UpdateWorld();
}
Terrain::~Terrain() {
	SAFE_DELETE(alphaMapBuffer);
	SAFE_DELETE(secondDiffuseMapBuffer);
	SAFE_DELETE(secondSpecularMapBuffer);
	SAFE_DELETE(secondNormalMapBuffer);
	SAFE_DELETE(thirdDiffuseMapBuffer);
	SAFE_DELETE(thirdSpecularMapBuffer);
	SAFE_DELETE(thirdNormalMapBuffer);
}

void Terrain::Render() {
	if (!Active()) return;
	material->UseProgram();
	if (alphaMap) {
		alphaMap->Set(alphaMapBuffer->slot);
		alphaMapBuffer->Set();
	}

	if (secondDiffuseMap) {
		secondDiffuseMap ->Set(secondDiffuseMapBuffer->slot);
		secondDiffuseMapBuffer->Set();
	}
	if (secondSpecularMap) {
		secondSpecularMap->Set(secondSpecularMapBuffer->slot);
		secondSpecularMapBuffer->Set();
	}
	if (secondNormalMap) {
		secondNormalMap->Set(secondNormalMapBuffer->slot);
		secondNormalMapBuffer->Set();
	}

	if (thirdDiffuseMap) {
		thirdDiffuseMap->Set(thirdDiffuseMapBuffer->slot);
		thirdDiffuseMapBuffer->Set();
	}
	if (thirdSpecularMap) {
		thirdSpecularMap->Set(thirdSpecularMapBuffer->slot);
		thirdSpecularMapBuffer->Set();
	}
	if (thirdNormalMap) {
		thirdNormalMap->Set(thirdNormalMapBuffer->slot);
		thirdNormalMapBuffer->Set();
	}

	GameObject::Render();
}

void Terrain::GUIRender() {
	if (ImGui::TreeNode(("Terrain_" + tag).c_str())) {
		LoadTerrainDialog();
		LoadHeightDialog(); ImGui::SameLine(); LoadAlphaDialog();
		GameObject::GUIRender();
		ImGui::TreePop();
	}
}

float Terrain::getHeight(float x, float z) {
	if (!Calc::isInIdx(Point(static_cast<int>(floor(x)), static_cast<int>(floor(z))), Size)) return 0.0f;
	int idx[4] = {
		static_cast<int>(floor(floor(x) +     (floor(z    ) * Size.x))), // 2 3
		static_cast<int>(floor(floor(x) + 1 + (floor(z    ) * Size.x))), // 0 1
		static_cast<int>(floor(floor(x) +     (floor(z + 1) * Size.x))), // 요 범위안에 있음
		static_cast<int>(floor(floor(x) + 1 + (floor(z + 1) * Size.x)))
	};
	float prog_x = x - vertices[idx[0]].pos.x;
	float prog_z = z - vertices[idx[0]].pos.z;
	float inc_x = (prog_x > prog_z) ? 
		(vertices[idx[1]].pos.y - vertices[idx[0]].pos.y) :
		(vertices[idx[3]].pos.y - vertices[idx[2]].pos.y);
	float inc_z = (prog_x > prog_z) ?
		(vertices[idx[3]].pos.y - vertices[idx[1]].pos.y) :
		(vertices[idx[2]].pos.y - vertices[idx[0]].pos.y);
	return vertices[idx[0]].pos.y + (inc_x * prog_x + inc_z * prog_z);
}

// HeightMap Lode
void Terrain::LoadHeight() {
	CreateMesh();
	UpdateMesh();
}
void Terrain::LoadHeightDialog() 
{ if (EditorGUI::Dialog_ImageButton_Pattern("LoadHeight", "Load HeightMap", heightMap)) LoadHeight(); }

// AlphaMap Lode
void Terrain::LoadAlphaDialog() 
{ EditorGUI::Dialog_ImageButton_Pattern("LoadAlpha", "Load AlphaMap", alphaMap); } // alphaMap은 자동업뎃

// Terrain Lode
void Terrain::LoadTerrain(string path) {
	BinaryReader* r = new BinaryReader(path);
	Tag() = r->ReadLine();

	heightMap = Texture::Load(r->ReadLine());
	alphaMap  = Texture::Load(r->ReadLine());

	material->LoadMaterial(r->ReadLine());
	material->SetShader(
		"Assets/GLSL/Landscape/Terrain.vert", 
		"Assets/GLSL/Landscape/Terrain.frag");

	secondDiffuseMap  = Texture::Load(r->ReadLine());  // Second
	secondSpecularMap = Texture::Load(r->ReadLine());
	secondNormalMap   = Texture::Load(r->ReadLine());

	thirdDiffuseMap  = Texture::Load(r->ReadLine()); // Third
	thirdSpecularMap = Texture::Load(r->ReadLine());
	thirdNormalMap   = Texture::Load(r->ReadLine());

	SAFE_DELETE(r);
}
void Terrain::LoadTerrainDialog() {
	string map_path = EditorGUI::Dialog_Button_Pattern(
		"Load Terrain", "LoadTerrain", "Load Terrain", ".map", "Assets\\Data\\Maps\\.");
	if (map_path.size() != 0) {
		LoadTerrain(map_path);
		UpdateWorld();
	}
}

Vector3 Terrain::PointPicking(Camera*& from) {
	Ray ray = from->ScreenPointToRay(mousePos);
	Point prev_origin = Point(PickingPos.x, PickingPos.z);
	while (!searchList.empty()) searchList.pop();
	searchList.push(prev_origin);
	for (int i = 1; !searchList.empty(); i++) {
		// 탐색 
		for (Point elem; !searchList.empty(); searchList.pop()) {
			elem = searchList.top();
			uint idx[4];
			idx[0] = elem.x + Size.x * elem.y;
			idx[1] = elem.x + 1 + Size.x * elem.y;
			idx[2] = elem.x + Size.x * (elem.y + 1);
			idx[3] = elem.x + 1 + Size.x * (elem.y + 1);
			Vector3 p[4];
			for (uint i = 0; i < 4; i++) p[i] = vertices[idx[i]].pos;

			float distance = false;
			if (Calc::RayTriangle_Distance(ray, p[0], p[1], p[2], distance))
				return PickingPos = ray.position + ray.direction * distance;
			if (Calc::RayTriangle_Distance(ray, p[3], p[1], p[2], distance))
				return PickingPos = ray.position + ray.direction * distance;
		}

		// searchList 재삽입
		int odd = (i * 2 + 1);
		if (Calc::isInIdx(prev_origin.y - i, Size.y - 1)) {
			int line_y = prev_origin.y - i;
			for (int x = 0; x < odd; x++) { // 아랫쪽 x
				Point elem = Point(prev_origin.x - i + x, line_y);
				if (Calc::isInIdx(elem.x, Size.x - 1)) searchList.push(elem);
			}
		}
		if (Calc::isInIdx(prev_origin.y + i, Size.y - 1)) {
			int line_y = prev_origin.y + i;
			for (int x = 0; x < odd; x++) { // 윗쪽 x
				Point elem = Point(prev_origin.x - i + x, line_y);
				if (Calc::isInIdx(elem.x, Size.x - 1)) searchList.push(elem);
			}
		}

		if (Calc::isInIdx(prev_origin.x - i, Size.x - 1)) {
			int line_x = prev_origin.x - i;
			for (int y = 0; y < odd - 2; y++) { // 왼쪽 y
				Point elem = Point(line_x, prev_origin.y - (i - 1) + y);
				if (Calc::isInIdx(elem.y, Size.y - 1)) searchList.push(elem);
			}
		}
		if (Calc::isInIdx(prev_origin.x + i, Size.x - 1)) {
			int line_x = prev_origin.x + i;
			for (int y = 0; y < odd - 2; y++) { // 오른쪽 y
				Point elem = Point(line_x, prev_origin.y - (i - 1) + y);
				if (Calc::isInIdx(elem.y, Size.y - 1)) searchList.push(elem);
			}
		}
	}
	return Vector3(0, 0, 0);
}