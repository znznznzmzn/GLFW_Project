#include "../Framework.h"
#include "Scene_Basic.h"

Scene_Basic::Scene_Basic() {
	obj = new Cube();
	weather = new Rain();
	weather->Play(Vector3(0, 0, 0));
}

Scene_Basic::~Scene_Basic() {
	SAFE_T_DELETE(weather);
	SAFE_T_DELETE(obj);
}

void Scene_Basic::Update() {
	obj->UpdateWorld();
	weather->Update();
}
void Scene_Basic::PreRender() {
}
void Scene_Basic::Render() {
	obj->Render();
	weather->Render();
}
void Scene_Basic::PostRender() {
}

void Scene_Basic::GUIRender() {
	obj->GUIRender();
	weather->GUIRender();
}
