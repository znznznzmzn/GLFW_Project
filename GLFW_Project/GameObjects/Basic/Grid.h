#pragma once
class Grid {
private:
	const int MAX_VALUE = 100; 
	ShaderProgram* GridShader;
	MatrixBuffer* wBuffer;
	vector<VertexColor> vertices;

	uint width, height;

	uint VertexArrayBuffer = 0;
	uint VertexBuffer = 0;

public:
	Grid();
	~Grid();

	void Render();
};