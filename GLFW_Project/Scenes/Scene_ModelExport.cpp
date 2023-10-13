#include "../Framework.h"
#include "Scene_ModelExport.h"

Scene_ModelExport::Scene_ModelExport() {
	string model_name = "Archer";
	 // Model
	//ModelExporter* exporter = new ModelExporter();
	//exporter->ExportModel(model_name);
	//SAFE_DELETE(exporter);

	 // Anim
	//string anim_name = "HappyIdle";
	//ModelExporter* exporter = new ModelExporter();
	//exporter->ExportClip("DancingTwerk");
	//SAFE_DELETE(exporter);

	model = new ModelAnimator(model_name);
	//model->ReadClip(anim_name);
}
Scene_ModelExport::~Scene_ModelExport() {
	SAFE_DELETE(model);
}

void Scene_ModelExport::Update() {
	model->UpdateFrame();
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
