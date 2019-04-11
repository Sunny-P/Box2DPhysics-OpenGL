#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes

// EDIT
#include "Utility.h"
#include "Camera.h"

#include <assimp/types.h>
// EDIT END

struct lightInfo {
	// Lighting
	float ambientStr;
	glm::vec3 ambientColour;
	glm::vec3 ambient;
	glm::vec3 lightColour;
	glm::vec3 lightPos;

	// Specular strength
	float specularStr;
};

struct MeshTexture {
	GLuint id;
	std::string type;
	aiString path;
};

struct Vertex {

	glm::vec3 Position; // Position	
	glm::vec3 Normal; // Normal	
	glm::vec2 TexCoords; // TexCoords
};

class ModelMesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<MeshTexture> textures;

	/*  Functions  */
	// Constructor
	ModelMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<MeshTexture> textures);

	// Render the mesh
	void Render(Camera* camera, GLuint program, lightInfo _lighting, glm::mat4 _model);

	void SetScale(glm::vec3 _scaleVec);

	void CalculateModelMatrix();

	void SetShininess(float newShininess);

	void IncrementPosition(float _incXBy, float _incYBy, float _incZBy);
	void IncrementRotation(float _degrees);
	void IncrementScale(float _incXBy, float _incYBy, float _incZBy);

	void SetRotationAngle(float _angle);

	void IncrementPositionVec(glm::vec3 _incrementVec);

	void SetPosition(float _XPos, float _YPos, float _ZPos);
	void SetRotation(float _angle);

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	
	float rotationAngle;

	glm::mat4 model;
	glm::mat4 mvp;

	float shininess;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};