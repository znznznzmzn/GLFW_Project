#include "../../Framework.h"

void GameObject::init(string vshader_path, string fshader_path) {
	material = new Material(vshader_path, fshader_path);
	wBuffer = new MatrixBuffer(material->Get_ProgramID(), "world");
	mesh = new UniversalMesh(vertices, indices);
	world = Matrix(1.0f);

	index_count = indices.size();
}
GameObject::~GameObject() {
	SAFE_DELETE(mesh);
	SAFE_DELETE(wBuffer);
	SAFE_DELETE(material);
}

void GameObject::Render() {
	if (!Active()) return;
	material->Set();
	wBuffer->data = world;
	wBuffer->Set();
	mesh->Set();
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, (void*)0);
}

void GameObject::GUIRender() {
	if (ImGui::TreeNode(tag.c_str())) {
		char name_buf[128] = "";
		if (ImGui::InputTextWithHint("Tag", tag.c_str(), name_buf, IM_ARRAYSIZE(name_buf)))
			if (KEY_DOWN(ImGuiKey_Enter)) tag = string(name_buf);
		Transform::GUIRender();
		material->GUIRender();
		ImGui::TreePop();
	}
}

//void GameObject::InstancingRender(uint instanceCount) {
//	if (!Active()) return;
//	wBuffer->Set(world);
//	wBuffer->SetVSBuffer(0);
//	mesh->IASet();
//	material->Set();
//	DC->DrawIndexedInstanced(indices.size(), instanceCount, 0, 0, 0);
//}
