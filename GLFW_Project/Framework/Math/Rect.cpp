#include "../Framework.h"

Rect::Rect() {
	rectData = new Vertex2D();
}
Rect::~Rect() {
	SAFE_DELETE(rectData);
}

void Rect::Render() {
}

void Rect::GUIRender()
{
}

void Rect::Save() {
}
void Rect::Load() {
}
