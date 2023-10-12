#include "../../Framework.h"

unordered_map<string, Texture*> Texture::textures;

Texture::Texture(const string& path) { 
	this->path = path; 
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(data);
}
Texture::~Texture() {  }

void Texture::Start() {
	stbi_set_flip_vertically_on_load(true);
	stbi_flip_vertically_on_write(true);
}

Texture* Texture::Load(const string& path) {
	if (textures.count(path) > 0) return textures[path];
	else return textures[path] = new Texture(path);
}

void Texture::Clear() { 
	for (pair<string, Texture*> elem : textures) { 
		glDeleteTextures(1, &elem.second->id);
		SAFE_DELETE(elem.second); 
	}
}

void Texture::Unload(const string& path) {
	Texture*& tmp = textures[path];
	textures.erase(path);
	glDeleteTextures(1, &tmp->id);
	SAFE_DELETE(tmp);
}

void Texture::Set(const int& slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::ReadPixels(vector<Vector4>& outPixels) {
	GLubyte* pixels = new GLubyte[width * height * 4];
	Set(0);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	int n = width * height;
	const float scale = 1.0f / 255.0f;
	outPixels.resize(n);
	for (int i = 0; i < n; i++) {
		outPixels[i].x = static_cast<float>(pixels[i * 4 + 0]) * scale;
		outPixels[i].y = static_cast<float>(pixels[i * 4 + 1]) * scale;
		outPixels[i].z = static_cast<float>(pixels[i * 4 + 2]) * scale;
		outPixels[i].w = static_cast<float>(pixels[i * 4 + 3]) * scale;
	}
}

