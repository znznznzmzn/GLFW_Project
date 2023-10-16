#include "../Framework.h"
#include "Scene_Basic.h"

Scene_Basic::Scene_Basic() {
	obj = new Cube();
	cube_map = new CubeMap();
	cube_map->SetTexture("Assets/Textures/Skybox/top.jpg"   , CubeMap::Direction::Up);
	cube_map->SetTexture("Assets/Textures/Skybox/bottom.jpg", CubeMap::Direction::Down);
	cube_map->SetTexture("Assets/Textures/Skybox/front.jpg" , CubeMap::Direction::Forward);
	cube_map->SetTexture("Assets/Textures/Skybox/back.jpg"  , CubeMap::Direction::Back);
	cube_map->SetTexture("Assets/Textures/Skybox/left.jpg"  , CubeMap::Direction::Left);
	cube_map->SetTexture("Assets/Textures/Skybox/right.jpg" , CubeMap::Direction::Right);
	//cube_map->SetActive(false);

	//color_sky = new ColorSky();
}

Scene_Basic::~Scene_Basic() {
	SAFE_T_DELETE(obj);
	SAFE_DELETE(cube_map);
	//SAFE_DELETE(color_sky);
}

void Scene_Basic::Update() {
	obj->UpdateWorld();
}
void Scene_Basic::PreRender() {
}
void Scene_Basic::Render() {
	cube_map->Render();
	//color_sky->Render();
	obj->Render();
}
void Scene_Basic::PostRender() {
}

void Scene_Basic::GUIRender() {
	cube_map->GUIRender();
	//color_sky->GUIRender();
	obj->GUIRender();
}
