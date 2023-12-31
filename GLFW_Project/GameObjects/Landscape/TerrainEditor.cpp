#include "../Framework.h"

TerrainEditor::TerrainEditor(uint width, uint height) : Size(width, height) {
	material = new Material(
		"Assets/GLSL/Landscape/TerrainEditor.vert", 
		"Assets/GLSL/Landscape/TerrainEditor.frag");
	material->LockShader();
	brushBuffer = new BrushBuffer(material->Get_ProgramID());
	wBuffer = new MatrixBuffer(material->Get_ProgramID(), "world");
	
	alphaMapBuffer = new TextureBuffer(material->Get_ProgramID(), "alphaMap");
	secondDiffuseMapBuffer  = new TextureBuffer(material->Get_ProgramID(), "secondDiffuseMap");
	secondSpecularMapBuffer = new TextureBuffer(material->Get_ProgramID(), "secondSpecularMap");
	secondNormalMapBuffer   = new TextureBuffer(material->Get_ProgramID(), "secondNormalMap");
	thirdDiffuseMapBuffer  = new TextureBuffer(material->Get_ProgramID(), "thirdDiffuseMap");
	thirdSpecularMapBuffer = new TextureBuffer(material->Get_ProgramID(), "thirdSpecularMap");
	thirdNormalMapBuffer   = new TextureBuffer(material->Get_ProgramID(), "thirdNormalMap");

	secondDiffuseMapBuffer ->slot = 4;
	secondSpecularMapBuffer->slot = 5;
	secondNormalMapBuffer  ->slot = 6;
	thirdDiffuseMapBuffer ->slot = 7;
	thirdSpecularMapBuffer->slot = 8;
	thirdNormalMapBuffer  ->slot = 9;

	pre_ray_checker = new BoxCollider();
	pre_ray_checker->SetParent(this);
	CreateMesh();
}
TerrainEditor::~TerrainEditor() {
	SAFE_DELETE(material);
	SAFE_DELETE(mesh);
	SAFE_DELETE(wBuffer);
	SAFE_DELETE(brushBuffer);

	SAFE_DELETE(alphaMapBuffer);
	SAFE_DELETE(secondDiffuseMapBuffer );
	SAFE_DELETE(secondSpecularMapBuffer);
	SAFE_DELETE(secondNormalMapBuffer  );
	SAFE_DELETE(thirdDiffuseMapBuffer );
	SAFE_DELETE(thirdSpecularMapBuffer);
	SAFE_DELETE(thirdNormalMapBuffer  );

	SAFE_T_DELETE(pre_ray_checker);
}

void TerrainEditor::CreateMesh() {
	SAFE_DELETE(mesh);
	vector<Vector4> pixels(Size.x * Size.y, Vector4(0, 0, 0, 0));
	if (heightMap) {
		Size.x = heightMap->Width();
		Size.y = heightMap->Height();
		heightMap->ReadPixel4(pixels);
	}
	vertices.clear();
	indices .clear();

	// Vertices
	for (int z = 0; z < Size.y; z++) {
		for (int x = 0; x < Size.x; x++) {
			VertexAlpha elem;
			elem.pos = { float(x), 0.0f, (float)z };
			elem.pos.y = pixels[Size.x * z + x].x * MAX_HEIGHT;

			elem.uv.x = 1 - (x / (float)(Size.x - 1));
			elem.uv.y = (z / (float)(Size.y - 1));

			elem.normal = { 0.0f, 0.0f, 0.0f };

			vertices.emplace_back(elem);
		}
	}
	// Indices
	for (int z = 0; z < Size.y - 1; z++) {
		for (int x = 0; x < Size.x - 1; x++) {
			indices.emplace_back(x + ((z + 1) * Size.x));
			indices.emplace_back(x + 1 + ((z + 1) * Size.x));
			indices.emplace_back(x + (z * Size.x));

			indices.emplace_back(x + (z * Size.x));
			indices.emplace_back(x + 1 + ((z + 1) * Size.x));
			indices.emplace_back(x + 1 + (z * Size.x));
		}
	}

	CalcNormal();
	CalcTangent();

	mesh = new AlphaMesh(vertices, indices);
	FindHighest();
	UpdateRayChecker();
}

