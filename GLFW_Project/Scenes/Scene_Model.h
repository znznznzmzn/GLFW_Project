#pragma once

class Scene_Model : public Scene {
	ModelBone* model;
public:
	Scene_Model();
	~Scene_Model();
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};