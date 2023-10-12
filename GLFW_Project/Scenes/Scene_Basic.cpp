#include "../Framework.h"
#include "Scene_Basic.h"

Scene_Basic::Scene_Basic() {
	obj = new Cube();
}

Scene_Basic::~Scene_Basic() {
	SAFE_T_DELETE(obj);
}

void Scene_Basic::Update() {
	obj->UpdateWorld();
}
void Scene_Basic::PreRender() {
}
void Scene_Basic::Render() {
	obj->Render();
}
void Scene_Basic::PostRender() {
}

void Scene_Basic::GUIRender() {
	obj->GUIRender();
}
