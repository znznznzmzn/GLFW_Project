#include "../Framework.h"

Grid::Grid() : width(MAX_VALUE), height(MAX_VALUE) {
	GridShader = ShaderProgram::Create("Assets/GLSL/Grid.vert", "Assets/GLSL/Grid.frag");
	GridShader->Use();

	//Axis X
	vertices.emplace_back(VertexColor(static_cast<float>(-MAX_VALUE), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));
	vertices.emplace_back(VertexColor(static_cast<float>(+MAX_VALUE), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));
	//Axis Y
	vertices.emplace_back(VertexColor(0.0f, static_cast<float>(-MAX_VALUE), 0.0f, 0.0f, 1.0f, 0.0f, 1.0f));
	vertices.emplace_back(VertexColor(0.0f, static_cast<float>(+MAX_VALUE), 0.0f, 0.0f, 1.0f, 0.0f, 1.0f));
	//Axis Z
	vertices.emplace_back(VertexColor(0.0f, 0.0f, static_cast<float>(-MAX_VALUE), 0.0f, 0.0f, 1.0f, 1.0f));
	vertices.emplace_back(VertexColor(0.0f, 0.0f, static_cast<float>(+MAX_VALUE), 0.0f, 0.0f, 1.0f, 1.0f));

	float halfW = width  / 2.0f;
	float halfH = height / 2.0f;

	for (float z = -halfH; z <= halfH; z += 1) {
		if (z == 0) continue;
		vertices.emplace_back(VertexColor(-halfH, 0.0f, z, 0.5f, 0.5f, 0.5f, 1.0f));
		vertices.emplace_back(VertexColor(+halfH, 0.0f, z, 0.5f, 0.5f, 0.5f, 1.0f));
	}
	for (float x = -halfW; x <= halfW; x += 1) {
		if (x == 0) continue;
		vertices.emplace_back(VertexColor(x, 0.0f, -halfW, 0.5f, 0.5f, 0.5f, 1.0f));
		vertices.emplace_back(VertexColor(x, 0.0f, +halfW, 0.5f, 0.5f, 0.5f, 1.0f));
	}

	glGenVertexArrays(1, &VertexArrayBuffer); 
	glGenBuffers(1, &VertexBuffer); 

	glBindVertexArray(VertexArrayBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexColor), vertices.data(), GL_STATIC_DRAW);

	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	wBuffer = new MatrixBuffer(GridShader->GetProgramID(), "world");
	wBuffer->data = Matrix(1.0f);
}

Grid::~Grid() {
	SAFE_DELETE(wBuffer);
	glDeleteVertexArrays(1, &VertexArrayBuffer);
	glDeleteBuffers(1, &VertexBuffer);
}

void Grid::Render() {
	GridShader->Use();
	wBuffer->Set();
	glBindVertexArray(VertexArrayBuffer);
	glDrawArrays(GL_LINES, 0, vertices.size());
	glBindVertexArray(0);
}
