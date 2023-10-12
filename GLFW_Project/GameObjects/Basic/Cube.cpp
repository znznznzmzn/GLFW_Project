#include "../../Framework.h"

void Cube::setVertex() {
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;
	// front
	vertices.emplace_back(Vertex(-w2, -h2, -d2, 0, 1, 0, 0, -1));
	vertices.emplace_back(Vertex(-w2, +h2, -d2, 0, 0, 0, 0, -1));
	vertices.emplace_back(Vertex(+w2, +h2, -d2, 1, 0, 0, 0, -1));
	vertices.emplace_back(Vertex(+w2, -h2, -d2, 1, 1, 0, 0, -1));
	// back
	vertices.emplace_back(Vertex(-w2, -h2, +d2, 1, 1, 0, 0, +1));
	vertices.emplace_back(Vertex(+w2, -h2, +d2, 0, 1, 0, 0, +1));
	vertices.emplace_back(Vertex(+w2, +h2, +d2, 0, 0, 0, 0, +1));
	vertices.emplace_back(Vertex(-w2, +h2, +d2, 1, 0, 0, 0, +1));
	// top
	vertices.emplace_back(Vertex(-w2, +h2, -d2, 0, 1, 0, +1, 0));
	vertices.emplace_back(Vertex(-w2, +h2, +d2, 0, 0, 0, +1, 0));
	vertices.emplace_back(Vertex(+w2, +h2, +d2, 1, 0, 0, +1, 0));
	vertices.emplace_back(Vertex(+w2, +h2, -d2, 1, 1, 0, +1, 0));
	// bottom
	vertices.emplace_back(Vertex(-w2, -h2, -d2, 1, 1, 0, -1, 0));
	vertices.emplace_back(Vertex(+w2, -h2, -d2, 0, 1, 0, -1, 0));
	vertices.emplace_back(Vertex(+w2, -h2, +d2, 0, 0, 0, -1, 0));
	vertices.emplace_back(Vertex(-w2, -h2, +d2, 1, 0, 0, -1, 0));
	// left	
	vertices.emplace_back(Vertex(-w2, -h2, +d2, 0, 1, -1, 0, 0));
	vertices.emplace_back(Vertex(-w2, +h2, +d2, 0, 0, -1, 0, 0));
	vertices.emplace_back(Vertex(-w2, +h2, -d2, 1, 0, -1, 0, 0));
	vertices.emplace_back(Vertex(-w2, -h2, -d2, 1, 1, -1, 0, 0));
	// right
	vertices.emplace_back(Vertex(+w2, -h2, -d2, 0, 1, +1, 0, 0));
	vertices.emplace_back(Vertex(+w2, +h2, -d2, 0, 0, +1, 0, 0));
	vertices.emplace_back(Vertex(+w2, +h2, +d2, 1, 0, +1, 0, 0));
	vertices.emplace_back(Vertex(+w2, -h2, +d2, 1, 1, +1, 0, 0));

	indices = {
		0 , 1 , 2 , 0 , 2 , 3 ,
		4 , 5 , 6 , 4 , 6 , 7 ,
		8 , 9 , 10, 8 , 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	 };

	/* 이미지 커팅식
	float size = 0.5f;

	vertices.emplace_back(Vertex(-size, +size, -size, 0.00f, 0.66f));
	vertices.emplace_back(Vertex(-size, -size, -size, 0.25f, 0.66f));
	vertices.emplace_back(Vertex(+size, +size, -size, 0.00f, 0.33f));
	vertices.emplace_back(Vertex(+size, -size, -size, 0.25f, 0.33f));

	vertices.emplace_back(Vertex(-size, -size, +size, 0.50f, 0.66f));
	vertices.emplace_back(Vertex(+size, -size, +size, 0.50f, 0.33f));
	vertices.emplace_back(Vertex(-size, +size, +size, 0.75f, 0.66f));
	vertices.emplace_back(Vertex(+size, +size, +size, 0.75f, 0.33f));

	vertices.emplace_back(Vertex(-size, +size, -size, 1.00f, 0.66f));
	vertices.emplace_back(Vertex(+size, +size, -size, 1.00f, 0.33f));

	vertices.emplace_back(Vertex(-size, +size, -size, 0.25f, 1.00f));
	vertices.emplace_back(Vertex(-size, +size, +size, 0.50f, 1.00f));

	vertices.emplace_back(Vertex(+size, +size, -size, 0.25f, 0.00f));
	vertices.emplace_back(Vertex(+size, +size, +size, 0.50f, 0.00f));

	indices = {
		0,  2,  1,  1,  2,  3, // front
		4,  5,  6,  5,  7,  6, // back
		6,  7,  8,  7,  9 , 8, //top
		1,  3,  4,  3,  5,  4, //bottom
		1, 11, 10,  1,  4, 11, // left
		3, 12,  5,  5, 12, 13  //right
	 };*/
	
	Calc::Vertexing::Tangent(vertices, indices);
}

Cube::Cube() {
	tag = "Cube";
	setVertex();
	init();
}
