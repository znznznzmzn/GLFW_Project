#include "Framework.h"
#include "Framework.h"

SkyBox::SkyBox(wstring dds_file) {
	sphere = new Sphere_Reversed();
	sphere->GetMaterial()->SetShader(L"Shaders/Landscape/SkyBox.hlsl");

	skyBox = Texture::Add(dds_file);

	dsv[0] = new DepthStencilState();
	dsv[1] = new DepthStencilState();
	dsv[1]->DepthEnable(false);
}

SkyBox::~SkyBox() {
	SAFE_DELETE(sphere);
	SAFE_DELETE(dsv[0]);
	SAFE_DELETE(dsv[1]);
}

void SkyBox::Render() {
	if(!Active()) return;
	skyBox->PSSet(10);
	dsv[1]->SetState();
	sphere->Render();
	dsv[0]->SetState();
}