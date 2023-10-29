#pragma once
class Scene_GUI : public Scene {
public:
	Scene_GUI();
	~Scene_GUI();
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};