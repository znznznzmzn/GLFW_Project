#pragma once

namespace EditorGUI {
	const string filter_image = "ImageFiles{.tga,.png,.jpg,.jpeg,.bmp,.gif,.hdr,.pnm}"; // ���� �̹��� ����
	const string filter_model = "ModelFiles{.fbx,.obj}"; // ���� �� ����

	bool ImageButton(const string& title, Texture*& img, ImVec2 size = { 80, 80 }); // �̹�����ư ��������

	bool Dialog_ImageButton_Pattern(const string& key,
		const string& title, Texture*& img, ImVec2 size = { 80, 80 });
	string Dialog_Button_Pattern(string button_name, string key, 
		string title, string file_type, string open_path, ImVec2 button_size = { 0, 0 });
}