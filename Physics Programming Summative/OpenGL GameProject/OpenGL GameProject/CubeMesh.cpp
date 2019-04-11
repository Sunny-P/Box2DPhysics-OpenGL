#include "CubeMesh.h"
//#include "Camera.h"
#include "texture.h"

CubeMesh::CubeMesh(GLuint _tex)
{
	//texture = new Texture("Resources/Textures/AwesomeFace.png");
	//texture = _texture;
	//texture->init();
	shouldExist = true;
	tex = _tex;

	width = 0.5f;
	height = 0.5f;
	length = 0.5f;

	position = glm::vec3(0.0f, 0.25f, 0.0f);
	rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	xRotation = glm::vec3(1.0f, 0.0f, 0.0f);
	zRotation = glm::vec3(0.0f, 0.0f, 1.0f);
	scale = glm::vec3(width, height, length);

	rotationAngle = 0.0f;
	xRotationAngle = 0.0f;
	zRotationAngle = 0.0f;

	// Shininess should be a value 1-256
	shininess = 20.0f;
}

CubeMesh::~CubeMesh()
{
	//delete texture;
}

void CubeMesh::init()
{
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

void CubeMesh::render(GLuint _program)
{
	// Disable any openGL funcs unwanted/unnecessary for object
	glDisable(GL_BLEND);

	glUseProgram(_program);

	GLuint playerModelLoc = glGetUniformLocation(_program, "modelMat");
	glUniformMatrix4fv(playerModelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniform1f(glGetUniformLocation(_program, "shininess"), shininess);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(_program, "tex"), 0);

	glBindVertexArray(VAO);
	glDrawElements(drawType, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Reenable disabled func for others
	glEnable(GL_BLEND);

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CubeMesh::update()
{
	UpdateModelMatrix();
}

glm::mat4 CubeMesh::GetModelMatrix()
{
	return modelMatrix;
}

void CubeMesh::IncrementPosition(float _incXBy, float _incYBy, float _incZBy)
{
	position.x += _incXBy;
	position.y += _incYBy;
	position.z += _incZBy;
}

void CubeMesh::IncrementRotation(float _degrees)
{
	rotationAngle += _degrees;
}

void CubeMesh::IncrementScale(float _incXBy, float _incYBy, float _incZBy)
{
	scale.x += _incXBy;
	scale.y += _incYBy;
	scale.z += _incZBy;
	width += _incXBy;
	height += _incYBy;
	length += _incZBy;
}

void CubeMesh::IncrementPositionVec(glm::vec3 _incrementVec)
{
	position += _incrementVec;
}

void CubeMesh::SetPosition(float _XPos, float _YPos, float _ZPos)
{
	position.x = _XPos;
	position.y = _YPos;
	position.z = _ZPos;
}

void CubeMesh::SetRotation(glm::vec3 rotationVec)
{
	rotationAngle = rotationVec.y;
	xRotationAngle = rotationVec.x;
	zRotationAngle = rotationVec.z;
}

void CubeMesh::SetScale(float _XScale, float _YScale, float _ZScale)
{
	scale.x = _XScale;
	scale.y = _YScale;
	scale.z = _ZScale;

	width = _XScale;
	height = _YScale;
	length = _ZScale;
}

void CubeMesh::SetScale(glm::vec3 _vecScale)
{
	scale = _vecScale;

	width = _vecScale.x;
	height = _vecScale.y;
	length = _vecScale.z;
}

float CubeMesh::GetWidth()
{
	return width;
}

float CubeMesh::GetHeight()
{
	return height;
}

float CubeMesh::GetLength()
{
	return length;
}

GLuint CubeMesh::GetTexture()
{
	return tex;
}

void CubeMesh::SetShininess(float newShininess)
{
	shininess = newShininess;
}

float CubeMesh::GetShininess()
{
	return shininess;
}

glm::vec3 CubeMesh::GetPosition()
{
	return position;
}

glm::vec3 CubeMesh::GetRotation()
{
	return rotation;
}

float CubeMesh::GetRotationAngle()
{
	return rotationAngle;
}

bool CubeMesh::GetShouldExist()
{
	return shouldExist;
}

void CubeMesh::SetShouldExist(bool _shouldExist)
{
	shouldExist = _shouldExist;
}

void CubeMesh::UpdateModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(), position)
				* glm::rotate(glm::mat4(), glm::radians(rotationAngle), rotation) * glm::rotate(glm::mat4(), glm::radians(xRotationAngle), xRotation) * glm::rotate(glm::mat4(), glm::radians(zRotationAngle), zRotation)
				* glm::scale(glm::mat4(), scale);
}
