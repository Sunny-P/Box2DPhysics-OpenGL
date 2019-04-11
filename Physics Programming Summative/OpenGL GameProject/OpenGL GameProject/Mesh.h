#pragma once
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

class Texture;
class Camera;

class Mesh {
public:
	Mesh();
	~Mesh();
	
	virtual void init() = 0;
	//virtual void render();
	//virtual void update();

protected:
	Camera * camera;

	Texture * texture;
	std::string texFilePath;

	GLuint VAO, VBO, EBO;

	glm::mat4 modelMatrix;
};