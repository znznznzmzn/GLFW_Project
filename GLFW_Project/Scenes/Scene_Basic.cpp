#include "../Framework.h"
#include "Scene_Basic.h"

Scene_Basic::Scene_Basic() {
	obj = new Cube();
	instance = new ModelBoneInstance("PotMan", 16);
	instance->Arrange_Flat(2.0f, Vector3(0, 0, 0), Vector3(0.01f, 0.01f, 0.01f));
}

Scene_Basic::~Scene_Basic() {
	SAFE_DELETE(instance);
	SAFE_T_DELETE(obj);
}

void Scene_Basic::Update() {
	obj->UpdateWorld();
	//instance->UpdateWorld();
}
void Scene_Basic::PreRender() {
}
void Scene_Basic::Render() {
	obj->Render();
	instance->Render();
}
void Scene_Basic::PostRender() {
}

void Scene_Basic::GUIRender() {
	obj->GUIRender();
	instance->GUIRender();
}
