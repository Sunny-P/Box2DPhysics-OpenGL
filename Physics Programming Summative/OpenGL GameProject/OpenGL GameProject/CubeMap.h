#pragma once
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include <vector>
#include <string>

class Camera;

class CubeMap {
public:
	CubeMap(Camera* _camera, std::vector<std::string> _vecFilePaths);
	~CubeMap();

	void Update();
	void Render(GLuint _program);

	GLuint GetTexID();

protected:
	GLuint program;
	GLuint VAO, VBO, EBO;

	GLuint texID;

	glm::mat4 MVP;

	Camera* camera;

	int indexCount;
	int drawType;

	// TO DO: RE-WORK OUT THESE VERTICES TO MAKE SURE YOU HAVE IT CORRECT
	// ASLSO RE-WORK OUT THE INDICES ARRAY TO REFLECT THE CHANGES
	GLfloat vertices[72]{
		// Position						
		//		Front Face
		/*0*/	-0.5f, 0.5f, 0.5f,		
		/*1*/	-0.5f, -0.5f, 0.5f,		
		/*2*/	0.5f, -0.5f, 0.5f,		
		/*3*/	0.5f, 0.5f, 0.5f,		

		//		Back
		/*4*/	-0.5f, 0.5f, -0.5f,		
		/*5*/	-0.5f, -0.5f, -0.5f,	
		/*6*/	0.5f, -0.5f, -0.5f,		
		/*7*/	0.5f, 0.5f, -0.5f,		

		//		Left
		/*8*/	-0.5f, 0.5f, -0.5f,		
		/*9*/	-0.5f, -0.5f, -0.5f,	
		/*10*/	-0.5f, -0.5f, 0.5f,		
		/*11*/	-0.5f, 0.5f, 0.5f,		

		//		Right
		/*12*/	0.5f, 0.5f, 0.5f,		
		/*13*/	0.5f, -0.5f, 0.5f,		
		/*14*/	0.5f, -0.5f, -0.5f,		
		/*15*/	0.5f, 0.5f, -0.5f,		

		//		Top
		/*16*/	-0.5f, 0.5f, -0.5f,		
		/*17*/	-0.5f, 0.5f, 0.5f,		
		/*18*/	0.5f, 0.5f, 0.5f,		
		/*19*/	0.5f, 0.5f, -0.5f,		

		//		Bottom
		/*20*/	-0.5f, -0.5f, 0.5f,		
		/*21*/	-0.5f, -0.5f, -0.5f,	
		/*22*/	0.5f, -0.5f, -0.5f,		
		/*23*/	0.5f, -0.5f, 0.5f
	};

	GLuint indices[36]{
		// Front
		0, 2, 1,
		0, 3, 2,

		7, 5, 6,
		7, 4, 5,

		8, 10, 9,
		8, 11, 10,

		12, 14, 13,
		12, 15, 14,

		16, 18, 17,
		16, 19, 18,

		20, 22, 21,
		20, 23, 22
	};
};