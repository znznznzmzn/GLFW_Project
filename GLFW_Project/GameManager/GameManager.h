#pragma once

class GameManager {
private:
	bool EditorMode = true;

	void Create();
	void Delete();
public:
	GameManager();
	~GameManager();

	void Update();
	void Render();
};