void TerrainEditor::Update() { 
	if (ImGui::GetIO().WantCaptureMouse) return;
	if (MOUSE_PRESS(1)) return;
	ray = MAIN_CAMERA->ScreenPointToRay(mousePos); 
	if (!pre_ray_checker->RayCollision(ray)) return;

	PickingPos = brushBuffer->picking_posBuffer->data = Picking();

	if (MOUSE_PRESS(0)) Brush();
	if (MOUSE_UP(0)) {
		ResetNormalTangent();
		CalcNormal();
		CalcTangent();

		mesh->UpdateVertex(vertices);
	}
}
void TerrainEditor::Render() {
	if (!Active()) return;
	material->Set();

	brushBuffer->Set();

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

	wBuffer->data = world;
	wBuffer->Set();
	mesh->Set();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
}

void TerrainEditor::CalcNormal() {
	uint idx1 = 0;
	uint idx2 = 0;
	uint idx3 = 0;
	int n = indices.size() / 3;
	for (int i = 0; i < n; i++) {
		idx1 = indices[i * 3 + 0];
		idx2 = indices[i * 3 + 1];
		idx3 = indices[i * 3 + 2];

		Vector3 normal = glm::normalize(glm::cross(
			vertices[idx2].pos - vertices[idx1].pos,
			vertices[idx3].pos - vertices[idx1].pos));

		vertices[idx1].normal += normal;
		vertices[idx2].normal += normal;
		vertices[idx3].normal += normal;
	}
}
void TerrainEditor::CalcTangent() {
	uint idx1 = 0;
	uint idx2 = 0;
	uint idx3 = 0;
	uint n = indices.size() / 3;
	for (uint i = 0; i < n; i++) {
		idx1 = indices[i * 3 + 0];
		idx2 = indices[i * 3 + 1];
		idx3 = indices[i * 3 + 2];

		Vector3 e0 = vertices[idx2].pos - vertices[idx1].pos;
		Vector3 e1 = vertices[idx3].pos - vertices[idx1].pos;

		float u0 = vertices[idx2].uv.x - vertices[idx1].uv.x;
		float u1 = vertices[idx3].uv.x - vertices[idx1].uv.x;
		float v0 = vertices[idx2].uv.y - vertices[idx1].uv.y;
		float v1 = vertices[idx3].uv.y - vertices[idx1].uv.y;

		Vector3 tangent = (1.0f / (u0 * v1 - v0 * u1)) * (e0 * v1 - e1 * v0);

		vertices[idx1].tangent += tangent;
		vertices[idx2].tangent += tangent;
		vertices[idx3].tangent += tangent;
	}
	for (VertexAlpha& elem : vertices) {
		Vector3 t = elem.tangent;
		Vector3 n = elem.normal;
		elem.tangent = glm::normalize(t - n * glm::dot(n, t));
	}
}
void TerrainEditor::ResetNormalTangent() 
{ for (VertexAlpha& elem : vertices) elem.tangent = elem.normal = Vector3(0, 0, 0); }

