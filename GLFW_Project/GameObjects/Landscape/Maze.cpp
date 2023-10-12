#include "Framework.h"

void Maze::AStar_Maze::CreateNode(const vector<float4>& map_data, Point Size, float2 interval) {
	this->Size = Size;
	for (UINT z = 0; z < Size.y; z++) {
		for (UINT x = 0; x < Size.x; x++) {
			Vector3 pos = { 
				x * interval.x + interval.x / 2.0f,
				0.0f, 
				z * interval.y + interval.y / 2.0f
			};
			Node* elem = new Node(pos, nodes.size());
			int idx = Size.x * z + x;
			if (map_data[idx].x <= 0.0f && 
				map_data[idx].y <= 0.0f && 
				map_data[idx].z <= 0.0f) elem->SetObstacle(true);
			nodes.push_back(elem);
		}
	}
	SetEdge();
}

Maze::Panel::Panel(float2 size) {
	vertices.emplace_back(VertexType(0	   , 0, 0	  , 0, 1, 0, +1, 0));
	vertices.emplace_back(VertexType(0	   , 0, size.y, 0, 0, 0, +1, 0));
	vertices.emplace_back(VertexType(size.x, 0, size.y, 1, 0, 0, +1, 0));
	vertices.emplace_back(VertexType(size.x, 0, 0	  , 1, 1, 0, +1, 0));
	indices = { 0 , 1 , 2 , 0 , 2 , 3 };
	init();
}

