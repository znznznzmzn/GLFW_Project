#pragma once

class Sky : public Transform {
private:
	class SkyBuffer : public ConstantBuffer {
	public: 
		struct Data {
			float3 centerColor = { 0.3f, 0.3f, 0.0f };
			float height = 5.0f;

			float3 apexColor = { 0.0f, 1.0f, 1.0f };
			float padding = 0.0f;
		} data;
		SkyBuffer() : ConstantBuffer(&data, sizeof(Data)) { }
	} *skyBuffer;
	Sphere_Reversed* sphere;
	DepthStencilState* dsv[2];
public:
	Sky();
	~Sky();

	void Render();
	void GUIRender();
};