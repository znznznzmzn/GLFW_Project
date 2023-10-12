#pragma once

class Scene_Collision : public Scene {
private:
	float dist = 0;
	Ray r;
	BoxCollider* collider1;
	BoxCollider* collider2;

	BoxCollider* indicator[3];

	vector<Vector3> v;
public:
	Scene_Collision();
	~Scene_Collision();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};