#pragma once
class Environment : public Singleton<Environment> {
private:
	void CreateState();

	Camera* main_camera;
	GlobalLightBuffer* main_light;

	Tween* main_tween;
public:
	Environment();
	~Environment();

	void Update();

	void Set();
	void PostSet();

	void GUIRender();

	Camera*& GetMainCamera() { return main_camera; }
	GlobalLightBuffer*& GetMainLight() { return main_light; }

	Tween*& GetMainTween() { return main_tween; }
};