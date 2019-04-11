#pragma once
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

class Texture;

class Quad {
public:
	Quad(GLuint _tex);
	~Quad();

	void render(GLuint _program);
	void update();

	glm::mat4 GetModelMatrix();

	void IncrementPosition(float _incXBy, float _incYBy, float _incZBy);
	void IncrementRotation(float _degrees);
	void IncrementScale(float _incXBy, float _incYBy, float _incZBy);

	void IncrementPositionVec(glm::vec3 _incrementVec);

	void SetPosition(float _XPos, float _YPos, float _ZPos);
	void SetRotation(glm::vec3 rotationVec);
	void SetScale(float _XScale, float _YScale, float _ZScale);

	float GetXPosition() const;
	float GetYPosition() const;

	float GetYScale() const;

	GLuint GetTexture();

protected:
	glm::vec3 position;
	glm::vec3 xRotation;
	glm::vec3 yRotation;
	glm::vec3 zRotation;
	glm::vec3 scale;

	float yRotationAngle;
	float xRotationAngle;
	float zRotationAngle;

	GLuint tex;

	GLuint VAO, VBO, EBO;

	GLfloat vertices[32]{
		// Position				// Colour/Normal		// Tex Coord
		-0.5f, 0.0f, -0.5f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		0.5f, 0.0f, 0.5f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		0.5f, 0.0f, -0.5f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f
	};

	GLuint indices[6]{
		// Winding order
		0, 1, 2,
		0, 2, 3
	};

	int indexCount, drawType;

	glm::mat4 modelMatrix;
	void UpdateModelMatrix();
};