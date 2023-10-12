#include "Framework.h"

Sky::Sky() {
	sphere = new Sphere_Reversed();
	sphere->GetMaterial()->SetShader(L"Shaders/Landscape/Sky.hlsl");

	skyBuffer = new SkyBuffer();

	dsv[0] = new DepthStencilState();
	dsv[1] = new DepthStencilState();
	dsv[1]->DepthEnable(false);
}

Sky::~Sky() {
	SAFE_DELETE(sphere);
	SAFE_DELETE(skyBuffer);
	SAFE_DELETE(dsv[0]);
	SAFE_DELETE(dsv[1]);
}

void Sky::Render() {
	skyBuffer->SetPSBuffer(10);
	dsv[1]->SetState();
	sphere->Render();
	dsv[0]->SetState();
}

void Sky::GUIRender() {
	ImGui::ColorEdit3("Center", (float*)&skyBuffer->data.centerColor);
	ImGui::ColorEdit3("ApexColor", (float*)&skyBuffer->data.apexColor);
	ImGui::DragFloat("Height", (float*)&skyBuffer->data.height);
}
