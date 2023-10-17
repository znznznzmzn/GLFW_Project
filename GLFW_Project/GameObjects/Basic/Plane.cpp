#include "../Framework.h"

void Plane::createVertex(int count) {
	if (count < 1) count = 1;

	const float base_min = -(base_distance / 2.0f) * count;
	const float base_max = +(base_distance / 2.0f) * count;
	const float base_length = base_distance * count;

	for (float z = base_min; z <= base_max; z += base_distance)
		for (float x = base_min; x <= base_max; x += base_distance) {
			Vertex elem;
			elem.pos = { x, 0.0f, z };
			elem.uv = { 
				1 - ((x + base_max) / base_length),
				1 - ((z + base_max) / base_length)
			};
			vertices.emplace_back(elem); // normal
		}
	
	const float len = static_cast<float>(count + 1);
	for (int z = 0; z < len - 1; z++) {
		for (int x = 0; x < len - 1; x++) {
			indices.emplace_back(x + ((z + 1) * len));
			indices.emplace_back(x + ((z + 1) * len) + 1);
			indices.emplace_back(x + (z * len));
			
			indices.emplace_back(x + (z * len));
			indices.emplace_back(x + ((z + 1) * len) + 1);
			indices.emplace_back(x + 1 + (z * len));
		}
	}

	Calc::Vertexing::Normal(vertices, indices);
	Calc::Vertexing::Tangent(vertices, indices);
}

Plane::Plane(int count) {
	createVertex(count);
	init();
}
