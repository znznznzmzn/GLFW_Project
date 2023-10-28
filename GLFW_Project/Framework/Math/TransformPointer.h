#pragma once

class TransformPointer { // targetWorld를 조종한다 (인스턴스에 사용할 Transform)
private:
	Matrix* targetWorld = nullptr;
protected:
	Transformation local;
	Transformation global;

	Vector3 pivot = Vector3(0.0f, 0.0f, 0.0f);
	Matrix* parent = nullptr;
	TransformPointer* parentTransform = nullptr;

	bool is_active = true;
public:
	TransformPointer(Matrix* target = nullptr) { targetWorld = target; }
	~TransformPointer() { NullParent(); }

	virtual void UpdateWorld();
	virtual void GUIRender_Local () { local .GUIRender(); }
	virtual void GUIRender_Global() { global.GUIRender(); }

	void NullParent() {
		parentTransform = nullptr;
		parent = nullptr;
	}
	void SetParent(TransformPointer* transform) {
		parentTransform = transform;
		parent = &transform->GetWorld();
	}
	void SetParent(Matrix* matrix) { parent = matrix; }
	TransformPointer* GetParent() { return parentTransform; }

	Vector3 Forward();
	Vector3 Up();
	Vector3 Right();

	Vector3 GlobalPosition() { return global.Position; }
	Vector3 GlobalRotation() { return global.Rotation; }
	Vector3 GlobalScale   () { return global.Scale; }

	Matrix& GetWorld() { return *targetWorld; }

	Vector3& Position() { return local.Position; }
	Vector3& Rotation() { return local.Rotation; }
	Vector3& Scale   () { return local.Scale; }

	void SetLocal(const Matrix& localMatrix) { local.Decompose(localMatrix); } // Matrix로 local 구하기

	virtual void SetActive(bool isActive) { this->is_active = isActive; }
	bool ActiveSelf() { return is_active; }
	bool Active();

	void SetTransformation(Transformation const& value) { local = value; }

	void Register(Matrix* target) { targetWorld = target; }
};