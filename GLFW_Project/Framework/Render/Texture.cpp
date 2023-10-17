#include "../Framework.h"

unordered_map<string, Texture*> Texture::textures;

Texture::Texture(const string& path) {
	this->path = path; 
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		if (nrChannels == 1) format = GL_RED;
		else if (nrChannels == 3) format = GL_RGB;
		else if (nrChannels == 4) format = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "Failed to load texture" << std::endl;
	
	// GL_TEXTURE_2D
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	stbi_image_free(data);
}
Texture::~Texture() {  }

void Texture::Init() {
	stbi_set_flip_vertically_on_load(false); // stbi 이미지 로드 형태
	stbi_flip_vertically_on_write(true); // stbi 이미지 작성 형태
}

Texture* Texture::Load(const string& path, string key) {
	if (key.size() == 0) key = path;
	if (textures.count(key) > 0) return textures[key];
	else return textures[key] = new Texture(path);
}

void Texture::Clear() { 
	for (pair<string, Texture*> elem : textures) { 
		glDeleteTextures(1, &elem.second->id);
		SAFE_DELETE(elem.second); 
	}
}

void Texture::Unload(const string& key) {
	Texture*& tmp = textures[key];
	textures.erase(key);
	glDeleteTextures(1, &tmp->id);
	SAFE_DELETE(tmp);
}

void Texture::Set(const int& slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

GLubyte* Texture::GetRawData() {
	GLubyte* pixels = new GLubyte[width * height * nrChannels];
	glBindTexture(GL_TEXTURE_2D, id);
	glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, pixels);
	return pixels;
}

void Texture::ReadPixel4(vector<Vector4>& outPixels, float fallback_pixel) {
	if (nrChannels != 4) {
		cout << "Warning! this texture nrChannels is not RGBA, nrChannels = " << nrChannels << endl;
		cout << "fallback_pixel = " << fallback_pixel << endl;
	}
	GLubyte* pixels = GetRawData();

	int n = width * height;
	const float scale = 1.0f / 255.0f;
	outPixels.resize(n);
	for (int i = 0; i < n; i++) {
		int j = 0;
		for (; j < nrChannels; j++) 
			outPixels[i][j] = static_cast<float>(pixels[i * nrChannels + j]) * scale;
		while (j < 4) {
			outPixels[i][j] = fallback_pixel;
			j++;
		}
	}
}

