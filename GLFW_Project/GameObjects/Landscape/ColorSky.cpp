#include "../Framework.h"

void ColorSky::CreateMesh(int slice, int stack, float radius) {
	float phiStep = Calc::PI / stack;
	float thetaStep = Calc::PI2 / slice;
	//Vertices
	vertices.clear();
	for (int i = 0; i <= stack; i++) {
		float phi = i * phiStep;
		for (int j = 0; j <= slice; j++) {
			float theta = j * thetaStep;
			Vector3 pos = Vector3(0, 0, 0);
			pos.x = sin(phi) * cos(theta); // normal
			pos.y = cos(phi);
			pos.z = sin(phi) * sin(theta);
			vertices.push_back(pos * radius);
		}
	}
	indices.clear();
	for (int i = 0; i < stack; i++) { // ¹Ý´ë¹æÇâÀ¸·Î ÀÎµ¦½Ì (Reversed Sphere)
		for (int j = 0; j < slice; j++) {
			indices.push_back((slice + 1) *  i + j);	  //0
			indices.push_back((slice + 1) * (i + 1) + j); //2
			indices.push_back((slice + 1) *  i + j + 1);  //1

			indices.push_back((slice + 1) * (i + 1) + j);	  //2
			indices.push_back((slice + 1) * (i + 1) + j + 1); //3
			indices.push_back((slice + 1) *  i + j + 1);	  //1
		}
	}

	SAFE_DELETE(mesh);
	mesh = new PositionMesh(vertices, indices);

	index_count = indices.size();
}

ColorSky::ColorSky(int slice, int stack, float radius) {
	skyShader = ShaderProgram::Create(
		"Assets/GLSL/Landscape/Sky.vert", 
		"Assets/GLSL/Landscape/SkyColor.frag");
	CreateMesh(slice, stack, radius);
	skyColorBuffer = new SkyColorBuffer();
	skyColorBuffer->Bind(skyShader->GetProgramID());
}

ColorSky::~ColorSky() {
	SAFE_DELETE(skyColorBuffer);
	SAFE_DELETE(mesh);
}

void ColorSky::Render() {
	if (!isActive) return;
	skyShader->Use();
	skyColorBuffer->Set();
	glDepthMask(GL_FALSE);
	mesh->Set();
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, (void*)0);
	glDepthMask(GL_TRUE);
}

void ColorSky::GUIRender() {
	if (ImGui::TreeNode(name.c_str())) {
		ImGui::Checkbox("isActive", &isActive);
		char name_buf[128] = "";
		if (ImGui::InputTextWithHint("SkyName", name.c_str(), name_buf, IM_ARRAYSIZE(name_buf)))
			if (KEY_DOWN(ImGuiKey_Enter)) name = string(name_buf);
		ImGui::ColorEdit3("Center", (float*)&skyColorBuffer->data.centerColor);
		ImGui::ColorEdit3("ApexColor", (float*)&skyColorBuffer->data.apexColor);
		ImGui::DragFloat ("Height", (float*)&skyColorBuffer->data.height);
		ImGui::TreePop();
	}
}
