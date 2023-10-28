#pragma once

class Instance {
protected:
	bool is_active = true;

	string tag = "Unknown";

	uint instance_count;

	uint matrix_buffer_id = -1;
	vector<Matrix> matrices;

	vector<TransformPointer*> controllers;
	void ClearController();

	void CreateInstance();
public:
	Instance(uint instanceCount = 100) { instance_count = instanceCount; };
	~Instance() { ClearController(); };

	void Arrange_Flat(float gap = 1, Vector3 position = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1)); // instance_count에 따라 정렬
	void Arrange_3D(float gap = 1, Vector3 position = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1)); // instance_count에 따라 3D로 정렬

	void Arrange_RandomArea(Vector3 position = Vector3(0, 0, 0), Vector3 area_size = Vector3(5, 5, 5)); // 위치(position)에서 범위(area_size[Box]) 안에 랜덤으로 정렬
	void Arrange_RandomRange(Vector3 position = Vector3(0, 0, 0), float range = 5); // 위치(position)에서 범위(range[Sphere]) 안에 랜덤으로 정렬

	virtual void Render() = 0; // 그리기
	virtual void UpdateWorld(int index = -1); // -1 == matrices 전부 UpdateWorld 시키기
	//virtual void GUIRender(uint index); // 종류별로 다를 수 있음
	virtual void GUIRender_Instance(uint index); // 하나의 Instance에 대해 GUI렌더

	virtual void SetActive(bool isActive) { this->is_active = isActive; }
	bool IsActive() { return is_active; }

	string& Tag() { return tag; }

	void Save(); //- [vector<Matrix> matrices]를 세이브로드
	void Load();
	//- 확장자명 정하기 
	// .mats(마테리얼이랑 겹침), 
	// .srts(pos,rot,scl형태로 저장해서 좀 않맞음)
	// 추천좀
};
