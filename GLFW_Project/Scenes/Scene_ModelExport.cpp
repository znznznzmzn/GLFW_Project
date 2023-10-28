#include "../Framework.h"
#include "Scene_ModelExport.h"

Scene_ModelExport::Scene_ModelExport() {
	ModelExporter* exporter = new ModelExporter();

	string model_name = "Knife";
	 // Model
	exporter->ExportModel(model_name);

	 // Anim
	//exporter->ExportClip("HappyIdle");
	//exporter->ExportClip("DancingTwerk");

	model = new Model(model_name);
	//model->ReadClip("HappyIdle");
	//model->ReadClip("DancingTwerk");
	//model->PlayClip(0);
	//model->Load();

	SAFE_DELETE(exporter);
}
Scene_ModelExport::~Scene_ModelExport() {
	SAFE_DELETE(model);
}

void Scene_ModelExport::Update() {
	//model->UpdateFrame();
	model->UpdateWorld();
}

void Scene_ModelExport::PreRender() {
}
void Scene_ModelExport::Render() {
	model->Render();
}
void Scene_ModelExport::PostRender() {
}

void Scene_ModelExport::GUIRender() {
	model->GUIRender();
}
