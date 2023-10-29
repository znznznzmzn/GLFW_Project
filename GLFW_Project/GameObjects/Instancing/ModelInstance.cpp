#include "../Framework.h"

void ModelInstance::LoadModel(string model_name) {
	ClearModelMap();
	ModelReader* reader = new ModelReader();
	data = reader->ReadModel(model_name);
	for (MeshData*& elem : data->meshes) {
		ModelMesh* mesh = new ModelMesh(elem->vertices, elem->indices);
		render_map[data->materials[elem->meterialIndex]].emplace_back(mesh);
	}
	SAFE_DELETE(reader);
}

void ModelInstance::SetShaders(string vshader_path, string fshader_path) {
	for (Material*& elem : data->materials) elem->SetShader(vshader_path, fshader_path);
	InitUniformBuffer();
}

void ModelInstance::ClearModelMap() {
	for (auto elem : render_map) {
		for (ModelMesh*& mesh : elem.second) { SAFE_DELETE(mesh); }
		elem.second.clear();
	}
	render_map.clear();
}

void ModelInstance::AttachInstanceMatrix(uint start_index) {
	CreateInstance();

	if (matrix_buffer_id != -1) glDeleteBuffers(1, &matrix_buffer_id);

	glGenBuffers(1, &matrix_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, matrix_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, instance_count * sizeof(Matrix), matrices.data(), GL_DYNAMIC_DRAW);

	for (auto elem : render_map) {
		for (ModelMesh*& mesh : elem.second) {
			mesh->Set();
			// vertex attributes
			size_t offset = sizeof(Vector4);
			size_t stride = sizeof(Matrix);

			for (int i = 0; i < 4; i++) {
				glEnableVertexAttribArray(start_index + i);
				glVertexAttribPointer(start_index + i, 4, GL_FLOAT, GL_FALSE, stride, (void*)(i * offset));
				glVertexAttribDivisor(start_index + i, 1);
			}
			glBindVertexArray(0);
		}
	}
}

void ModelInstance::SetMeshInstance() {
	glBindBuffer(GL_ARRAY_BUFFER, matrix_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, instance_count * sizeof(Matrix), matrices.data(), GL_DYNAMIC_DRAW);
}

ModelInstance::ModelInstance(string model_name, uint instanceCount) : Instance(instanceCount) {
	LoadModel(model_name);
	SetShaders(
		"Assets/GLSL/Universal/UniversalInstance.vert",
		"Assets/GLSL/Universal/Universal.frag");
	AttachInstanceMatrix();
}
ModelInstance::~ModelInstance() {
	ClearModelMap();
	SAFE_DELETE(data);
}

void ModelInstance::Render() {
	if (!is_active) return;
	for (auto elem : render_map) {
		elem.first->Set(); // Material(Shader) Set
		SetUniformBuffer();
		for (ModelMesh*& mesh : elem.second) {
			mesh->Set();
			SetMeshInstance();
			glDrawElementsInstanced(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0, instance_count);
		}
	}
}
void ModelInstance::GUIRender() {
	if (ImGui::TreeNode((tag + "_Model Instance").c_str())) {
		ImGui::Checkbox(tag.c_str(), &is_active);
		char name_buf[128] = "";
		if (ImGui::InputTextWithHint("Tag", tag.c_str(), name_buf, IM_ARRAYSIZE(name_buf)))
			if (KEY_DOWN(ImGuiKey_Enter)) tag = string(name_buf);

		for (auto elem : render_map) elem.first->GUIRender(); // Material GUIRender
		
		if (ImGui::TreeNode((tag + "_Instance Controllers").c_str())) {
			//- ImGui::SameLine + -
			ImGui::SliderInt((tag + "_Model Instance Target").c_str(), &controller_index, 0, instance_count - 1);
			if (instance_count > 0) GUIRender_Instance(controller_index);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}
