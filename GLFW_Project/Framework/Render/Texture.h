#pragma once

class Texture {
private:
	static unordered_map<string, Texture*> textures;

	uint id = 0;
	int width = 0, height = 0, nrChannels = 0;
	GLenum format = GL_RGBA;
	string path; // path == key

	Texture(const string& path);
	~Texture();
public:
	static void Init(); // �ʱ�ȭ
	static Texture* Load(const string& path, string key = "");
	static void Unload(const string& key);
	static void Clear();

	void Set(const int& slot);

	const uint& GetID() { return id; } 
	const int& Width () { return width ; }
	const int& Height() { return height; }
	const int& GetChannel() { return nrChannels; }
	const GLenum& GetFormat() { return format; }

	string GetPath() { return path; } // path == key �̱⿡ ���� X

	GLubyte* GetRawData();
	void ReadPixel4(vector<Vector4>& outPixels, float fallback_pixel = 0.0f); // �ش� �ؽ����� pixel�����͸� �о�´� RGBA
};