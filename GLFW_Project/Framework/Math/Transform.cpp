#include "../Framework.h"

void Transform::GUIRender() {
	if (ImGui::TreeNode((tag + "_Transform").c_str())) {
		ImGui::Checkbox(tag.c_str(), &is_active);
		GUIRender_Local();
		ImGui::DragFloat3("Pivot", (float*)&pivot, 0.01f);
		if (ImGui::Button("Save")) Save(); ImGui::SameLine(); if (ImGui::Button("Load")) Load();
		ImGui::SameLine(); SaveAs(); ImGui::SameLine(); LoadFrom();
		ImGui::TreePop();
	}
}

void Transform::Save(string path) {
	if (path.size() == 0) path = "Assets/Data/Transforms/" + tag + ".srt";
	BinaryWriter* w = new BinaryWriter(path);
	UpdateWorld();
	w->WriteLine(Utility::String::From(local.Position));
	w->WriteLine(Utility::String::From(local.Rotation));
	w->WriteLine(Utility::String::From(local.Scale));
	w->WriteLine(Utility::String::From(pivot));
	SAFE_DELETE(w);
}
void Transform::Load(string path) {
	if (path.size() == 0) path = "Assets/Data/Transforms/" + tag + ".srt";
	BinaryReader* r = new BinaryReader(path);
	local.Position = Utility::String::ToVector3(r->ReadLine());
	local.Rotation = Utility::String::ToVector3(r->ReadLine());
	local.Scale    = Utility::String::ToVector3(r->ReadLine());
	pivot = Utility::String::ToVector3(r->ReadLine());
	UpdateWorld();
	SAFE_DELETE(r);
}

void Transform::SaveAs() {
	string save_path = 
		EditorGUI::Dialog_Button_Pattern("Save As",
			"SaveAs", "Save Transform", ".srt", "Assets\\Data\\Transforms\\.");
	if (save_path.size() != 0) Save(save_path);
}
void Transform::LoadFrom() {
	string save_path =
		EditorGUI::Dialog_Button_Pattern("Load From", 
			"LoadFrom", "Load Transform", ".srt", "Assets\\Data\\Transforms\\.");
	if (save_path.size() != 0) Load(save_path);
}