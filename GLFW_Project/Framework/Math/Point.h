#pragma once
// 편의성(인덱스 계산)을 위한 Point 클래스

class Point { // int x, y
public:
	int x, y;
	Point() : x(0), y(0) { }
	Point(int x, int y) : x(x), y(y) { }
	Point(uint x, uint y) : x(static_cast<int>(x)), y(static_cast<int>(y)) { }
	Point(float x, float y) : x(static_cast<int>(floor(x))), y(static_cast<int>(floor(y))) { }
	Point(Vector2 vec2) : Point(vec2.x, vec2.y) { }
	Point(Vector3 vec3) : Point(vec3.x, vec3.y) { }
	operator Vector2() { return Vector2(x, y); }
	operator Vector3() { return Vector3(x, y, 0); }
	Point operator-(const int& value) const { return Point(x - value, y - value); }
	Point operator+(const int& value) const { return Point(x + value, y + value); }
	Point operator-(const float& value) const { return Point(x - value, y - value); }
	Point operator+(const float& value) const { return Point(x + value, y + value); }
	Point operator-(const Point& value) const { return Point(x - value.x, y - value.y); }
	Point operator+(const Point& value) const { return Point(x + value.x, y + value.y); }
	bool operator==(const Point& value) const { return x == value.x && y == value.y; }
	bool operator!=(const Point& value) const { return !(*this == value); }
	// 필요한 연산이 있으면 그때그때 추가하기
};
class UPoint { // uint x, y
public:
	uint x, y;
	UPoint() : x(0), y(0) { }
	UPoint(int x, int y) : x(static_cast<uint>(x)), y(static_cast<uint>(y)) { }
	UPoint(uint x, uint y) : x(x), y(y) { }
	UPoint(float x, float y) : x(static_cast<uint>(floor(x))), y(static_cast<uint>(floor(y))) { }
	UPoint(Vector2 vec2) : UPoint(static_cast<uint>(vec2.x), static_cast<uint>(vec2.y)) { }
	UPoint(Vector3 vec3) : UPoint(static_cast<uint>(vec3.x), static_cast<uint>(vec3.y)) { }
	operator Vector2() { return Vector2(static_cast<float>(x), static_cast<float>(y)); }
	operator Vector3() { return Vector3(static_cast<float>(x), static_cast<float>(y), 0); }
};

class Point3 { // int x, y, z
public:
	int x, y, z;
	Point3() : x(0), y(0), z(0) { }
	Point3(int x, int y, int z) : x(x), y(y), z(z) { }
};

class UPoint3 { // uint x, y, z
public:
	uint x, y, z;
	UPoint3() : x(0), y(0), z(0) { }
	UPoint3(uint x, uint y, uint z) : x(x), y(y), z(z) { }
	operator Vector3() { return Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); }
	Vector3 operator*(const float& value) const 
	{ return Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)) * value; }
};