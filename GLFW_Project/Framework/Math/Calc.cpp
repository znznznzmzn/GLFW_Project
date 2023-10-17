#include "../Framework.h"

namespace Calc {
	const double sq5 = sqrt(5);
	int Fibo(int cnt)
	{ return static_cast< int >( ( 1 / sq5 ) * ( pow(( 1 + sq5 ) / 2, cnt) - pow(( 1 - sq5 ) / 2, cnt) ) ); }
	
	namespace Vertexing {
		uint idx1 = 0;
		uint idx2 = 0;
		uint idx3 = 0;
		void Normal(vector<VertexUVNormalTangent>& vertices, vector<uint> const& indices) {
			int n = static_cast<int>(indices.size() / 3);
			for (int i = 0; i < n; i++) {
				idx1 = indices[i * 3 + 0];
				idx2 = indices[i * 3 + 1];
				idx3 = indices[i * 3 + 2];
	
				Vector3 normal = glm::normalize(glm::cross(
					vertices[idx2].pos - vertices[idx1].pos,
					vertices[idx3].pos - vertices[idx1].pos));
	
				vertices[idx1].normal = normal + vertices[idx1].normal;
				vertices[idx2].normal = normal + vertices[idx2].normal;
				vertices[idx3].normal = normal + vertices[idx3].normal;
			}
		}
		void Tangent(vector<VertexUVNormalTangent>& vertices, vector<uint> const& indices) {
			int n = static_cast<int>(indices.size() / 3);
			for (int i = 0; i < n; i++) {
				idx1 = indices[i * 3 + 0];
				idx2 = indices[i * 3 + 1];
				idx3 = indices[i * 3 + 2];
	
				Vector3 e0 = vertices[idx2].pos - vertices[idx1].pos;
				Vector3 e1 = vertices[idx3].pos - vertices[idx1].pos;
	
				float u0 = vertices[idx2].uv.x - vertices[idx1].uv.x;
				float v0 = vertices[idx2].uv.y - vertices[idx1].uv.y;
				float u1 = vertices[idx3].uv.x - vertices[idx1].uv.x;
				float v1 = vertices[idx3].uv.y - vertices[idx1].uv.y;
	
				float d = 1.0f / (u0 * v1 - v0 * u1);
	
				Vector3 tangent = d * (e0 * v1 - e1 * v0);
	
				vertices[idx1].tangent += tangent;
				vertices[idx2].tangent += tangent;
				vertices[idx3].tangent += tangent;
			}
	
			for (VertexUVNormalTangent& vertex : vertices) {
				Vector3 t = vertex.tangent;
				Vector3 n = vertex.normal;
				vertex.tangent = glm::normalize((t - n * glm::dot(n, t)));
			}
		}

		void Clear_Normal(vector<VertexUVNormalTangent>& vertices)
		{ for (VertexUVNormalTangent& elem : vertices) elem.normal = Vector3(0, 0, 0); }
		void Clear_Tangent(vector<VertexUVNormalTangent>& vertices)
		{ for (VertexUVNormalTangent& elem : vertices) elem.tangent = Vector3(0, 0, 0); }
		void Clear_NormalTangent(vector<VertexUVNormalTangent>& vertices)
		{ for (VertexUVNormalTangent& elem : vertices) elem.tangent = elem.normal = Vector3(0, 0, 0); }
	}
	
	Vector3 ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point) {
		Vector3 line = end - start;
		return start + line * Clamp(
			glm::dot(line, point - start) / glm::dot(line, line), 0.0f, 1.0f);
	}

	bool RayTriangle_Collision(
		const Ray& ray, 
		const Vector3& v0, const Vector3& v1, const Vector3& v2, 
		float& t, float& u, float& v) {
		Vector3 v0v1 = v1 - v0;
		Vector3 v0v2 = v2 - v0;
		Vector3 pvec = glm::cross(ray.direction, v0v2);
		float det = glm::dot(v0v1, pvec);

		if (det < FLT_EPSILON) return false;
		if (fabs(det) < FLT_EPSILON) return false;
		float invDet = 1 / det;

		Vector3 tvec = ray.position - v0;
		u = glm::dot(tvec, pvec) * invDet;
		if (u < 0 || u > 1) return false;

		Vector3 qvec = glm::cross(tvec, v0v1);
		v = glm::dot(ray.direction, qvec) * invDet;
		if (v < 0 || u + v > 1) return false;

		t = glm::dot(v0v2, qvec) * invDet;

		return true;
	}
	bool RayTriangle_Shorten(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2) {
		Vector3 v0v1 = v1 - v0;
		Vector3 pvec = glm::cross(ray.direction, v2 - v0);
		float det = glm::dot(v0v1, pvec);

		if (det < FLT_EPSILON) return false;
		if (fabs(det) < FLT_EPSILON) return false;

		Vector3 tvec = ray.position - v0;
		float u = glm::dot(tvec, pvec) / det;
		if (u < 0 || u > 1) return false;
		
		float v = glm::dot(ray.direction, glm::cross(tvec, v0v1)) / det;
		if (v < 0 || u + v > 1) return false;

		return true;
	}
	bool RayTriangle_Distance(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2, float& distance) {
		Vector3 v0v1 = v1 - v0;
		Vector3 v0v2 = v2 - v0;
		Vector3 pvec = glm::cross(ray.direction, v0v2);
		float det = glm::dot(v0v1, pvec);

		if (det < FLT_EPSILON) return false;
		if (fabs(det) < FLT_EPSILON) return false;

		Vector3 tvec = ray.position - v0;
		float u = glm::dot(tvec, pvec) / det;
		if (u < 0 || u > 1) return false;

		Vector3 qvec = glm::cross(tvec, v0v1);
		float v = glm::dot(ray.direction, qvec) / det;
		if (v < 0 || u + v > 1) return false;

		distance = glm::dot(v0v2, qvec) / det;
		return true;
	}
	bool RayTriangle_Contact(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2, Contact& contact) {
		bool result = RayTriangle_Distance(ray, v0, v1, v2, contact.distance);
		if (result) contact.hitPoint = ray.position + ray.direction * contact.distance; 
		return result; 
	}
}

