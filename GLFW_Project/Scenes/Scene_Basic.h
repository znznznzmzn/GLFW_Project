#pragma once

class Scene_Basic : public Scene {
private:
	GameObject* obj;
public:
	Scene_Basic();
	~Scene_Basic();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};