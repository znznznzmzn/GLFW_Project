#pragma once

class Scene_ModelExport : public Scene {
	Model* model;

public:
	Scene_ModelExport();
	~Scene_ModelExport();
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};