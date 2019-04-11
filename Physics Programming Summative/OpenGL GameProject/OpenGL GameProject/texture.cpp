#include "texture.h"

Texture::Texture(std::string _filePath, GLint textureWrapType)
{
	filePath = _filePath;
	tex = 0;

	texWrap = textureWrapType;
}

//Texture::Texture(std::string _filePath, GLint textureWrapType)
//{
//	filePath = _filePath;
//	tex = 0;
//	texWrap = textureWrapType;
//}

Texture::~Texture()
{
}

void Texture::init()
{
	int height, width, channels;
	unsigned char* image;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	image = SOIL_load_image(filePath.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	//--Texture Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texWrap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//--
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetFilePath(std::string _filepath)
{
	filePath = _filepath;
}

GLuint Texture::GetTexture()
{
	return tex;
}
