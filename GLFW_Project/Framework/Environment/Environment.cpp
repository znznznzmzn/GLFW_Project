#include "../Framework.h"

Environment::Environment() {
	CreateState();

	main_camera = new Camera();
	main_camera->Tag() = "MainCamera";
	main_camera->Load(); 
	main_camera->UpdateWorld();

	main_light = new GlobalLightBuffer();

	main_tween = new Tween();
}

Environment::~Environment() {
	main_camera->Save();
	SAFE_DELETE(main_camera);
	SAFE_DELETE(main_tween);
	SAFE_DELETE(main_light);
}

void Environment::Update() {
	main_tween->Update();
	main_camera->Update();
}

void Environment::Set() {
	main_light->Set();
	main_camera->SetView();
}

void Environment::PostSet() {
}

void Environment::GUIRender() {
	string fps = "FPS : " + to_string((int)ImGui::GetIO().Framerate);
	ImGui::Text(fps.c_str());

	if (ImGui::TreeNode("Main Light")) {
		ImGui::ColorEdit4("Light Color", (float*)&main_light->data.light_color);

		Vector4 degree = main_light->data.light_direction * 180.0f / PI_F;
		ImGui::DragFloat3("Light Direction", (float*)&degree, 0.1f);
		main_light->data.light_direction = degree * PI_F / 180.0f;

		ImGui::ColorEdit4("Ambient Light", (float*)&main_light->data.ambient_light);
		ImGui::ColorEdit4("Ambient Ceil" , (float*)&main_light->data.ambient_ceil);
		ImGui::TreePop();
	}

	main_camera->GUIRender();
}

void Environment::CreateState() {
	glEnable(GL_MULTISAMPLE); // MSAA

	glEnable(GL_DEPTH_TEST); // Depth State
	glEnable(GL_STENCIL_TEST); // Stencil State

	glEnable(GL_CULL_FACE); // Culling State
	glCullFace(GL_FRONT); // > GL_BACK
	glFrontFace(GL_CW); // > GL_CCW

	glEnable(GL_TEXTURE_2D_ARRAY); // Texture2DArray
	
	glEnable(GL_BLEND); // BlendState
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}