namespace Easing {
	const float n1 = 7.5625f;
	const float d1 = 2.75f;
	float OutBounce(float value) {
		if ( value < 1.0f / d1 ) return n1 * value * value;
		else if ( value < 2.0f / d1 ) return n1 * ( value -= 1.5f / d1 ) * value + 0.75f;
		else if ( value < 2.5f / d1 ) return n1 * ( value -= 2.25f / d1 ) * value + 0.9375f;
		else return n1 * ( value -= 2.625f / d1 ) * value + 0.984375f;
	}
	float InBounce(float value) { return 1 - OutBounce(1 - value); }

	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;
	float InBack (float value) { return c3 * pow(value, 3.0f) - c1 * pow(value, 2.0f); }
	float OutBack(float value) { return 1.0f + c3 * pow(value - 1.0f, 3.0f) + c1 * pow(value - 1.0f, 2.0f); }

	float InExpo (float value) { return value = ( value == 0.0f ) ? 0.0f : pow(2.0f, 10.0f * value - 10.0f); }
	float OutExpo(float value) { return 1.0f - InExpo(1.0f - value); }

	const float c4 = (2.0f * Calc::PI) / 3.0f;
	const float c5 = (2.0f * Calc::PI) / 4.5f;
	float InElastic(float value) {
		return 
			(value == 0.0f) ? 0.0f : 
			(value == 1.0f) ? 1.0f : 
			-pow(2.0f, 10.0f * value - 10.0f) * sin((value * 10.0f - 10.75f) * c4);
	}
	float OutElastic(float value){
		return 
			(value == 0.0f) ? 0.0f : 
			(value == 1.0f) ? 1.0f : 
			pow(2.0f, -10.0f * value) * sin((value * 10.0f - 0.75f) * c4) + 1.0f;
	}
	float InOutElastic(float value) {
		return
			(value == 0.0f) ? 0.0f :
			(value == 1.0f) ? 1.0f : 
			(value <  0.5f) ? -(pow(2.0f, +20.0f * value - 10.0f) * sin((20.0f * value - 11.125f) * c5)) / 2.0f :
							   (pow(2.0f, -20.0f * value + 10.0f) * sin((20.0f * value - 11.125f) * c5)) / 2.0f + 1.0f;
	}


	float Type(TYPE const& ease_type, float const& progress) {
		switch (ease_type) {
		default: case Easing::TYPE::LINEAR: return progress;
		case Easing::TYPE::OUT_BOUNCE: return Easing::OutBounce(progress);
		case Easing::TYPE:: IN_BOUNCE: return Easing:: InBounce(progress);
		case Easing::TYPE::OUT_BACK: return Easing::OutBack(progress);
		case Easing::TYPE:: IN_BACK: return Easing:: InBack(progress);
		case Easing::TYPE::OUT_EXPO: return Easing::OutExpo(progress);
		case Easing::TYPE:: IN_EXPO: return Easing:: InExpo(progress);
		}
	}
}

namespace Random { // C++ 17용 최적화된 랜덤함수 (빠름 ㅇㅇ) 
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dist(INT_MIN, INT_MAX); // int최대 최소값
	int Number() { return dist(gen); }
	int NumberTo(unsigned int to) { return abs(Number()) % to; }
	float RealNumber() { return static_cast<float>(Number() % 10000) / Calc::PI; }
	int   Range (int   min, int   max) { return Calc::Repeat(Number(), min, max); } // min이상 max미만
	float Range (float min, float max) { return Calc::Repeat(RealNumber(), min, max); } // ※ min이상 max미만
	float Length(float lenght) { return Range(-lenght / 2.0f, lenght / 2.0f); }
	float PI() { return Length(Calc::PI); }
}