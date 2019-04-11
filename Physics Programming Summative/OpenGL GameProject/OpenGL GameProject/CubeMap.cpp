#include "CubeMap.h"
#include "Camera.h"

CubeMap::CubeMap(Camera * _camera, std::vector<std::string> _vecFilePaths)
{
	camera = _camera;

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
		3 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);
	/*glVertexAttribPointer(
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
	glEnableVertexAttribArray(2);*/

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	indexCount = sizeof(indices) / sizeof(GLuint);
	drawType = GL_TRIANGLES;

	int width, height;
	unsigned char* image;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	for (GLuint i = 0; i < 6; i++)
	{
		std::string fullPathName = "Resources/Shaders/CubeMap/";
		fullPathName.append(_vecFilePaths[i]);

		image = SOIL_load_image(fullPathName.c_str(),
								&width,
								&height,
								0,
								SOIL_LOAD_RGBA);
		glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						0,
						GL_RGBA,
						width,
						height,
						0,
						GL_RGBA,
						GL_UNSIGNED_BYTE,
						image);
		SOIL_free_image_data(image);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
}

CubeMap::~CubeMap()
{
}

void CubeMap::Update()
{
	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f)) 
					* glm::scale(glm::mat4(), glm::vec3(2000.0f, 2000.0f, 2000.0f));
	MVP = camera->GetPVMatrix() * model;
}

void CubeMap::Render(GLuint _program)
{
	glUseProgram(_program);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	glUniform1i(glGetUniformLocation(_program, "cubeSampler"), 0);
	glUniformMatrix4fv(glGetUniformLocation(_program, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	glBindVertexArray(VAO);
	glDrawElements(drawType, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

GLuint CubeMap::GetTexID()
{
	return texID;
}
