#pragma once

class Maze : public Transform {
private:
	float2 RoomSize = { 1.5f, 1.5f }; // �� �ϳ��� ������
	float RoomHeight = 3.0f; // �� �ϳ��� ����
	class AStar_Maze : public AStar { // �̷� ��ã��� AStar
	public:
		AStar_Maze(UINT width = 20, UINT height = 20) : AStar(width, height) {};
		~AStar_Maze() {}
		void CreateNode(const vector<float4>& map_data, Point Size, float2 interval);
	} *PathFinder;

	class Panel : public GameObject { public: Panel(float2 size = { 1.0f, 1.0f }); };
	Texture* mazeMap = nullptr;


	bool IsBlank(const float4& value) { return value.x <= 0.0f && value.y <= 0.0f && value.z <= 0.0f; }
	void CreateMaze();
	GameObject_Packer* pack_wall   ;
	GameObject_Packer* pack_floor  ;
	GameObject_Packer* pack_ceiling;

	Point Size; // ��ü ���� ������
	Ray distRay;
	Contact powerContact;

	bool ContainCollision(Vector3 Position, float dist = 0.3f);
	vector<BoxCollider*> collisions;
	Cube* escapeCube;
	BoxCollider* escapeCollision;

	vector<Vector3> PickableNodes;
public:
	Maze(wstring maze_map_path);
	~Maze();

	void Render();
	void GUIRender();
	void UpdateWorld() override;

	void LoadMazeMap(wstring maze_map_path) { mazeMap = Texture::Add(maze_map_path); }

	void SetWallMat   (string mat_path) { pack_wall   ->SetMaterial(mat_path); }
	void SetFloorMat  (string mat_path) { pack_floor  ->SetMaterial(mat_path); }
	void SetCeilingMat(string mat_path) { pack_ceiling->SetMaterial(mat_path); }

	Material*& GetWallMat   () { return pack_wall   ->GetMaterial(); }
	Material*& GetFloorMat  () { return pack_floor  ->GetMaterial(); }
	Material*& GetCeilingMat() { return pack_ceiling->GetMaterial(); }

	void GetPath(Vector3 start_pos, Vector3 end_pos, OUT vector<Vector3>& path)
	{ PathFinder->GetPath(start_pos, end_pos, path); } // ��ã�� ��û �ޱ�

	void GetRandomPath(Vector3 start_pos, OUT vector<Vector3>& path)  // ������ġ ��ã�� ��û �ޱ�
	{ PathFinder->GetPath(start_pos, GetRandomPos(), path); }
	Vector3& GetRandomPos(); // ������ġ �ޱ�
	
	Vector3 StartPos = Vector3::zero();

	vector<BoxCollider*>& GetCollisions() { return collisions; }
	BoxCollider*& GetEscapeCollision() { return escapeCollision; }

	void CalcWallCollision(Transform* OUT target, SphereCollider* IN targetCollision);
	bool CollisionObstacle(Ray const& ray, float const& distance);
};