#pragma once

class Scene_Basic : public Scene {
private:
	GameObject* obj;
public:
	Scene_Basic();
	~Scene_Basic();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};