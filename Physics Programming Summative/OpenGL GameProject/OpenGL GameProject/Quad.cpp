#include "Quad.h"
#include "texture.h"

GLuint Quad::GetTexture()
{
	return tex;
}

void Quad::UpdateModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(), position)
				* (glm::rotate(glm::mat4(), glm::radians(yRotationAngle), yRotation) * glm::rotate(glm::mat4(), glm::radians(zRotationAngle), zRotation) * glm::rotate(glm::mat4(), glm::radians(xRotationAngle), xRotation))
				* glm::scale(glm::mat4(), scale);
}

Quad::Quad(GLuint _tex)
{
	tex = _tex;

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	xRotation = glm::vec3(1.0f, 0.0f, 0.0f);
	yRotation = glm::vec3(0.0f, 1.0f, 0.0f);
	zRotation = glm::vec3(0.0f, 0.0f, 1.0f);
	scale = glm::vec3(0.5f, 0.5f, 0.5f);

	yRotationAngle = 0.0f;
	zRotationAngle = 0.0f;
	xRotationAngle = 90.0f;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	indexCount = sizeof(indices) / sizeof(GLuint);
	drawType = GL_TRIANGLES;
}

Quad::~Quad()
{
}

void Quad::render(GLuint _program)
{
	// Disable any openGL funcs unwanted/unnecessary for object
	//glDisable(GL_BLEND);

	glUseProgram(_program);

	GLuint playerModelLoc = glGetUniformLocation(_program, "modelMat");
	glUniformMatrix4fv(playerModelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(_program, "tex"), 0);

	glBindVertexArray(VAO);
	glDrawElements(drawType, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Reenable disabled func for others
	//glEnable(GL_BLEND);

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Quad::update()
{
	UpdateModelMatrix();
}

glm::mat4 Quad::GetModelMatrix()
{
	return modelMatrix;
}

void Quad::IncrementPosition(float _incXBy, float _incYBy, float _incZBy)
{
	position.x += _incXBy;
	position.y += _incYBy;
	position.z += _incZBy;
}

void Quad::IncrementRotation(float _degrees)
{
	yRotationAngle += _degrees;
}

void Quad::IncrementScale(float _incXBy, float _incYBy, float _incZBy)
{
	scale.x += _incXBy;
	scale.y += _incYBy;
	scale.z += _incZBy;
}

void Quad::IncrementPositionVec(glm::vec3 _incrementVec)
{
	position += _incrementVec;
}

void Quad::SetPosition(float _XPos, float _YPos, float _ZPos)
{
	position.x = _XPos;
	position.y = _YPos;
	position.z = _ZPos;
}

void Quad::SetRotation(glm::vec3 rotationVec)
{
	xRotationAngle = rotationVec.x;
	yRotationAngle = rotationVec.y;
	zRotationAngle = rotationVec.z;
}

void Quad::SetScale(float _XScale, float _YScale, float _ZScale)
{
	scale.x = _XScale;
	scale.y = _YScale;
	scale.z = _ZScale;
}

float Quad::GetXPosition() const
{
	return position.x;
}

float Quad::GetYPosition() const
{
	return position.y;
}

float Quad::GetYScale() const
{
	return scale.z;
}
