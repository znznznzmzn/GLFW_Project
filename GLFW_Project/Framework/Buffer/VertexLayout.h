#pragma once

struct Vertex2D { // 2D용 - 아직 확실하게 구성은 불가(Rotation, Size등)
	Vector2 pos;
	Vector2 uv;
	float rot;
	Vertex2D() : pos(0, 0), uv(0, 0), rot(0) { }
	Vertex2D(Vector2 pos, Vector2 uv, float rot) : pos(pos), uv(uv), rot(rot) { }
	Vertex2D(float x, float y, float u, float v, float rot) : pos(x, y), uv(u, v), rot(rot) { }
};

struct VertexPosition { // 위치만
	Vector3 pos;
	VertexPosition() : pos(0, 0, 0) { }
	VertexPosition(Vector3 pos) : pos(pos) { }
	VertexPosition(float x, float y, float z) : pos(x, y, z) { }
};

struct VertexColor { // 색만
	Vector3 pos;
	Vector4 color;
	VertexColor() : pos(0, 0, 0), color(1, 1, 1, 1) { }
	VertexColor(float x, float y, float z) : pos(x, y, z), color(1, 1, 1, 1) { }
	VertexColor(float x, float y, float z, float r, float g, float b, float a) : pos(x, y, z), color(r, g, b, a) { }
	VertexColor(float x, float y, float z, Vector4 color) : pos(x, y, z), color(color) { }
	VertexColor(Vector3 pos, Vector4 color) : pos(pos), color(color) { }
};

struct VertexUV { // UV까지만(빛 영향 없음)
	Vector3 pos;
	Vector2 uv;
	VertexUV() : pos(0, 0, 0), uv(0, 0) { }
	VertexUV(float x, float y, float z, float u, float v) : pos(x, y, z), uv(u, v) { }
};

struct VertexUVNormal { // Normal만 계산
	Vector3 pos;
	Vector2 uv;
	Vector3 normal;
	VertexUVNormal() : pos(0, 0, 0), uv(0, 0), normal(0, 0, 0) { }
	VertexUVNormal(Vector3 pos, Vector2 uv, Vector3 normal) : pos(pos), uv(uv), normal(normal) { }
	VertexUVNormal(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: pos(x, y, z), uv(u, v), normal(nx, ny, nz) { }
};

struct VertexUVNormalTangent {
	Vector3 pos;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
	VertexUVNormalTangent() : pos(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0) { }
	VertexUVNormalTangent(Vector3 pos) : pos(pos), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0) { }
	VertexUVNormalTangent(Vector3 pos, Vector2 uv)
		: pos(pos), uv(uv), normal(0, 0, 0), tangent(0, 0, 0) {}
	VertexUVNormalTangent(Vector3 pos, Vector2 uv, Vector3 normal, Vector3 tangent)
		: pos(pos), uv(uv), normal(normal), tangent(tangent) { }
	VertexUVNormalTangent(float x, float y, float z, float u, float v)
		: pos(x, y, z), uv(u, v), normal(0, 0, 0), tangent(0, 0, 0) { }
	VertexUVNormalTangent(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: pos(x, y, z), uv(u, v), normal(nx, ny, nz), tangent(0, 0, 0) { }
	VertexUVNormalTangent(float x, float y, float z, float u, float v,
		float nx, float ny, float nz, float tx, float ty, float tz)
		: pos(x, y, z), uv(u, v), normal(nx, ny, nz), tangent(tx, ty, tz) { }
};
typedef VertexUVNormalTangent Vertex; // 사용에 편의를 위해 (Universal)Vertex로 typedef

struct VertexUVNormalTangentBlend {
	Vector3 pos;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
	Vector4 indices;
	Vector4 weights;
	VertexUVNormalTangentBlend() : 
		pos(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0),
		indices(0, 0, 0, 0), weights(0, 0, 0, 0) { }
};
typedef VertexUVNormalTangentBlend ModelVertex; // 모델용 Vertex

struct VertexAlpha { // TerrainEditor용 VertexAlpha
	Vector3 pos;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
	Vector4 alpha;
	VertexAlpha() : pos(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0), alpha(1, 1, 1, 1) { }
};

struct VertexSize {
	Vector3 pos;
	Vector2 size;
	VertexSize() : pos(0, 0, 0), size(0, 0) { }
};

