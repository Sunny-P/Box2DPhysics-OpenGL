#include "Mesh_Sphere.h"
#include "texture.h"

Mesh_Sphere::Mesh_Sphere(GLuint _tex)
{
	radius = 0.5f;

	const int sections = 30;
	const int vertexAttrib = 8;
	const int indexPerQuad = 6;

	double phi = 0;
	double theta = 0;

	float vertices[(sections) * (sections) * vertexAttrib];
	int offset = 0;
	for (int i = 0; i < sections; i++)
	{
		theta = 0;

		for (int j = 0; j < sections; j++)
		{
			float x = float(cos(phi) * sin(theta));
			float y = float(cos(theta));
			float z = float(sin(phi) * sin(theta));

			vertices[offset++] = x * radius;
			vertices[offset++] = y * radius;
			vertices[offset++] = z * radius;
						   
			vertices[offset++] = x;
			vertices[offset++] = y;
			vertices[offset++] = z;
						   
			vertices[offset++] = (float)i / (sections - 1);
			vertices[offset++] = (float)j / (sections - 1);

			theta += (M_PI / (sections - 1));
		}

		phi += (2 * M_PI) / (sections - 1);
	}


	GLuint indices[(sections) * (sections) * indexPerQuad];
	offset = 0;
	for (int i = 0; i < sections; i++)
	{
		for (int j = 0; j < sections; j++)
		{
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);	
			indices[offset++] = (i * sections) + (j);
			indices[offset++] = (((i + 1) % sections) * sections) + (j);
									  
			indices[offset++] = (i * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);		
		}
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	indexCount = sizeof(indices) / sizeof(GLuint);
	drawType = GL_TRIANGLES;

	tex = _tex;
}

Mesh_Sphere::~Mesh_Sphere()
{
	//delete texture;
}

void Mesh_Sphere::init()
{
	//texture = new Texture("Resources/Textures/Rayman.jpg");
	//texture->init();

	position = glm::vec3(0.0f, 0.25f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 1.0f);
	scale = glm::vec3(0.5f, 0.5f, 0.5f);
	rotationAngle = 0.0f;

	shininess = 35.0f;
}

void Mesh_Sphere::render(GLuint _program, GLuint skyboxTexID)
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

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(_program, "skybox"), 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexID);

	glBindVertexArray(VAO);
	glDrawElements(drawType, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Reenable disabled func for others
	glEnable(GL_BLEND);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh_Sphere::update()
{
	UpdateModelMatrix();
}

glm::mat4 Mesh_Sphere::GetModelMatrix()
{
	return modelMatrix;
}

void Mesh_Sphere::IncrementPosition(float _incXBy, float _incYBy, float _incZBy)
{
	position.x += _incXBy;
	position.y += _incYBy;
	position.z += _incZBy;
}

void Mesh_Sphere::IncrementRotation(float _degrees)
{
	rotationAngle += _degrees;
}

void Mesh_Sphere::IncrementScale(float _incXBy, float _incYBy, float _incZBy)
{
	scale.x += _incXBy;
	scale.y += _incYBy;
	scale.z += _incZBy;
}

void Mesh_Sphere::SetPosition(float _XPos, float _YPos, float _ZPos)
{
	position.x = _XPos;
	position.y = _YPos;
	position.z = _ZPos;
}

void Mesh_Sphere::SetRotation(float _angle)
{
	rotationAngle = _angle;
}

void Mesh_Sphere::SetScale(float _XScale, float _YScale, float _ZScale)
{
	scale.x = _XScale;
	scale.y = _YScale;
	scale.z = _ZScale;
}

void Mesh_Sphere::SetShininess(float newShininess)
{
	shininess = newShininess;
}

float Mesh_Sphere::GetShininess()
{
	return shininess;
}

GLuint Mesh_Sphere::GetTexture()
{
	return tex;
}

void Mesh_Sphere::UpdateModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(), position)
		* glm::rotate(glm::mat4(), glm::radians(rotationAngle), rotation)
		* glm::scale(glm::mat4(), scale);
}
