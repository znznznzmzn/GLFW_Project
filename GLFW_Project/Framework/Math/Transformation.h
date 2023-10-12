#pragma once

class Transformation { // Transform ∞£∆Ì»≠
private:
	bool SyncScale = false;
public:
	Vector3 Position = Vector3(0, 0, 0);
	Vector3 Rotation = Vector3(0, 0, 0);
	Vector3 Scale    = Vector3(1, 1, 1);
	Transformation() = default;
	Transformation(Vector3 position, Vector3 rotation, Vector3 scale) :
		Position(position), Rotation(rotation), Scale(scale) { }
	~Transformation() = default;

	void GetMatrix(Matrix& out, const Vector3& pivot = Vector3(0.0f, 0.0f, 0.0f)) {
		out  = glm::translate(Matrix(1.0f), Position + pivot); // Position
		out  = glm::scale(out, Scale); // Scale
		out *= glm::toMat4(Quaternion(Rotation)); // Rotation
	}
	void Decompose(const Matrix& in) {
		// Position
		Position = in[3];

		// Scale
		for (int i = 0; i < 3; i++)
			Scale[i] = glm::length(Vector3(in[i]));

		// Rotation
		{
			Quaternion tmp = glm::quat_cast(
				glm::mat3(
					Vector3(in[0]) / Scale[0],
					Vector3(in[1]) / Scale[1],
					Vector3(in[2]) / Scale[2]));
			Rotation = Vector3(tmp.x, tmp.y, tmp.z);
		}
	}

	void GUIRender(const string& tag) {
		ImGui::DragFloat3((tag + ":Pos").c_str(), (float*)&Position, 0.01f);
		Vector3 degree = Rotation * Calc::PI_to_Degree;
		ImGui::DragFloat3((tag + ":Rot").c_str(), (float*)&degree, 1.0f);
		Rotation = degree * Calc::Degree_to_PI;
		if (SyncScale) {
			ImGui::DragFloat((tag + ":Scale").c_str(), (float*)&Scale.x, 0.01f);
			Scale.y = Scale.x;
			Scale.z = Scale.x;
		}
		else ImGui::DragFloat3((tag + ":Scale").c_str(), (float*)&Scale, 0.01f);
		ImGui::Checkbox("SyncScale", &SyncScale);
	}
};