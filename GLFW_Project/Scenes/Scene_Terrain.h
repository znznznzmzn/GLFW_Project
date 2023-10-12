#pragma once
class Scene_Terrain : public Scene {
private:
	TerrainEditor* te;
	// Terrain* t;
public:
	Scene_Terrain();
	~Scene_Terrain();
	virtual void Update    () override;
	virtual void PreRender () override;
	virtual void Render    () override;
	virtual void PostRender() override;
	virtual void GUIRender () override;
};