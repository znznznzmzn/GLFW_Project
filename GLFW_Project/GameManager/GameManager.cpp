#include "../Framework.h"

#include "../Scenes/Scene_Grid.h"
#include "../Scenes/Scene_Basic.h"
#include "../Scenes/Scene_Collision.h"
#include "../Scenes/Scene_Terrain.h"
#include "../Scenes/Scene_Model.h"
#include "../Scenes/Scene_ModelExport.h"

GameManager::GameManager() {
	Create(); // ȯ�� �� �ؽ��� �ʱ� ����

	// �� ����
	SCENE->Create("Grid", new Scene_Grid());
	//SCENE->Create("Scene", new Scene_Basic());
	//SCENE->Create("Scene", new Scene_Terrain());
	SCENE->Create("Scene", new Scene_Collision());
	//SCENE->Create("Scene", new Scene_Model());
	//SCENE->Create("Scene", new Scene_ModelExport());

	SCENE->Add("Grid");
	SCENE->Add("Scene");

	// GlobalBuffer��(View, Projection ��) ��� ���̴��� Bind�ǵ��� ����
	ShaderProgram::BindAll(MAIN_CAMERA->GetViewBuffer());
	ShaderProgram::BindAll(MAIN_CAMERA->GetProjectionBuffer());
	ShaderProgram::BindAll(MAIN_LIGHT); //- ���߿� ���Ǻ� BindAll ����
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
	// if (KEY_DOWN(ImGuiKey_F2)) ; //- ���̴� ���ҽ� ����
	// if (KEY_DOWN(ImGuiKey_F3)) ; //- �ؽ�ó ���ҽ� ����
	// if (KEY_DOWN(ImGuiKey_F4)) ; //- ���� ���ҽ� ����
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
