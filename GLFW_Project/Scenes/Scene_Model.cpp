#include "../Framework.h"
#include "Scene_Model.h"

Scene_Model::Scene_Model() {
	model = new ModelBone("PotMan");
	//model->ReadClip("DancingTwerk", 0);
	//model->ReadClip("HappyIdle", 0);
	//model->PlayClip(0);
	model->Load();
}
Scene_Model::~Scene_Model() {
	SAFE_DELETE(model);
}

void Scene_Model::Update() {
	//model->UpdateFrame();
	model->UpdateWorld();
}
void Scene_Model::PreRender() {
}
void Scene_Model::Render() {
	model->Render();
}
void Scene_Model::PostRender() {
}
void Scene_Model::GUIRender() {
	model->GUIRender();
}
