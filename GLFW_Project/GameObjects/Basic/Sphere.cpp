#include "../../Framework.h"

void Sphere::createMesh(int slice, int stack) {
	const float radius = 0.5f;
	float phiStep = Calc::PI / stack;
	float thetaStep = Calc::PI2 / slice;
	//Vertices
	vertices.clear();
	indices.clear();
	for (int i = 0; i <= stack; i++) {
		float phi = i * phiStep;
		for (int j = 0; j <= slice; j++) {
			float theta = j * thetaStep;
			Vertex vertex;
			vertex.normal.x = sin(phi) * cos(theta); // normal
			vertex.normal.y = cos(phi);
			vertex.normal.z = sin(phi) * sin(theta);

			vertex.pos = Vector3(vertex.normal) * radius;

			vertex.uv.x = (float)j / slice;
			vertex.uv.y = (float)i / stack;

			vertices.push_back(vertex);
		}
	}
	//Indices
	for (int i = 0; i < stack; i++) {
		for (int j = 0; j < slice; j++) {
			indices.push_back((slice + 1) * i + j);			 //0
			indices.push_back((slice + 1) * i + j + 1);		 //1
			indices.push_back((slice + 1) * (i + 1) + j);	 //2

			indices.push_back((slice + 1) * (i + 1) + j);	  //2
			indices.push_back((slice + 1) * i + j + 1);		  //1
			indices.push_back((slice + 1) * (i + 1) + j + 1); //3
		}
	}
	Calc::Vertexing::Tangent(vertices, indices);
}
Sphere::Sphere(int slice) : Sphere(slice, slice) { }
Sphere::Sphere(int slice, int stack) {
	tag = "Sphere";
	createMesh(slice, stack);
	init();
}
