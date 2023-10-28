#pragma once

class Transform : public TransformPointer { // 일반적인 상황에서 사용하는 Transform
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
