#pragma once

class Transform : public TransformPointer { // �Ϲ����� ��Ȳ���� ����ϴ� Transform
protected:
	Matrix world = Matrix(1);
	string tag = "Unknown";
public:
	Transform() : TransformPointer(&world) {  }
	~Transform() = default;

	virtual void GUIRender();

	void Save(string path = "");
	void Load(string path = "");
	void SaveAs();
	void LoadFrom();

	string& Tag() { return tag; }
};
