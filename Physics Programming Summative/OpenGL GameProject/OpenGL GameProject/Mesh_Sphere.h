#pragma once

// Local Includes
//#include "Mesh.h"

#include <glew.h>
#include <freeglut.h>
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <string>

class Texture;

class Mesh_Sphere
{
public:
	Mesh_Sphere(GLuint _tex);
	~Mesh_Sphere();

	void init();
	void render(GLuint _program, GLuint skyboxTexID = NULL);
	void update();

	glm::mat4 GetModelMatrix();

	void IncrementPosition(float _incXBy, float _incYBy, float _incZBy);
	void IncrementRotation(float _degrees);
	void IncrementScale(float _incXBy, float _incYBy, float _incZBy);

	void SetPosition(float _XPos, float _YPos, float _ZPos);
	void SetRotation(float _angle);
	void SetScale(float _XScale, float _YScale, float _ZScale);

	void SetShininess(float newShininess);
	float GetShininess();

	GLuint GetTexture();

protected:
	GLuint VAO, VBO, EBO;
	GLuint tex;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	float radius;

	float rotationAngle;

	float shininess;

	GLuint indexCount;
	int drawType;

	glm::mat4 modelMatrix;
	void UpdateModelMatrix();

	//Texture * texture;
	//std::string textureFilePath;
};