#include "../Framework.h"

SceneManager::SceneManager () { }
SceneManager::~SceneManager() { for (pair<string, Scene*> elem : scenes) { SAFE_DELETE(elem.second); } }

void SceneManager::Update    () { for (Scene* elem : activeScenes) elem->Update    (); }
void SceneManager::PreRender () { for (Scene* elem : activeScenes) elem->PreRender (); }
void SceneManager::Render    () { for (Scene* elem : activeScenes) elem->Render    (); }
void SceneManager::PostRender() { for (Scene* elem : activeScenes) elem->PostRender(); }
void SceneManager::GUIRender () { for (Scene* elem : activeScenes) elem->GUIRender (); }

void SceneManager::Create(string key, Scene* scene) {
	if (scenes.count(key) > 0) return;
	scenes[key] = scene;
}

void SceneManager::Add(string key) {
	if (scenes.count(key) == 0) return;
	for (Scene* elem : activeScenes) if (scenes[key] == elem) return;
	activeScenes.push_back(scenes[key]);
	activeScenes.back()->Start();
}

void SceneManager::Remove(string key) {
	for (uint i = 0; i < activeScenes.size(); i) {
		if (activeScenes[i] == scenes[key]) {
			scenes[key]->End();
			activeScenes.erase(activeScenes.begin() + i);
			return;
		}
	}
}
