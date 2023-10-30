#pragma once

class Rect {
private:
	Vertex2D* rectData = nullptr; //- 积己磊俊辑 积己矫难林扁
protected:
	string tag = "Unknown";

public:
	Rect();
	~Rect();
	
	void Render();
	void GUIRender();

	float Left () { return rectData->pos.x - rectData->size.x / 2.0f; }
	float Right() { return rectData->pos.x + rectData->size.x / 2.0f; }
	float Up   () { return rectData->pos.y - rectData->size.y / 2.0f; }
	float Down () { return rectData->pos.y + rectData->size.y / 2.0f; }

	void Save();
	void Load();
};