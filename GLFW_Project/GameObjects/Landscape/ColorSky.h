#pragma once

class ColorSky {
private:
	vector<Vector3> vertices;
	vector<uint> indices;
	uint index_count = 0;
protected:
	string name = "Sky_00";
	bool isActive = true;

	SkyColorBuffer *skyColorBuffer;
	ShaderProgram* skyShader;
	PositionMesh* mesh;

	void CreateMesh(int slice, int stack, float radius);
public:
	ColorSky(int slice = 15, int stack = 15, float radius = 0.5f);
	~ColorSky();

	void Render();
	void GUIRender();

	void SetActive(bool will_active) { isActive = will_active; }
	bool IsActive() { return isActive; }
};