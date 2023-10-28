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

	void Arrange_Flat(float gap = 1, Vector3 position = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1)); // instance_count�� ���� ����
	void Arrange_3D(float gap = 1, Vector3 position = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1)); // instance_count�� ���� 3D�� ����

	void Arrange_RandomArea(Vector3 position = Vector3(0, 0, 0), Vector3 area_size = Vector3(5, 5, 5)); // ��ġ(position)���� ����(area_size[Box]) �ȿ� �������� ����
	void Arrange_RandomRange(Vector3 position = Vector3(0, 0, 0), float range = 5); // ��ġ(position)���� ����(range[Sphere]) �ȿ� �������� ����

	virtual void Render() = 0; // �׸���
	virtual void UpdateWorld(int index = -1); // -1 == matrices ���� UpdateWorld ��Ű��
	//virtual void GUIRender(uint index); // �������� �ٸ� �� ����
	virtual void GUIRender_Instance(uint index); // �ϳ��� Instance�� ���� GUI����

	virtual void SetActive(bool isActive) { this->is_active = isActive; }
	bool IsActive() { return is_active; }

	string& Tag() { return tag; }

	void Save(); //- [vector<Matrix> matrices]�� ���̺�ε�
	void Load();
	//- Ȯ���ڸ� ���ϱ� 
	// .mats(���׸����̶� ��ħ), 
	// .srts(pos,rot,scl���·� �����ؼ� �� �ʸ���)
	// ��õ��
};