void TerrainEditor::Brush() {
	Point origin(PickingPos.x, PickingPos.z);
	if (!Calc::isInIdx(origin, Size) || editType == EditType::NONE) return;

	// 브러시 범위 구하기
	float& brush_range = brushBuffer->rangeBuffer->data;
	Point brush_min = { origin.x - floor(brush_range), origin.y - floor(brush_range) };
	Point brush_max = { origin.x + ceil (brush_range), origin.y + ceil (brush_range) };
	brush_min = Calc::ClampIdx(brush_min, Size - 1);
	brush_max = Calc::ClampIdx(brush_max, Size - 1);

	switch (brushBuffer->brush_typeBuffer->data) { // 브러시 타입별로 목표로하는 버택스 세부 선별
	case 0: { // 원
		int idx = 0;
		float distance;
		for (int z = brush_min.y; z <= brush_max.y; z++) {
			for (int x = brush_min.x; x <= brush_max.x; x++) {
				idx = x + (z * Size.x);
				distance = glm::distance(
					Vector3(vertices[idx].pos.x, 0, vertices[idx].pos.z), 
					Vector3(PickingPos.x       , 0, PickingPos.z       ));
				if (distance < brush_range)
					Brushing(vertices[idx]);
			}
		}
		break;
	}
	case 1: { // 사각형
		for (int z = brush_min.y; z <= brush_max.y; z++)
			for (int x = brush_min.x; x <= brush_max.x; x++)
				Brushing(vertices[x + (z * Size.x)]);
		break;
	}
	case 2: { // 삼각형?
		for (int z = brush_min.y; z <= brush_max.y; z++) // 일단 사각형으로 작동
			for (int x = brush_min.x; x <= brush_max.x; x++)
				Brushing(vertices[x + (z * Size.x)]);
		break;
	}
	default: break;
	}
	mesh->UpdateMesh(vertices, indices);
}
void TerrainEditor::Brushing(VertexAlpha& target) {
	switch (editType) {
	case EditType::HEIGHT: {
		target.pos.y = 
			Calc::Clamp(target.pos.y + AdjustStrength * DELTA, 0.0f, MAX_HEIGHT);
		return;
	}
	case EditType::EVERAGE: {
		target.pos.y = 
			Calc::Lerp(target.pos.y, AdjustStrength, DELTA);
		return;
	}
	case EditType::ALPHA: {
		target.alpha[selectMap] = 
			Calc::Clamp(target.alpha[selectMap] + AdjustStrength * DELTA, 0.0f, 1.0f);
		return;
	}
	default: return;
	}
}

