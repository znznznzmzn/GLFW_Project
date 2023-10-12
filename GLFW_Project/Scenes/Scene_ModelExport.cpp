#include "../Framework.h"
#include "Scene_ModelExport.h"

Scene_ModelExport::Scene_ModelExport() {
	 // Model
	//string model_name = "PotMan";
	//ModelExporter* exporter = new ModelExporter();
	//exporter->ExportModel(model_name);
	//SAFE_DELETE(exporter);

	 // Anim
	string anim_name = "HappyIdle";
	string anim_path = "Assets/Models/ModelSources/Animations/" + anim_name + ".fbx";
	ModelExporter* exporter = new ModelExporter();
	exporter->ExportClip("DancingTwerk");
	exporter->ExportClip(anim_name, anim_path);
	SAFE_DELETE(exporter);

	model = new ModelAnimator("PotMan");
	model->ReadClip(anim_name);
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