void Maze::CreateMaze() {
	if (mazeMap == nullptr) return; // mazeMap

	// initialize
	PickableNodes.clear();
	Size.x = mazeMap->Width();
	Size.y = mazeMap->Height();
	vector<float4> pixels(Size.x * Size.y, float4(0, 0, 0, 0));
	mazeMap->ReadPixels(pixels);

	// Packer
	if (pack_wall) {
		pack_wall->NullParent();
		SAFE_DELETE(pack_wall);
	} pack_wall = new GameObject_Packer();

	if (pack_floor) {
		pack_floor->NullParent();
		SAFE_DELETE(pack_floor);
	} pack_floor = new GameObject_Packer();

	if (pack_ceiling) {
		pack_ceiling->NullParent();
		SAFE_DELETE(pack_ceiling);
	} pack_ceiling = new GameObject_Packer();

	pack_wall->SetParent(this);
	pack_floor->SetParent(this);
	pack_ceiling->SetParent(this);

	pack_wall->ClearPack();
	pack_floor->ClearPack();
	pack_ceiling->ClearPack();

	// Rooms
	for (int z = 0; z < Size.y; z++) {
		for (int x = 0; x < Size.x; x++) {
			// 흑색 = 빈공간 
			// x녹색 = 끝
			// y청색 = 시작
			// z적색 = 길 ※ 뒤집혀서 읽힌다
			// 
			int idx = Size.x * z + x;
			if (IsBlank(pixels[idx])) continue; // 빈공간은 넘어가기

			if (pixels[idx].x > 0.0f) { // 탈출구일 경우
				escapeCube = new Cube();
				escapeCube->Scale() = { RoomSize.x, RoomHeight, RoomSize.y };
				escapeCube->Position() = { RoomSize.x * x, 0, RoomSize.y * z };
				escapeCube->Position() += escapeCube->Scale() / 2.0f;
				escapeCube->GetMaterial()->SetShader(L"Shaders/Basic/Texture.hlsl");
				escapeCube->GetMaterial()->SetDiffuseMap(L"Textures/GUI/Basic.png");
				escapeCube->SetParent(this);
				escapeCube->UpdateWorld();

				if (escapeCollision) {
					escapeCollision->NullParent();
					SAFE_DELETE(escapeCollision);
				} escapeCollision = new BoxCollider();
				escapeCollision->Scale() = escapeCube->Scale();
				escapeCollision->Position() = escapeCube->Position();
				escapeCollision->SetParent(this);
				escapeCollision->UpdateWorld();
				continue;
			}
			if (pixels[idx].y > 0.0f) // 시작부분일경우
				StartPos = { // 시작위치 지정
					RoomSize.x * x + RoomSize.x / 2.0f,
					0.0f,
					RoomSize.y * z + RoomSize.y / 2.0f
			};

			// if (pixels[idx].x > 0.0f || pixels[idx].y > 0.0f) - 길일 경우
			Panel* floor = new Panel(RoomSize); // 바닥
			floor->Position() = { RoomSize.x * x, 0.0f, RoomSize.y * z };
			pack_floor->AddObject(floor);

			Panel* ceiling = new Panel(RoomSize); // 천장
			ceiling->Position() = { RoomSize.x * x, RoomHeight, RoomSize.y * z };
			ceiling->Rotation() = { 0.0f, XM_PI / 2.0f, XM_PI };
			pack_ceiling->AddObject(ceiling);

			//벽면
			Panel* Wall;
			if (z + 1 < Size.y) { //전방
				if (IsBlank(pixels[idx + Size.x])) {
					Wall = new Panel({ RoomSize.x, RoomHeight });
					Wall->Position() = { RoomSize.x * x, 0.0f, RoomSize.y * (z + 1) };
					Wall->Rotation() = { -XM_PI / 2.0f, 0.0f, 0.0f };
					pack_wall->AddObject(Wall);

					Vector3 colliderScl = { RoomSize.x, RoomHeight, RoomSize.y };
					Vector3 colliderPos = Vector3(RoomSize.x * x, 0.0f, RoomSize.y * (z + 1)) + colliderScl / 2.0f;
					if (!ContainCollision(colliderPos)) {
						BoxCollider* collision = new BoxCollider();
						collision->Scale() = colliderScl;
						collision->Position() = colliderPos;
						collision->SetParent(this);
						collisions.emplace_back(collision);
					}
				}
			}
			if (z - 1 >= 0) { //후방
				if (IsBlank(pixels[idx - Size.x])) {
					Wall = new Panel({ RoomSize.x, RoomHeight });
					Wall->Position() = { RoomSize.x * (x + 1), 0.0f, RoomSize.y * z };
					Wall->Rotation() = { -XM_PI / 2.0f, XM_PI, 0.0f };
					pack_wall->AddObject(Wall);

					Vector3 colliderScl = { RoomSize.x, RoomHeight, RoomSize.y };
					Vector3 colliderPos = Vector3(RoomSize.x * x, 0.0f, RoomSize.y * (z - 1)) + colliderScl / 2.0f;
					if (!ContainCollision(colliderPos)) {
						BoxCollider* collision = new BoxCollider();
						collision->Scale() = colliderScl;
						collision->Position() = colliderPos;
						collision->SetParent(this);
						collisions.emplace_back(collision);
					}
				}
			}
			if (x - 1 >= 0) { //좌측
				if (IsBlank(pixels[idx - 1])) {
					Wall = new Panel({ RoomSize.y, RoomHeight });
					Wall->Position() = { RoomSize.x * x, 0.0f, RoomSize.y * z };
					Wall->Rotation() = { -XM_PI / 2.0f, -XM_PI / 2.0f, 0.0f };
					pack_wall->AddObject(Wall);

					Vector3 colliderScl = { RoomSize.x, RoomHeight, RoomSize.y };
					Vector3 colliderPos = Vector3(RoomSize.x * (x - 1), 0.0f, RoomSize.y * z) + colliderScl / 2.0f;
					if (!ContainCollision(colliderPos)) {
						BoxCollider* collision = new BoxCollider();
						collision->Scale() = colliderScl;
						collision->Position() = colliderPos;
						collision->SetParent(this);
						collisions.emplace_back(collision);
					}
				}
			}
			if (x + 1 < Size.x) { //우측
				if (IsBlank(pixels[idx + 1])) {
					Wall = new Panel({ RoomSize.y, RoomHeight });
					Wall->Position() = { RoomSize.x * (x + 1), 0.0f, RoomSize.y * (z + 1) };
					Wall->Rotation() = { -XM_PI / 2.0f, XM_PI / 2.0f, 0.0f };
					pack_wall->AddObject(Wall);

					Vector3 colliderScl = { RoomSize.x, RoomHeight, RoomSize.y };
					Vector3 colliderPos = Vector3(RoomSize.x * (x + 1), 0.0f, RoomSize.y * z) + colliderScl / 2.0f;
					if (!ContainCollision(colliderPos)) {
						BoxCollider* collision = new BoxCollider();
						collision->Scale() = colliderScl;
						collision->Position() = colliderPos;
						collision->SetParent(this);
						collisions.emplace_back(collision);
					}
				}
			}
		}
	}
	pack_wall->Pack();
	pack_floor->Pack();
	pack_ceiling->Pack();

	pack_wall->ClearPack();
	pack_floor->ClearPack();
	pack_ceiling->ClearPack();

	// PathFinder
	if (PathFinder) SAFE_DELETE(PathFinder);
	PathFinder = new AStar_Maze(Size.x, Size.y);
	PathFinder->CreateNode(pixels, Size, RoomSize);
	//PathFinder->ToggleRender(true);
}

