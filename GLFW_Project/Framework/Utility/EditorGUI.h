#pragma once

namespace EditorGUI {
	const string filter_image = "ImageFiles{.tga,.png,.jpg,.jpeg,.bmp,.gif,.hdr,.pnm}"; // 종합 이미지 필터
	const string filter_model = "ModelFiles{.fbx,.obj}"; // 종합 모델 필터

	bool ImageButton(const string& title, Texture*& img, ImVec2 size = { 80, 80 }); // 이미지버튼 간편적용

	bool Dialog_ImageButton_Pattern(const string& key,
		const string& title, Texture*& img, ImVec2 size = { 80, 80 });
	string Dialog_Button_Pattern(string button_name, string key, 
		string title, string file_type, string open_path, ImVec2 button_size = { 0, 0 });
}