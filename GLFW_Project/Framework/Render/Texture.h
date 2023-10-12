#pragma once

class Texture {
private:
	uint id;
	int width, height, nrChannels;
	string path;

	static unordered_map<string, Texture*> textures;

	Texture(const string& path);
	~Texture();
public:
	static void Start();
	static Texture* Load(const string& path);
	static void Unload(const string& path);
	static void Clear();

	void Set(const int& slot);

	const uint& GetID() { return id; }
	const int& Width () { return width ; }
	const int& Height() { return height; }
	const int& GetChannel() { return nrChannels; }

	string GetPath() { return path; }

	void ReadPixels(vector<Vector4>& outPixels);
};