#pragma once

class Transform {
protected:
	Transformation local;
	Transformation global;

	Vector3 pivot = Vector3(0.0f, 0.0f, 0.0f);
	Matrix  world = Matrix(1);
	Transform* parentTransform = nullptr;
	Matrix* parent = nullptr;

	string tag = "Unknown";
	bool is_active = true;
public:
	Transform() { }
	~Transform() { SAFE_DELETE(parent); }

	virtual void UpdateWorld();
	virtual void GUIRender();

	void NullParent() {
		parentTransform = nullptr;
		parent = nullptr;
	}
	void SetParent(Transform* transform) {
		parentTransform = transform;
		parent = &transform->GetWorld();
	}
	void SetParent(Matrix* matrix) { parent = matrix; }
	Transform* GetParent() { return parentTransform; }

	Vector3 Forward();
	Vector3 Up();
	Vector3 Right();

	Vector3 GlobalPosition() { return global.Position; }
	Vector3 GlobalRotation() { return global.Rotation; }
	Vector3 GlobalScale   () { return global.Scale   ; }

	Matrix& GetWorld() { return world; }

	Vector3& Position() { return local.Position; }
	Vector3& Rotation() { return local.Rotation; }
	Vector3& Scale   ()	{ return local.Scale   ; }

	void SetLocal(const Matrix& localMatrix) { local.Decompose(localMatrix); } // Matrix로 local 구하기

	string& Tag() { return tag; }

	virtual void SetActive(bool isActive) { this->is_active = isActive; }
	bool ActiveSelf() { return is_active; }
	bool Active();

	void SetTransformation(Transformation const& value) { local = value; }

	void Save(string path = "");
	void Load(string path = "");
	void SaveAs();
	void LoadFrom();
};