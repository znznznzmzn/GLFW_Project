#pragma once

class Transformation { // Transform 간편화
private:
	bool SyncScale = false;
public:
	Vector3 Position = Vector3(0, 0, 0);
	Vector3 Rotation = Vector3(0, 0, 0);
	Vector3 Scale    = Vector3(1, 1, 1);
	Transformation() = default;
	Transformation(Vector3 position, Vector3 rotation, Vector3 scale) :
		Position(position), Rotation(rotation), Scale(scale) { }
	Transformation(const Matrix& in) { Decompose(in); }
	~Transformation() = default;

	operator Matrix() // Position, Rotation, Scale로 즉시 Matrix 반환
	{ return glm::scale(glm::translate(Matrix(1), Position), Scale) * glm::toMat4(Quaternion(Rotation)); }

	void GetMatrix(Matrix& out, const Vector3& pivot = Vector3(0.0f, 0.0f, 0.0f)) {
		out  = glm::translate(Matrix(1), Position + pivot); // Position
		out  = glm::scale(out, Scale); // Scale
		out *= glm::toMat4(Quaternion(Rotation)); // Rotation
	}
	void Decompose(const Matrix& in) {
		Position = in[3]; // Position

		Scale[0] = glm::length(Vector3(in[0])); // Scale
		Scale[1] = glm::length(Vector3(in[1]));
		Scale[2] = glm::length(Vector3(in[2]));

		{ // Rotation
			Quaternion tmp = glm::quat_cast(
				glm::mat3(
					Vector3(in[0]) / Scale[0],
					Vector3(in[1]) / Scale[1],
					Vector3(in[2]) / Scale[2]));
			Rotation = Vector3(tmp.x, tmp.y, tmp.z);
		}
	}

	void GUIRender() {
		ImGui::DragFloat3("Position", (float*)&Position, 0.01f);
		Vector3 degree = Rotation * Calc::PI_to_Degree;
		ImGui::DragFloat3("Rotation", (float*)&degree, 1.0f);
		Rotation = degree * Calc::Degree_to_PI;
		if (SyncScale) {
			ImGui::DragFloat("Scale", (float*)&Scale.x, 0.01f);
			Scale.y = Scale.x;
			Scale.z = Scale.x;
		}
		else ImGui::DragFloat3(":Scale", (float*)&Scale, 0.01f);
		ImGui::Checkbox("SyncScale", &SyncScale);
	}
};