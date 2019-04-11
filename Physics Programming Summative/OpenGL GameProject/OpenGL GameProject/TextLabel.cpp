#include "TextLabel.h"

TextLabel::TextLabel(std::string _text, std::string font, glm::vec2 pos, glm::vec3 color, float scale)
{
	text = _text;
	SetColor(color);
	SetScale(scale);
	SetPosition(pos);

	drawAlt = false;

	GLfloat halfWidth = (GLfloat)scrWidth * 0.5f;
	GLfloat halfHeight = (GLfloat)scrHeight * 0.5f;

	proj = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight);
	ShaderLoader loader;
	textProgram = loader.CreateProgram("Resources/Shaders/TextVS.vs", "Resources/Shaders/TextFS.fs");

	FT_Library ft;
	FT_Face face;

	if (FT_Init_FreeType(&ft) != 0)
	{
		std::cout << "ERROR: FREETYPE: Could not init FreeType Library!" << std::endl;
	}

	if (FT_New_Face(ft, font.c_str(), 0, &face) != 0)
	{
		std::cout << "ERROR::FREETYPE: Failed to Load Font!" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph!" << std::endl;
			continue;
		}

		GLuint texture = GenerateTexture(face);

		FontChar character = { texture,
		glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
		(GLuint)face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, FontChar>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

TextLabel::~TextLabel()
{
}

void TextLabel::render()
{
	glUseProgram(textProgram);
	glUniform3f(glGetUniformLocation(textProgram, "textColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(textProgram, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glBindVertexArray(VAO);

	glm::vec2 textPos = pos;
	for (std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		FontChar ch = Characters[*c];
		GLfloat xpos = textPos.x + ch.Bearing.x * scale;
		GLfloat ypos = textPos.y - (ch.Size.y - ch.Bearing.y) * scale;
		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		GLfloat vertices[6][4] = {
			{xpos, ypos + h, 0.0f, 0.0f}, {xpos, ypos, 0.0f, 1.0f}, {xpos + w, ypos, 1.0f, 1.0f},
			{xpos, ypos + h, 0.0f, 0.0f}, {xpos + w, ypos, 1.0f, 1.0f}, {xpos + w, ypos + h, 1.0f, 0.0f},
		};
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glUniform1i(glGetUniformLocation(textProgram, "tex"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		textPos.x += (ch.Advance >> 6) * scale;
	}

	

	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool TextLabel::GetDrawAlt()
{
	return drawAlt;
}

void TextLabel::SetDrawAlt(bool _drawAlt)
{
	drawAlt = _drawAlt;
}

GLuint TextLabel::GenerateTexture(FT_Face face)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RED,
		face->glyph->bitmap.width,
		face->glyph->bitmap.rows,
		0,
		GL_RED, GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}
