#pragma once
// texture class
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include <iostream>

class Texture
{
public:
	Texture(std::string _filePath, GLint textureWrapType = GL_CLAMP_TO_EDGE);
	//Texture(std::string _filePath, GLint textureWrapType);
	~Texture();

	void init();

	void SetFilePath(std::string _filepath);
	GLuint GetTexture();

protected:
	GLuint tex;
	std::string filePath;

	GLint texWrap;
};