#include "../Framework.h"

namespace EditorGUI {
	bool ImageButton(const string& title, Texture*& img, ImVec2 size) {
		return ((img != nullptr) ?
			ImGui::ImageButton((void*)img->GetID(), size) :
			ImGui::Button(title.c_str(), size));
	}
	bool Dialog_ImageButton_Pattern(const string& key, const string& title, Texture*& img, ImVec2 size) {
		if (ImageButton(title, img, size))
			IMGUI_DIALOG->OpenDialog(key, title, filter_image.c_str(), "Assets\\Textures\\.");

		if (IMGUI_DIALOG->Display(key)) {
			if (IMGUI_DIALOG->IsOk()) {
				string path = IMGUI_DIALOG->GetFilePathName();
				string::size_type n = path.find("Assets");
				if (n == string::npos) std::cout << "Wrong Assets Path = " << path << endl;
				else {
					img = Texture::Load(path.substr(n));
					IMGUI_DIALOG->Close();
					return true;
				}
			}
			IMGUI_DIALOG->Close();
		}
		return false;
	}
	string Dialog_Button_Pattern(
		string button_name, 
		string key, string title, string file_type, string open_path, 
		ImVec2 button_size) {
		if (ImGui::Button(button_name.c_str(), button_size))
			IMGUI_DIALOG->OpenDialog(key.c_str(), title.c_str(), file_type.c_str(), open_path.c_str());

		if (IMGUI_DIALOG->Display(key.c_str())) {
			if (IMGUI_DIALOG->IsOk()) {
				string path = IMGUI_DIALOG->GetFilePathName();
				string::size_type n = path.find("Assets");
				if (n == string::npos) std::cout << "Wrong Assets Path = " << path << endl;
				else {
					IMGUI_DIALOG->Close();
					return path.substr(n);
				}
			}
			IMGUI_DIALOG->Close();
		}
		return "";
	}
}
