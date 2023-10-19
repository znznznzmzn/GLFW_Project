#include "../Framework.h"

void Rain::Create() {
	vertices.clear();
	vertices.resize(MAX_COUNT);

	for (uint i = 0; i < particleCount; i++) {
		vertices[i].pos.x = Random::Range(-rainBuffer->data.size.x, +rainBuffer->data.size.x);
		vertices[i].pos.y = Random::Range(-rainBuffer->data.size.y, +rainBuffer->data.size.y);
		vertices[i].pos.z = Random::Range(-rainBuffer->data.size.z, +rainBuffer->data.size.z);

		vertices[i].size.x = Random::Range(minSize.x, maxSize.x);
		vertices[i].size.y = Random::Range(minSize.y, maxSize.y);
	}

	SAFE_DELETE(vBuffer);
	vector<uint> attributions;
	attributions.push_back(sizeof(Vector3));
	attributions.push_back(sizeof(Vector2));
	vBuffer = new VertexBuffer(vertices.data(), MAX_COUNT, attributions);
}

void Rain::SetUniform() {
	rainBuffer->Set();
	rainTexture->Set(rainMapBuffer->slot);
	rainMapBuffer->Set();
	vBuffer->Set();
	// Particle::SetUniform(); // world.position == origin
}

Rain::Rain() : Particle() {
	particleProgram = ShaderProgram::Create(
		"Assets/GLSL/Particle/Rain.vert",
		"Assets/GLSL/Particle/Rain.frag",
		"Assets/GLSL/Particle/Rain.geom");
	wBuffer = new WorldBuffer(particleProgram->GetProgramID());
	rainTexture = Texture::Load("Assets/Textures/FX/Rain.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	rainMapBuffer = new TextureBuffer(particleProgram->GetProgramID(), "rainMap");
	rainMapBuffer->slot = 0;
	rainBuffer = new WeatherBuffer("RainBuffer");
	rainBuffer->data = INITIAL_DATA;
	particleProgram->Bind(rainBuffer);
	particleCount = MAX_COUNT;
	Create();
}

Rain::~Rain() {
	SAFE_DELETE(rainBuffer);
	SAFE_DELETE(rainMapBuffer);
}

void Rain::Update() { rainBuffer->data.time += DELTA; }

void Rain::GUIRender() {
	if (ImGui::TreeNode((tag + "_RainEditor").c_str())) {
		ImGui::DragFloat3("Velocity", (float*)&rainBuffer->data.velocity);
		ImGui::DragFloat("Distance", &rainBuffer->data.drawDistance);
		ImGui::DragFloat3("Origin", (float*)&rainBuffer->data.origin);
		ImGui::DragFloat3("Range", (float*)&rainBuffer->data.size);
		ImGui::ColorEdit4("Color", (float*)&rainBuffer->data.particleColor);
		ImGui::DragFloat2("MinSize", (float*)&minSize, 0.01f);
		ImGui::DragFloat2("MaxSize", (float*)&maxSize, 0.01f);
		if (ImGui::Button("ResetSize")) Create();
		ImGui::TreePop();
	}
}
