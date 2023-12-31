#pragma once
class SphereCollider : public Collider {
public:
	SphereCollider();
	virtual bool RayCollision(Ray ray, Contact* contact = nullptr) override;
	virtual bool BoxCollision(BoxCollider* collider) override;
	virtual bool SphereCollision(SphereCollider* collider) override;
	virtual bool CapsuleCollision(CapsuleCollider* collider) override;
	float Radius() { return max(GlobalScale().x, max(GlobalScale().y, GlobalScale().z)); };
};

