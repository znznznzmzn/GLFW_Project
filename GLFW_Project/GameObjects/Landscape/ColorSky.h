#pragma once

class ColorSky {
private:
	vector<Vector3> vertices;
	vector<uint> indices;
	uint index_count = 0;
protected:
	string name = "Sky_00";
	bool isActive = true;

	class SkyColorBuffer : public GlobalBuffer {
	public: 
		struct Data {
			Vector3 centerColor = Vector3(0.3f, 0.3f, 0.3f);
			float height = 50.0f;
			Vector3 apexColor = Vector3(0.0f, 0.4f, 0.9f);
			float padding = 0.0f;
		} data;
		SkyColorBuffer() : GlobalBuffer("SkyColor", 4) {
			glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(data), NULL, GL_STATIC_DRAW);
			glBindBufferRange(GL_UNIFORM_BUFFER, buffer_slot, buffer_id, 0, sizeof(data));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
		void Set() override {
			glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data), &data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	} *skyColorBuffer;
	Shader* skyShader;
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