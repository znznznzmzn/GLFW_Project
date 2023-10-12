#include "../Framework.h"
#include "Scene_Terrain.h"

Scene_Terrain::Scene_Terrain() {
	te = new TerrainEditor();
	te->UpdateWorld();
}
Scene_Terrain::~Scene_Terrain() {
	SAFE_T_DELETE(te);
}

void Scene_Terrain::Update() {
	te->Update();
}
void Scene_Terrain::PreRender() {
}
void Scene_Terrain::Render() {
	te->Render();
}
void Scene_Terrain::PostRender() {
}
void Scene_Terrain::GUIRender() {
	te->GUIRender();
}
