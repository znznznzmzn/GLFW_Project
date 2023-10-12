#pragma once
class Plane : public GameObject {
private:
	const float base_distance = 0.5f;
	void createVertex(int count);
public:
	Plane(int count = 5);
};
