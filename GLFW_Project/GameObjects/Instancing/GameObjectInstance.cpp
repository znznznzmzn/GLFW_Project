#include "../Framework.h"

void GameObjectInstance::AttachInstanceMatrix(uint start_index) {
	CreateInstance();

	if(matrix_buffer_id != -1) glDeleteBuffers(1, &matrix_buffer_id);

	glGenBuffers(1, &matrix_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, matrix_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, instance_count * sizeof(Matrix), matrices.data(), GL_DYNAMIC_DRAW);

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

GameObjectInstance::GameObjectInstance(
	vector<Vertex>& vertices, 
	vector<uint>& indices, 
	uint instanceCount) : Instance(instanceCount) {
	material = new Material(
		"Assets/GLSL/Universal/UniversalInstance.vert",
		"Assets/GLSL/Universal/Universal.frag");
	material->LockShader(true);
	this->vertices = vertices;
	this->indices = indices;
	mesh = new UniversalMesh(vertices, indices);
	indices_count = static_cast<GLsizei>(indices.size());
	AttachInstanceMatrix();
}
GameObjectInstance::~GameObjectInstance() {
	SAFE_DELETE(mesh);
	SAFE_DELETE(material);
	glDeleteBuffers(1, &matrix_buffer_id);
}

void GameObjectInstance::Render() {
	if (!is_active) return;
	material->Set();
	mesh->Set();
	glBindBuffer(GL_ARRAY_BUFFER, matrix_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, instance_count * sizeof(Matrix), matrices.data(), GL_DYNAMIC_DRAW);
	glDrawElementsInstanced(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0, instance_count);
}
void GameObjectInstance::GUIRender() {
	if (ImGui::TreeNode((tag + "_Instance").c_str())) {
		ImGui::Checkbox(tag.c_str(), &is_active);
		char name_buf[128] = "";
		if (ImGui::InputTextWithHint("Tag", tag.c_str(), name_buf, IM_ARRAYSIZE(name_buf)))
			if (KEY_DOWN(ImGuiKey_Enter)) tag = string(name_buf);
		material->GUIRender();

		if (ImGui::TreeNode((tag + "_Instance Controllers").c_str())) {
			//- ImGui::SameLine + -
			ImGui::SliderInt((tag + "_Instance Target").c_str(), &controller_index, 0, instance_count - 1);
			if (instance_count > 0) GUIRender_Instance(controller_index);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}
