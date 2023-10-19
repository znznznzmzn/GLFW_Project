#include "../Framework.h"

void Snow::Create() {
	vertices.clear();
	vertices.resize(MAX_COUNT);

	for (uint i = 0; i < particleCount; i++) {
		vertices[i].size.x = Random::Range(0.1f, 0.4f);
		vertices[i].size.y = vertices[i].size.x;

		vertices[i].pos.x = Random::Range(-snowBuffer->data.size.x, +snowBuffer->data.size.x);
		vertices[i].pos.y = Random::Range(-snowBuffer->data.size.y, +snowBuffer->data.size.y);
		vertices[i].pos.z = Random::Range(-snowBuffer->data.size.z, +snowBuffer->data.size.z);

		vertices[i].random.x = Random::Range(0.0f, 1.0f);
		vertices[i].random.y = Random::Range(0.0f, 1.0f);
	}

	SAFE_DELETE(vBuffer);
	vector<uint> attributions;
	attributions.push_back(sizeof(Vector3));
	attributions.push_back(sizeof(Vector2));
	attributions.push_back(sizeof(Vector2));
	vBuffer = new VertexBuffer(vertices.data(), MAX_COUNT, attributions);
}

void Snow::SetUniform() {
	snowBuffer->Set();
	snowTexture->Set(snowMapBuffer->slot);
	snowMapBuffer->Set();
	vBuffer->Set();
	// Particle::SetUniform(); // world.position == origin
}

Snow::Snow() : Particle() {
	particleProgram = ShaderProgram::Create(
		"Assets/GLSL/Particle/Snow.vert",
		"Assets/GLSL/Particle/Snow.frag", 
		"Assets/GLSL/Particle/Snow.geom");
	wBuffer = new WorldBuffer(particleProgram->GetProgramID());
	snowTexture = Texture::Load("Assets/Textures/FX/Snow.png");
	snowMapBuffer = new TextureBuffer(particleProgram->GetProgramID(), "snowMap");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	snowMapBuffer->slot = 0;
	snowBuffer = new WeatherBuffer("SnowBuffer");
	snowBuffer->data = INITIAL_DATA;
	particleProgram->Bind(snowBuffer);
	particleCount = MAX_COUNT;
	Create();
}

Snow::~Snow() {
	SAFE_DELETE(snowBuffer);
	SAFE_DELETE(snowMapBuffer);
}

void Snow::Update() { snowBuffer->data.time += DELTA; }

void Snow::GUIRender() {
	if (ImGui::TreeNode((tag + "_SnowEditor").c_str())) {
		ImGui::DragFloat3("Velocity", (float*)&snowBuffer->data.velocity);
		ImGui::DragFloat("Distance", &snowBuffer->data.drawDistance);
		ImGui::DragFloat3("Origin", (float*)&snowBuffer->data.origin);
		ImGui::DragFloat3("Size", (float*)&snowBuffer->data.size);
		ImGui::DragFloat("Turbulence", (float*)&snowBuffer->data.turbulence);
		ImGui::ColorEdit4("Snow Color", (float*)&snowBuffer->data.particleColor);
		ImGui::TreePop();
	}
}
