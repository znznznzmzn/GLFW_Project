#pragma once
class Cylinder : public GameObject {
protected:
	void setVertex(int slice);
public:
	Cylinder(int slice = 21);
};