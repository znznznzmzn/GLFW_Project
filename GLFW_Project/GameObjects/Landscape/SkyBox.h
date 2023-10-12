#pragma once

class SkyBox : public Transform {
private:
	Sphere_Reversed* sphere;
	Texture* skyBox;
	DepthStencilState* dsv[2];
public:
	SkyBox(wstring dds_file);
	~SkyBox();

	void Render();
};