#pragma once

namespace Utility {
	bool CheckOK(uint const& programID, GLenum const& param, string const& name = "");

	namespace Print{
		void Mat4x4(Matrix target, string name = "default");
		void Float2(Vector2 target);
		void Float3(Vector3 target);
		void Float4(Vector4 target);
	}
	namespace String {
		vector<string> Split(string target, char split);

		int ToInt(string value);
		uint ToUint(string value);
		float ToFloat(string value);
		Vector2 ToVector2(string value);
		Vector3 ToVector3(string value);
		Vector4 ToVector4(string value);
		Matrix ToMatrix(string value);

		string From(const int& value);
		string From(const uint& value);
		string From(const float& value);
		string From(const Vector2& value);
		string From(const Vector3& value);
		string From(const Vector4& value);
		string From(const Matrix& value);
		
		string GetExtension(string path);
		void Replace(string& str, const string& compare, const string& replace);
		string GetFileName(string path);
		string GetFileNameWidthoutExtension(string path);
	}
}