#pragma once

class Snow : public Particle {
private:
	const WeatherBuffer::Data INITIAL_DATA = {
		Vector3(0.0f, -1.0f, 0.0f),      // velocity
		1000.0f,                         // drawDistance
		Vector4(1.0f, 1.0f, 1.0f, 1.0f), // particleColor
		Vector3(0.0f, 0.0f, 0.0f),       // origin
		0.0f,                            // time
		Vector3(50.0f, 50.0f, 50.0f),    // size
		1.0f                             // turbulence
	};
private:
	WeatherBuffer* snowBuffer = nullptr;
	struct VertexSnow {
		Vector3 pos;
		Vector2 size;
		Vector2 random;
	};
	vector<VertexSnow> vertices;

	Texture* snowTexture = nullptr;
	TextureBuffer* snowMapBuffer = nullptr;

	void Create();

	void SetUniform() override;
public:
	Snow();
	~Snow();

	virtual void Update() override;
	virtual void GUIRender() override;

	void SetTexture(string path) { snowTexture = Texture::Load(path); }
	string GetTexturePath() { return (snowTexture)? snowTexture->GetPath() : ""; }
};