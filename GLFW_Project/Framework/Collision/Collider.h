#pragma once

class BoxCollider;
class SphereCollider;
class CapsuleCollider;

class Collider : public Transform {
private:
	ShaderProgram* colliderShader;
	MatrixBuffer* wBuffer = nullptr;
	ColorBuffer* collider_colorBuffer;
	PositionMesh* mesh;
protected:
	enum class TYPE : uint { POINT, SPHERE, CAPSULE, BOX } collider_type = TYPE::POINT;

	vector<Vector3> vertices;
	vector<uint> indices;

	void CreateMesh();
public:
	Collider(TYPE collider_type);
	~Collider();

	void SetActive(bool isActive) override;

	bool Collision(Collider* collider);

	virtual bool RayCollision(Ray ray, Contact* contact = nullptr) = 0;
	virtual bool BoxCollision    (BoxCollider    * collider) = 0;
	virtual bool SphereCollision (SphereCollider * collider) = 0;
	virtual bool CapsuleCollision(CapsuleCollider* collider) = 0;

	void SetColor(Vector4 color) { collider_colorBuffer->data = color; }
	void SetColor(float r, float g, float b) { SetColor({ r, g, b, 1.0f }); }
	
	void Render();

	static Vector4 COLOR_NORMAL;
	static Vector4 COLOR_COLLIDE;
	static Vector4 COLOR_DISABLED;
};