#pragma once

class Rain : public Particle {
private:
	const WeatherBuffer::Data INITIAL_DATA = {
		Vector3(0.0f, -50.0f, 0.0f),    // velocity
		1000.0f,                         // drawDistance
		Vector4(1.0f, 1.0f, 1.0f, 1.0f), // particleColor
		Vector3(0.0f, 0.0f, 0.0f),       // origin
		0.0f,                            // time
		Vector3(50, 50, 50),             // size
		0.0f                             // turbulence (Rain에선 padding, 다른 데이터 환영)
	};
private:
	WeatherBuffer* rainBuffer;

	vector<VertexSize> vertices;

	Vector2 minSize = { 0.02f, 0.8f };
	Vector2 maxSize = { 0.4f, 1.6f };

	Texture* rainTexture = nullptr;
	TextureBuffer* rainMapBuffer = nullptr;

	virtual void SetUniform() override;
	void Create();
public:
	Rain();
	~Rain();

	virtual void Update() override;
	virtual void GUIRender() override;
};