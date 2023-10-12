#pragma once

// RayCast용 구조체
struct Ray {
	Vector3 position = Vector3(0, 0, 0);
	Vector3 direction = Vector3(0, 0, 0);
};
struct Contact {
	Vector3 hitPoint = Vector3(0, 0, 0);
	float distance = 0;
};

class Point { // float2 말고 int2를 용이하게 사용하기위해 생성
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
};

namespace Calc { // 수학식 
	float const PI = 3.141592f;
	float const PI2 = PI * 2.0f;
	float const PI_Degree = 180.0f;
	float const PI_to_Degree = 180.0f / PI;
	float const Degree_to_PI = PI / 180.0f;

	inline int   Clamp(int   const& value, int   const& min, int   const& max) // min이상 max이하 자르기
	{ return (value < min) ? min : ((value > max) ? max : value); }
	inline float Clamp(float const& value, float const& min, float const& max)
	{ return (value < min) ? min : ((value > max) ? max : value); }

	inline int  ClampIdx(int  const& value, int  const& N) // 0이상 max미만 인덱스 자르기 
	{ return (value < 0) ? 0 : ((value >= N) ? N - 1 : value); }
	inline uint ClampIdx(uint const& value, uint const& N)
	{ return (value < 0) ? 0 : ((value >= N) ? N - 1 : value); }

	inline Point Clamp(Point const& value, Point const& min, Point const& max) // Point Clamp
	{ return Point(Clamp(value.x, min.x, max.x), Clamp(value.y, min.y, max.y)); }
	inline Point ClampIdx(Point const& value, Point const& max) // 포인트 인덱스 Clamp
	{ return Point(ClampIdx(value.x, max.x), ClampIdx(value.y, max.y)); }

	// ※ min이상 max미만 점화식이다(제일 빠름), float사용시 조심하도록
	inline int   Repeat(int   const& value, int   const& min, int   const& max) // 정수형 : min이상 max미만 반복
	{ return min + ((value - min) % (max - min) + (max - min)) % (max - min); } // [(5,0,3) -> 2, (3,0,3) -> 0]
	inline int   Repeat(uint  const& value, uint  const& min, uint  const& max)
	{ return min + ((value - min) % (max - min) + (max - min)) % (max - min); }
	inline float Repeat(float const& value, float const& min, float const& max)
	{ return min + fmodf(fmodf(value - min, max - min) + (max - min), max - min); }

	inline float RepeatPI(float const& value) { return Repeat(value, -PI, PI); } // -PI ~ PI
	inline Vector3 RepeatPI(Vector3 value)
	{ return Vector3(RepeatPI(value.x), RepeatPI(value.y), RepeatPI(value.z)); } // ver. Vector3

// Index : 0이상 len미만 반복
	inline int RepeatIdx(int   const& value, int   const& N) { return (value % N + N) % N; }
	inline int RepeatIdx(uint  const& value, uint  const& N) { return (value % N + N) % N; }

	// 부드러운 이동, (from에 서 to까지 speed만큼만 이동)
	inline float const Lerp(float const& from, float const& to, float const& speed) { return from + (to - from) * speed; }

	// point가 min이상 max이하 범위안에 있는가
	inline bool const IsInLine(int const& point, int const& min, int const& max)
	{ return min <= point && point <= max; }
	inline bool const IsInLine(float const& point, float const& min, float const& max)
	{ return min <= point && point <= max; }
	inline bool const IsInBox(Vector2 const& point, Vector2 const& min, Vector2 const& max)
	{ return IsInLine(point.x, min.x, max.x) && IsInLine(point.y, min.y, max.y); }
	inline bool const IsInBox(Point const& point, Point const& min, Point const& max)
	{ return IsInLine(point.x, min.x, max.x) && IsInLine(point.y, min.y, max.y); }

	// 인덱스 간편화 -> 0이상 N이하 범위안에 있는가
	inline bool const isInIdx(int const& point, int const& N) { return 0 <= point && point < N; }
	inline bool const isInIdx(Point const& point, Point const& N) { return isInIdx(point.x, N.x) && isInIdx(point.y, N.y); }

	int Fibo(int cnt); // 피보나치 점화식

	namespace Vertexing { // 버텍스 자동 생성부분 (기본 수학식)
		void Normal(vector<VertexUVNormalTangent>& vertices, vector<uint> const& indices);
		void Tangent(vector<VertexUVNormalTangent>& vertices, vector<uint> const& indices);
	
		void Clear_Normal(vector<VertexUVNormalTangent>& vertices);
		void Clear_Tangent(vector<VertexUVNormalTangent>& vertices);
		void Clear_NormalTangent(vector<VertexUVNormalTangent>& vertices);
	}

	// 가까운 선 공식
	Vector3 ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point);

	bool RayTriangle_Collision(
		const Ray& ray, 
		const Vector3& v0, const Vector3& v1, const Vector3& v2, 
		float& t, float& u, float& v); // Möller-Trumbore 알고리즘 
	bool RayTriangle_Shorten(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2); // 간소화
	bool RayTriangle_Distance(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2, float& distance); // 거리반환
	bool RayTriangle_Contact(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2, Contact& contact); // Contact
}

namespace Easing { // value 0 ~ 1 사이의 진척도
	enum class TYPE : uint {
		LINEAR,
		IN_BOUNCE, OUT_BOUNCE,
		IN_BACK, OUT_BACK,
		IN_EXPO, OUT_EXPO,
		IN_ELASTIC, OUT_ELASTIC, IN_OUT_ELASTIC
	};
	float InBounce(float value); // 튕기기
	float OutBounce(float value);

	float InBack(float value); // 넘어서 밀렸다 오기
	float OutBack(float value); // 넘어서 밀렸다 오기

	float InExpo(float value); // 빠른 상승
	float OutExpo(float value);

	float InElastic(float value);
	float OutElastic(float value);
	float InOutElastic(float value);

	float Type(TYPE const& ease_type, float const& progress);

	// 0 ~ 1 ~ 0
	inline float Pow2(float value) { return -((value - 0.5f) * (value - 0.5f)) * 4.0f + 1.0f; }
}

namespace Random { // 랜덤함수
	int   Number(); // int최대 최소 랜덤 구하기
	int   NumberTo(unsigned int to); // 0 ~ to 까지의 랜덤 구하기
	float RealNumber(); // 실수값의 랜덤 구하기
	int   Range(int min, int max); // 범위랜덤
	float Range(float f_min, float f_max); // 범위핸덤(실수값)
	float Length(float lenght); // -lenght ~ lenght까지의 랜덤 
	float PI(); // -PI ~ PI 까지의 랜덤
}
