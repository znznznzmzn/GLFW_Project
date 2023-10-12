#pragma once
class Sphere : public GameObject {
protected:
	void createMesh(int slice, int stack);
public:
	Sphere(int slice = 15);
	Sphere(int slice, int stack);
};