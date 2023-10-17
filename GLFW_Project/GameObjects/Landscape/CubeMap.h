#pragma once

class CubeMap {
private:
	uint cubemap_texture_id = 0;
	
	vector<Vector3> vertices;
	vector<uint> indices;
	uint index_count = 0;
protected:
	string name = "CubeMap_00";
	bool isActive = true;

	ShaderProgram* skyShader = nullptr;
	PositionMesh* mesh = nullptr;
	TextureBuffer* cubeMapBuffer = nullptr;

	void CreateMesh(int slice = 15, int stack = 15);
public:
	enum class Direction {
		Right = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		Left  = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		Up   = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		Down = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		Forward = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		Back    = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	CubeMap(int slice = 15, int stack = 15);
	~CubeMap();

	void Render();
	void GUIRender();

	void SetActive(bool will_active) { isActive = will_active; }
	bool IsActive() { return isActive; }

	void SetTexture(Texture* texture, GLenum direction);
	void SetTexture(Texture* texture, Direction direction)
	{ SetTexture(texture, static_cast<GLenum>(direction)); }
	void SetTexture(const string& texture_path, GLenum direction)
	{ SetTexture(Texture::Load(texture_path), direction); }
	void SetTexture(const string& texture_path, Direction direction)
	{ SetTexture(Texture::Load(texture_path), direction); }
};

// [dds file = Direct Draw Surface file] DirectXTex 라이브러리 필요
// 현재는 텍스쳐 6개를 받아서 사용