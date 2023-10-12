#include "../../Framework.h"
#include "Utility.h"

namespace Utility {
	bool CheckOK(uint const& programID, GLenum const& param, string const& name) {
		int OK;
		char Log[512];
		glGetProgramiv(programID, param, &OK);
		if (!OK) {
			glGetProgramInfoLog(programID, 512, NULL, Log);
			cout << "ERROR::" << name << " Log : " << Log << endl;
		}
		return OK;
	}
	namespace Print {
		void Mat4x4(Matrix target, string name) {
			for (int i = 0; i < 4; i++) 
				Float4(Vector4(target[i][0], target[i][1], target[i][2], target[i][3]));
			cout << endl;
		}
		void Float2(Vector2 target) 
		{ printf("%.1f,\t%.1f\n", target.x, target.y); }
		void Float3(Vector3 target) 
		{ printf("%.1f,\t%.1f,\t%.1f\n", target.x, target.y, target.z); }
		void Float4(Vector4 target) 
		{ printf("%.1f,\t%.1f,\t%.1f,\t%.1f\n", target.x, target.y, target.z, target.w); }
	}

	namespace String {
		vector<string> Split(string target, char split) {
			vector<string> result;
			stringstream stream(target);
			string line;
			while (getline(stream, line, split)) result.emplace_back(line);
			return result;
		}

		int  ToInt (string value) { return stoi(value); }
		uint ToUint(string value) { return static_cast<uint>(ToInt(value)); }
		float ToFloat(string value) { return stof(value); }
		Vector2 ToVector2(string value) {
			vector<string> v = Split(value, ',');
			if (v.size() != 2) cout << "Warning!! Vector2 value size = " << v.size() << endl;
			return Vector2(ToFloat(v[0]), ToFloat(v[1]));
		}
		Vector3 ToVector3(string value) {
			vector<string> v = Split(value, ',');
			if (v.size() != 3) cout << "Warning!! Vector3 value size = " << v.size() << endl;
			return Vector3(ToFloat(v[0]), ToFloat(v[1]), ToFloat(v[2]));
		}
		Vector4 ToVector4(string value) {
			vector<string> v = Split(value, ',');
			if (v.size() != 4) cout << "Warning!! Vector4 value size = " << v.size() << endl;
			return Vector4(ToFloat(v[0]), ToFloat(v[1]), ToFloat(v[2]), ToFloat(v[3]));
		}
		Matrix ToMatrix(string value) {
			vector<string> m = Split(value, '\t');
			Matrix result = Matrix(0.0f); 
			for (int i = 0; i < 4; i++) result[i] = ToVector4(m[i]);
			return result;
		}

		string From(const   int& value) { return to_string(value); }
		string From(const  uint& value) { return to_string(value); }
		string From(const float& value) { return to_string(value); }
		string From(const Vector2& value) 
		{ return to_string(value.x) + "," + to_string(value.y); }
		string From(const Vector3& value) 
		{ return to_string(value.x) + "," + to_string(value.y) + "," + to_string(value.z); }
		string From(const Vector4& value) 
		{ return to_string(value.x) + "," + to_string(value.y) + "," + to_string(value.z) + "," + to_string(value.w); }
		string From(const Matrix& value) {
			return 
				From(Vector4(value[0][0], value[0][1], value[0][2], value[0][3])) + "\t" +
				From(Vector4(value[1][0], value[1][1], value[1][2], value[1][3])) + "\t" +
				From(Vector4(value[2][0], value[2][1], value[2][2], value[2][3])) + "\t" +
				From(Vector4(value[3][0], value[3][1], value[3][2], value[3][3]));
		}

		string String::GetExtension(string path) { return path.substr(path.find_last_of('.') + 1, path.length()); }
		void Replace(string& str, const string& compare, const string& replace) {
			size_t at = 0;
			while ((at = str.find(compare, at)) != string::npos) {
				str.replace(at, compare.length(), replace);
				at += replace.length();
			}
		}
		string GetFileName(string path) {
			Replace(path, "\\", "/");
			return path.substr(path.find_last_of('/') + 1);
		}
		string GetFileNameWidthoutExtension(string path) {
			string filename = GetFileName(path);
			size_t idx = filename.find_last_of('.');
			return filename.substr(0, idx);
		}
	}
}