bool Maze::ContainCollision(Vector3 Position, float dist) {
	for (BoxCollider* elem : collisions)
		if (Vector3::distanceXZ(elem->Position(), Position) < dist) return true;
	return false;
}

Maze::Maze(wstring maze_map_path) {
	LoadMazeMap(maze_map_path);
	CreateMaze();
} 
Maze::~Maze() {
}

void Maze::UpdateWorld() {
	Transform::UpdateWorld();
	pack_wall   ->UpdateWorld();
	pack_floor  ->UpdateWorld();
	pack_ceiling->UpdateWorld();
	escapeCube->UpdateWorld();
	escapeCollision->UpdateWorld();
	for (BoxCollider*& elem : collisions) elem->UpdateWorld();
}
Vector3& Maze::GetRandomPos() {
	if (PickableNodes.empty()) {
		for (Node*& elem : PathFinder->GetNodes())
			if (!elem->IsObstacle()) PickableNodes.emplace_back(elem->GlobalPosition());
	}
	return PickableNodes[Random::NumberTo(PickableNodes.size())];
}

void Maze::CalcWallCollision(Transform* OUT target, SphereCollider* IN targetCollision) {
	for (BoxCollider*& elem : collisions) { //- ※ : 정사각형에만 통하고 오차가 있음, 수정 필
		if (Vector3::distance(elem->GlobalPosition(), target->GlobalPosition()) < 3.0f) {
			if (elem->Collision(targetCollision)) { // 거리계산 연산이 더 빠르기 때문에 거리연산 후 충돌체크
				distRay.position = targetCollision->GlobalPosition();
				distRay.position.y = elem->GlobalPosition().y;
				distRay.direction = (elem->GlobalPosition() - targetCollision->GlobalPosition()).normalize();
				distRay.direction.y = 0;
				if (elem->RayCollision(distRay, &powerContact)) { // 만약을 대비해 Ray충돌시 밀기 작동
					float power = targetCollision->Radius() - powerContact.distance; // 밀려날 거리 계산
					if (power > 0) { // 밀려날 거리가 0 이상일때 밀려남(아니면 붙어버림)
						target->Position() -= distRay.direction * power; // 밀기
						target->UpdateWorld(); // 밀려난 만큼 업데이트
					}
				}
			}
		}
	}
}

bool Maze::CollisionObstacle(Ray const& ray, float const& distance) {
	Contact contact;
	for (BoxCollider*& elem : collisions) {
		if (elem->RayCollision(ray, &contact))
			if (contact.distance < distance) return true;
	}
	return false;
}

void Maze::Render() {
	pack_wall->Render();
	pack_floor->Render();
	pack_ceiling->Render();
	escapeCube->Render();
	PathFinder->Render();
	//escapeCollision->Render();
	//for (BoxCollider*& elem : collisions) elem->Render();
}
void Maze::GUIRender() {
	PathFinder->GUIRender();
}