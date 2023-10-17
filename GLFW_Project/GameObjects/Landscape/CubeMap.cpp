#include "../Framework.h"

void CubeMap::CreateMesh(int slice, int stack) {
	const float radius = 0.5f;
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
	for (int i = 0; i < stack; i++) { // 반대방향으로 인덱싱 (Reversed Sphere)
		for (int j = 0; j < slice; j++) {
			indices.push_back((slice + 1) * i + j);	  //0
			indices.push_back((slice + 1) * (i + 1) + j); //2
			indices.push_back((slice + 1) * i + j + 1);  //1

			indices.push_back((slice + 1) * (i + 1) + j);	  //2
			indices.push_back((slice + 1) * (i + 1) + j + 1); //3
			indices.push_back((slice + 1) * i + j + 1);	  //1
		}
	}

	SAFE_DELETE(mesh);
	mesh = new PositionMesh(vertices, indices);

	index_count = indices.size();
}

CubeMap::CubeMap(int slice, int stack) {
	skyShader = ShaderProgram::Create("Assets/GLSL/Sky.vert", "Assets/GLSL/SkyCube.frag");
	CreateMesh(slice, stack);
	cubeMapBuffer = new TextureBuffer(skyShader->GetProgramID(), "cubeMap");
	cubeMapBuffer->slot = 0;
	glGenTextures(1, &cubemap_texture_id);
}

CubeMap::~CubeMap() {
	SAFE_DELETE(cubeMapBuffer);
	SAFE_DELETE(mesh);
}

void CubeMap::Render() {
	if (!isActive) return;
	skyShader->Use();
	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture_id);
	cubeMapBuffer->Set();
	mesh->Set();
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, (void*)0);
	glDepthMask(GL_TRUE);
}

void CubeMap::GUIRender() {
	if (ImGui::TreeNode(name.c_str())) {
		ImGui::Checkbox("isActive", &isActive);
		char name_buf[128] = "";
		if (ImGui::InputTextWithHint("SkyName", name.c_str(), name_buf, IM_ARRAYSIZE(name_buf)))
			if (KEY_DOWN(ImGuiKey_Enter)) name = string(name_buf);
		//- 각 방향별 이미지 로드
		ImGui::TreePop();
	}
}

void CubeMap::SetTexture(Texture* texture, GLenum Direction) {
	GLubyte* raw_data = texture->GetRawData();
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture_id);
	glTexImage2D(Direction, 0, 
		GL_RGB,
		texture->Width (), 
		texture->Height(), 0, 
		GL_RGB, GL_UNSIGNED_BYTE, raw_data);

	// GL_TEXTURE_CUBE_MAP
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // 3D
	SAFE_DELETE(raw_data);
}
