#pragma once
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include <string>

//#include "Mesh.h"

//class Camera;
class Texture;

class CubeMesh {
public:
	CubeMesh(GLuint _tex);
	~CubeMesh();

	void init();
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
	void SetScale(glm::vec3 _vecScale);

	float GetWidth();
	float GetHeight();
	float GetLength();

	GLuint GetTexture();

	void SetShininess(float newShininess);
	float GetShininess();

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	float GetRotationAngle();

	bool GetShouldExist();
	void SetShouldExist(bool _shouldExist);

protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 xRotation;
	glm::vec3 zRotation;
	glm::vec3 scale;

	float width, height, length;

	float rotationAngle, xRotationAngle, zRotationAngle;

	float shininess;

	bool shouldExist;
	//Camera * camera;
	GLuint tex;

	//Texture * texture;
	//std::string textureFilePath;

	GLuint VAO, VBO, EBO;

	// Remember when working with positions, X & Z are horizontal components, Y is vertical component
	GLfloat vertices[192]{
		// Position						// Normal			// Tex Coords
		//		Front Face
		/*0*/	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		/*1*/	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
		/*2*/	0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		/*3*/	0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,

		//		Back
		/*4*/	-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	1.0f, 0.0f,
		/*5*/	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f,
		/*6*/	0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
		/*7*/	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f,

		//		Left
		/*8*/	-0.5f, 0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		/*9*/	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		/*10*/	-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		/*11*/	-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,

		//		Right
		/*12*/	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		/*13*/	0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		/*14*/	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		/*15*/	0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,

		//		Top
		/*16*/	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		/*17*/	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		/*18*/	0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		/*19*/	0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,

		//		Bottom
		/*20*/	-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		/*21*/	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f,
		/*22*/	0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 1.0f,
		/*23*/	0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f
	};

	GLuint indices[36]{
		// Front
		0, 1, 2,
		0, 2, 3,

		// Back
		7, 6, 5,
		7, 5, 4,

		// Left
		8, 9, 10,
		8, 10, 11,

		// Right
		12, 13, 14,
		12, 14, 15,

		// Top
		16, 17, 18,
		16, 18, 19,

		// Bottom
		20, 21, 22,
		20, 22, 23
	};

	int indexCount;
	int drawType;

	glm::mat4 modelMatrix;
	void UpdateModelMatrix();
};