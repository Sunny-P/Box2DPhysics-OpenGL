#pragma once
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H

#include <map>
#include <string>
#include <iostream>

struct FontChar
{
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

class TextLabel
{
public:
	TextLabel(
		std::string text,
		std::string font,
		glm::vec2 pos,
		glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f),
		float scale = 1.0f);
	~TextLabel();

	void render();
	void SetText(std::string newText) { text = newText; };
	void SetColor(glm::vec3 newColor) { color = newColor; };
	void SetScale(GLfloat newScale) { scale = newScale; };
	void SetPosition(glm::vec2 position) { pos = position; };

	bool GetDrawAlt();
	void SetDrawAlt(bool _drawAlt);

protected:
	GLuint GenerateTexture(FT_Face face);

	std::string text;
	GLfloat scale;
	glm::vec3 color;
	glm::vec2 pos;

	GLuint VAO, VBO, textProgram;
	glm::mat4 proj;
	std::map<GLchar, FontChar> Characters;

	bool drawAlt;

	const unsigned int scrWidth = 800;
	const unsigned int scrHeight = 600;
};