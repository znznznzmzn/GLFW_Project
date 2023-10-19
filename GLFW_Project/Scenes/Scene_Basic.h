#pragma once

class Scene_Basic : public Scene {
private:
	Cube* obj;
	Rain* weather;
public:
	Scene_Basic();
	~Scene_Basic();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};