Vector3 TerrainEditor::Picking() {
	for (int z = 0; z < Size.y - 1; z++) {
		for (int x = 0; x < Size.x - 1; x++) {
			Vector3 p[4] = {
				vertices[Size.x *  z +      x    ].pos,
				vertices[Size.x *  z +      x + 1].pos,
				vertices[Size.x * (z + 1) + x    ].pos,
				vertices[Size.x * (z + 1) + x + 1].pos
			};

			float distance = 0;
			if (Calc::RayTriangle_Distance(ray, p[1], p[0], p[2], distance)) return ray.position + ray.direction * distance;
			if (Calc::RayTriangle_Distance(ray, p[3], p[1], p[2], distance)) return ray.position + ray.direction * distance;
		}
	}
	return Vector3(0, 0, 0);
}
Vector3 TerrainEditor::PointPicking() {
	Point prev_origin = Point(PickingPos.x, PickingPos.z);
	while (!searchList.empty()) searchList.pop();
	searchList.push(prev_origin);
	for (int i = 1; !searchList.empty(); i++) {
		// 탐색 
		for (Point elem; !searchList.empty(); searchList.pop()) {
			elem = searchList.top();
			Vector3 p[4] = {
				vertices[Size.x *  elem.y +      elem.x    ].pos,
				vertices[Size.x *  elem.y +      elem.x + 1].pos,
				vertices[Size.x * (elem.y + 1) + elem.x    ].pos,
				vertices[Size.x * (elem.y + 1) + elem.x + 1].pos
			};
			float distance = 0;
			if (Calc::RayTriangle_Distance(ray, p[1], p[0], p[2], distance)) return ray.position + ray.direction * distance;
			if (Calc::RayTriangle_Distance(ray, p[3], p[1], p[2], distance)) return ray.position + ray.direction * distance;
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

void TerrainEditor::UpdateRayChecker() {
	pre_ray_checker->Position().x = Size.x / 2.0f - 0.5f;
	pre_ray_checker->Position().z = Size.y / 2.0f - 0.5f;
	pre_ray_checker->Position().y = highest / 2.0f;
	pre_ray_checker->Scale().x = static_cast<float>(Size.x - 1);
	pre_ray_checker->Scale().z = static_cast<float>(Size.y - 1);
	pre_ray_checker->Scale().y = highest;
	pre_ray_checker->UpdateWorld();
}

void TerrainEditor::FindHighest() {
	highest = FLT_MIN;
	for (const VertexAlpha& elem : vertices)
		if (highest < elem.pos.y) highest = elem.pos.y;
}

// 특정 위치의 높이 구하기
float TerrainEditor::getHeight(float const& x, float const& z) {
	if (!Calc::isInIdx(Point(floor(x), floor(z)), Size)) return 0.0f;
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

void TerrainEditor::GUIRender() {
	if (ImGui::TreeNode(("TerrainEditor_" + tag).c_str())) {
		ImGui::Text("Picking Pos : %.2f, %.2f, %.2f", PickingPos.x, PickingPos.y, PickingPos.z);

		ImGui::SliderInt("BrushType", &brushBuffer->brush_typeBuffer->data, 0, 2);
		ImGui::SliderFloat("Range", &brushBuffer->rangeBuffer->data, 0.5f, 100.0f);
		ImGui::SliderFloat("Adjust Strength", &AdjustStrength, -30.0f, 30.0f);
		ImGui::ColorEdit3("BrushColor", (float*)&brushBuffer->brush_colorBuffer->data);

		const char* editList[] = { "None", "Height", "Average", "Alpha" };
		ImGui::Combo("EditType", (int*)&editType, editList, 4);

		switch (editType) {
		case EditType::NONE: {
			LoadHeightDialog(); ImGui::SameLine(); LoadAlphaDialog();
			SaveHeightDialog(); ImGui::SameLine(); SaveAlphaDialog();
			break;
		}
		case EditType::HEIGHT: case EditType::EVERAGE: {
			LoadHeightDialog();
			SaveHeightDialog();
			break;
		}
		case EditType::ALPHA: {
			ImGui::SliderInt("SelectMap", &selectMap, 0, 1);

			{
				string path;
				path = EditorGUI::Dialog_Button_Pattern(
					"Second", "Second Material", "Load Sceond From Material",
					".mat", "Assets\\Data\\Materials\\.");
				if (path.size() != 0) {
					Material* data = new Material();
					data->LoadMaterial(path);
					secondDiffuseMap  = Texture::Load(data->GetDiffusePath());
					secondSpecularMap = Texture::Load(data->GetSpecularPath());
					secondNormalMap   = Texture::Load(data->GetNormalPath());
					delete data;
				}
				ImGui::SameLine();
				path = EditorGUI::Dialog_Button_Pattern(
					"Third", "Third Material", "Load Third From Material",
					".mat", "Assets\\Data\\Materials\\.");
				if (path.size() != 0) {
					Material* data = new Material();
					data->LoadMaterial(path);
					thirdDiffuseMap  = Texture::Load(data->GetDiffusePath());
					thirdSpecularMap = Texture::Load(data->GetSpecularPath());
					thirdNormalMap   = Texture::Load(data->GetNormalPath());
					delete data;
				}
				ImGui::SameLine();
				SaveAlphaDialog();
			}

			
			EditorGUI::Dialog_ImageButton_Pattern("SecondDiffuse", "Diffuse.2", secondDiffuseMap);
			ImGui::SameLine();
			EditorGUI::Dialog_ImageButton_Pattern("ThirdDiffuse", "Diffuse.3", thirdDiffuseMap);
			ImGui::SameLine();
			LoadAlphaDialog();

			EditorGUI::Dialog_ImageButton_Pattern("SecondSpecular", "Specular.2", secondSpecularMap);
			ImGui::SameLine();
			EditorGUI::Dialog_ImageButton_Pattern("ThirdSpecular", "Specular.3", thirdSpecularMap);

			EditorGUI::Dialog_ImageButton_Pattern("SecondNormal", "Normal.2", secondNormalMap);
			ImGui::SameLine();
			EditorGUI::Dialog_ImageButton_Pattern("ThirdNormal", "Normal.3", thirdNormalMap);
			break;
		}
		default: break;
		}

		material->GUIRender();

		LoadTerrainDialog(); ImGui::SameLine(); SaveTerrainDialog();
		ImGui::TreePop();
	}
}

// HeightMap Save/Lode
void TerrainEditor::LoadHeight(string path) {
	heightMap = Texture::Load(path);
	SAFE_DELETE(mesh);
	CreateMesh();
}
void TerrainEditor::SaveHeight(string path) {
	int n = Size.x * Size.y; 
	GLubyte* pixels = new GLubyte[n * 3];
	for (int i = 0; i < n; i++) {
		GLubyte height = static_cast<GLubyte>((vertices[i].pos.y / MAX_HEIGHT) * 255);
		pixels[i * 3 + 0] = height;
		pixels[i * 3 + 1] = height;
		pixels[i * 3 + 2] = height;
	}
	stbi_write_png(path.c_str(), Size.x, Size.y, 3, pixels, Size.x * 3);
	heightMap = Texture::Load(path);
	SAFE_DELETE_LIST(pixels);
}
void TerrainEditor::LoadHeightDialog() 
{ if (EditorGUI::Dialog_ImageButton_Pattern("LoadHeight", "Load HeightMap", heightMap)) LoadHeight(heightMap->GetPath()); }
void TerrainEditor::SaveHeightDialog() {
	string path = EditorGUI::Dialog_Button_Pattern(
		"Save Height", "SaveHeight", "Save Height Map", ".png", "Assets\\.");
	if (path.size() != 0) SaveHeight(path);
}

// AlphaMap Save/Lode
void TerrainEditor::UpdateAlpha() {
	vector<Vector4> pixels;
	alphaMap->ReadPixel4(pixels);
	if (vertices.size() != pixels.size()) {
		cout << "Warning!! alpha pixel count difference while [TerrainEditor::UpdateAlpha]" << endl;
		cout << "vertices.size() = " << vertices.size() << endl;
		cout << "pixels.size() = " << pixels.size() << endl;
	}
	uint vrt_n = vertices.size(); 
	uint alp_n = pixels.size();
	for (uint i = 0; i < vrt_n && i < alp_n; i++) vertices[i].alpha = pixels[i]; 
}
void TerrainEditor::SaveAlpha(string path) {
	int n = Size.x * Size.y;
	GLubyte* pixels = new GLubyte[n * 3];
	for (int i = 0; i < n; i++) {
		pixels[i * 3 + 0] = static_cast<GLubyte>(vertices[i].alpha.x * 255); 
		pixels[i * 3 + 1] = static_cast<GLubyte>(vertices[i].alpha.y * 255); 
		pixels[i * 3 + 2] = static_cast<GLubyte>(vertices[i].alpha.z * 255);
	}
	stbi_write_png(path.c_str(), Size.x, Size.y, 3, pixels, Size.x * 3);
	alphaMap = Texture::Load(path);
	SAFE_DELETE_LIST(pixels);
}
void TerrainEditor::LoadAlphaDialog() 
{ if (EditorGUI::Dialog_ImageButton_Pattern("LoadAlpha", "Load AlphaMap", alphaMap)) UpdateAlpha(); }

void TerrainEditor::SaveAlphaDialog() {
	string path = EditorGUI::Dialog_Button_Pattern(
		"Save Alpha", "SaveAlpha", "Save Alpha Map", ".png", "Assets\\.");
	if(path.size() != 0) SaveAlpha(path);
}

// Terrain Save/Lode
void TerrainEditor::LoadTerrain(string path) {
	BinaryReader* r = new BinaryReader(path);

	Tag() = r->ReadLine();

	LoadHeight(r->ReadLine());
	alphaMap = Texture::Load(r->ReadLine());
	UpdateAlpha();

	material->LoadMaterial(r->ReadLine());
	material->SetShader(
		"Assets/GLSL/Landscape/TerrainEditor.vert", 
		"Assets/GLSL/Landscape/TerrainEditor.frag");
	
	secondDiffuseMap  = Texture::Load(r->ReadLine());  // Second
	secondSpecularMap = Texture::Load(r->ReadLine());
	secondNormalMap   = Texture::Load(r->ReadLine());

	thirdDiffuseMap  = Texture::Load(r->ReadLine()); // Third
	thirdSpecularMap = Texture::Load(r->ReadLine());
	thirdNormalMap   = Texture::Load(r->ReadLine());

	SAFE_DELETE(r);
}
void TerrainEditor::SaveTerrain(string path) {
	BinaryWriter* w = new BinaryWriter(path);

	w->WriteLine(Tag());
	{ // height
		string height_path = path.substr(0, path.find_last_of('.')) + "_height.png";
		SaveHeight(height_path);
		w->WriteLine(height_path);
	}
	
	{ // alpha
		string alpha_path = path.substr(0, path.find_last_of('.')) + "_alpha.png";
		SaveAlpha(alpha_path);
		w->WriteLine(alpha_path);
	}
	
	{ // material
		string material_path = path.substr(0, path.find_last_of('.')) + "_material.mat";
		material->SaveMaterial(material_path);
		w->WriteLine(material_path);
	}
	
	w->WriteLine((secondDiffuseMap ) ? secondDiffuseMap ->GetPath() : ""); // second
	w->WriteLine((secondSpecularMap) ? secondSpecularMap->GetPath() : "");
	w->WriteLine((secondNormalMap  ) ? secondNormalMap  ->GetPath() : "");

	w->WriteLine((thirdDiffuseMap  ) ? thirdDiffuseMap  ->GetPath() : "");  // Third
	w->WriteLine((thirdSpecularMap ) ? thirdSpecularMap ->GetPath() : "");
	w->WriteLine((thirdNormalMap   ) ? thirdNormalMap   ->GetPath() : "");

	SAFE_DELETE(w);
}
void TerrainEditor::LoadTerrainDialog() {
	string map_path = EditorGUI::Dialog_Button_Pattern( 
		"Load Terrain", "LoadTerrain", "Load Terrain", ".map", "Assets\\Data\\Maps\\."); 
	if (map_path.size() != 0) LoadTerrain(map_path); 
}
void TerrainEditor::SaveTerrainDialog() {
	string map_path = EditorGUI::Dialog_Button_Pattern(
		"Load Terrain", "LoadTerrain", "Load Terrain", ".map", "Assets\\Data\\Maps\\.");
	if (map_path.size() != 0) {
		SaveTerrain(map_path);
	}
}

TerrainEditor::AlphaMesh::AlphaMesh(const vector<VertexAlpha>& vertices, const vector<uint>& indices) : Mesh<VertexAlpha>() {
	glBindVertexArray(vertex_array_id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexAlpha), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

	uint stride = sizeof(VertexAlpha);
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride // 0 
		, (void*)0);
	glEnableVertexAttribArray(0); 
	// uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride // pos(Vector3) 
		, (void*)(sizeof(Vector3))); 
	glEnableVertexAttribArray(1); 
	// normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride // pos(Vector3) + uv(Vector2)
		, (void*)(sizeof(Vector3) + sizeof(Vector2))); 
	glEnableVertexAttribArray(2); 
	// tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride // pos(Vector3) + uv(Vector2) + normal(Vector3) 
		, (void*)(sizeof(Vector3) + sizeof(Vector2) + sizeof(Vector3)));
	glEnableVertexAttribArray(3);
	// alpha
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride // pos(Vector3) + uv(Vector2) + normal(Vector3) + tangent(Vector3)
		, (void*)(sizeof(Vector3) + sizeof(Vector2) + sizeof(Vector3) + sizeof(Vector3)));
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

TerrainEditor::BrushBuffer::BrushBuffer(const uint& targetProgramID) {
	brush_typeBuffer = new IntBuffer(targetProgramID, "brush_type");
	brush_colorBuffer = new Vector3Buffer(targetProgramID, "brush_color");
	picking_posBuffer = new Vector3Buffer(targetProgramID, "picking_pos");
	rangeBuffer = new FloatBuffer(targetProgramID, "range");

	brush_typeBuffer->data = 0;
	brush_colorBuffer->data = Vector3(0, 0.5f, 0);
	picking_posBuffer->data = Vector3(0, 0, 0);
	rangeBuffer->data = 5.0f;
}
TerrainEditor::BrushBuffer::~BrushBuffer() {
	SAFE_DELETE(brush_typeBuffer);
	SAFE_DELETE(brush_colorBuffer);
	SAFE_DELETE(picking_posBuffer);
	SAFE_DELETE(rangeBuffer);
}
void TerrainEditor::BrushBuffer::Set() {
	brush_typeBuffer ->Set();
	brush_colorBuffer->Set();
	picking_posBuffer->Set();
	rangeBuffer->Set();
}
