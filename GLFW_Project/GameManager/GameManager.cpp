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
	SCENE->Create("Scene", new Scene_Collision());
	//SCENE->Create("Scene", new Scene_Model());
	//SCENE->Create("Scene", new Scene_ModelExport());

	SCENE->Add("Grid");
	SCENE->Add("Scene");

	// GlobalBuffer가(View, Projection 등) 모든 쉐이더에 Bind되도록 설정
	ShaderProgram::BindAll(MAIN_CAMERA->GetViewBuffer());
	ShaderProgram::BindAll(MAIN_CAMERA->GetProjectionBuffer());
	ShaderProgram::BindAll(MAIN_LIGHT); //- 나중에 조건부 BindAll 제작
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
	// if (KEY_DOWN(ImGuiKey_F2)) ; //- 쉐이더 리소스 관리
	// if (KEY_DOWN(ImGuiKey_F3)) ; //- 텍스처 리소스 관리
	// if (KEY_DOWN(ImGuiKey_F4)) ; //- 사운드 리소스 관리
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GameManager::Create() {
	glfwSwapInterval(1); // SwapChain

	Environment::Get();
	Texture::Init();
	Audio::Init();
}

void GameManager::Delete() {
	SceneManager::Delete();

	Environment::Delete();
	Shader::Clear();
	ShaderProgram::Clear();
	Texture::Clear();
	Audio::Clear();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
