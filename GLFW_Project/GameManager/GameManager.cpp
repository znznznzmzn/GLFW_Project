#include "../Framework.h"

#include "../Scenes/Scene_Grid.h"
#include "../Scenes/Scene_Basic.h"
#include "../Scenes/Scene_Collision.h"
#include "../Scenes/Scene_Terrain.h"
#include "../Scenes/Scene_Model.h"
#include "../Scenes/Scene_ModelExport.h"

GameManager::GameManager() {
	Create(); // 환경 및 텍스쳐 초기 세팅

	// 씬 생성
	SCENE->Create("Grid", new Scene_Grid());
	//SCENE->Create("Scene", new Scene_Basic());
	//SCENE->Create("Scene", new Scene_Terrain());
	//SCENE->Create("Scene", new Scene_Collision());
	SCENE->Create("Scene", new Scene_Model());
	//SCENE->Create("Scene", new Scene_ModelExport());

	SCENE->Add("Grid");
	SCENE->Add("Scene");

	// GlobalBuffer가(View, Projection 등) 모든 쉐이더에 Bind되도록 설정
	for (auto elem : Shader::GetShaders()) {
		MAIN_CAMERA->GetViewBuffer()->Bind(elem.second->Get_ProgramID());
		MAIN_CAMERA->GetProjectionBuffer()->Bind(elem.second->Get_ProgramID());
		if (elem.second->Get_Key().find("Grid"    ) == string::npos && 
			elem.second->Get_Key().find("Collider") == string::npos)
			MAIN_LIGHT->Bind(elem.second->Get_ProgramID());
	}
}
GameManager::~GameManager() { Delete(); }

void GameManager::Update() {
	ENVIRONMENT->Update();
	SCENE->Update();
}

void GameManager::Render() {
	SCENE->PreRender();
	glClearColor(FLOAT4_CLEAR_COLOR); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	ENVIRONMENT->Set(); 
	SCENE->Render();
	ENVIRONMENT->PostSet();
	SCENE->PostRender();
	ImGui_ImplOpenGL3_NewFrame(); 
	ImGui_ImplGlfw_NewFrame(); 
	ImGui::NewFrame();
	if (KEY_DOWN(ImGuiKey_F1)) EditorMode = !EditorMode;
	if (EditorMode) {
		ImGui::Begin("Inspector", &EditorMode);
		ENVIRONMENT->GUIRender();
		SCENE->GUIRender();
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GameManager::Create() {
	Environment::Get();
	Texture::Start();
}

void GameManager::Delete() {
	SceneManager::Delete();

	Environment::Delete();
	Shader ::Clear();
	Texture::Clear();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
