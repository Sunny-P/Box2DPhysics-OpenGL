#include "ModelMesh.h"

ModelMesh::ModelMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<MeshTexture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	// Now that we have all the required data, set the vertex buffers and its attribute pointers.
	this->setupMesh();
	scale = glm::vec3(0.2f, 0.2f, 0.2f);
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	rotationAngle = 0.0f;
	this->shininess = 20.0f;
}

void ModelMesh::Render(Camera * camera, GLuint program, lightInfo _lighting, glm::mat4 _model)
{
	glUseProgram(program);

	// Bind appropriate textures
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
										  // Retrieve texture number (the N in diffuse_textureN)
		stringstream ss;
		string number;
		string name = this->textures[i].type;
		if (name == "texture_diffuse")
			ss << diffuseNr++; // Transfer GLuint to stream
		else if (name == "texture_specular")
			ss << specularNr++; // Transfer GLuint to stream
		number = ss.str();
		// Now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	// EDIT
	//CalculateModelMatrix();
	if (camera != nullptr)
	{
		mvp = camera->GetPVMatrix() * _model;
		glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	}
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMat"), 1, GL_FALSE, glm::value_ptr(_model));
	glUniform1f(glGetUniformLocation(program, "shininess"), shininess);
	glUniform3fv(glGetUniformLocation(program, "ambient"), 1, glm::value_ptr(_lighting.ambient));
	glUniform3fv(glGetUniformLocation(program, "lightColour"), 1, glm::value_ptr(_lighting.lightColour));
	glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, glm::value_ptr(_lighting.lightPos));
	glUniform1f(glGetUniformLocation(program, "lightSpecStr"), _lighting.specularStr);
	// EDIT END

	// Draw mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Always good practice to set everything back to defaults once configured.
	for (GLuint i = 0; i < this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void ModelMesh::SetScale(glm::vec3 _scaleVec)
{
	scale = _scaleVec;
}

void ModelMesh::CalculateModelMatrix()
{
	model = glm::translate(glm::mat4(), position)
		* glm::rotate(glm::mat4(), glm::radians(rotationAngle), rotation)
		* glm::scale(glm::mat4(), scale);
}

void ModelMesh::SetShininess(float newShininess)
{
	shininess = newShininess;
}

void ModelMesh::IncrementPosition(float _incXBy, float _incYBy, float _incZBy)
{
	
}

void ModelMesh::IncrementRotation(float _degrees)
{
}

void ModelMesh::IncrementScale(float _incXBy, float _incYBy, float _incZBy)
{
}

void ModelMesh::SetRotationAngle(float _angle)
{
	rotationAngle = _angle;
}

void ModelMesh::IncrementPositionVec(glm::vec3 _incrementVec)
{
}

void ModelMesh::SetPosition(float _XPos, float _YPos, float _ZPos)
{
	position.x = _XPos;
	position.y = _YPos;
	position.z = _ZPos;
}

void ModelMesh::SetRotation(float _angle)
{
	rotationAngle = _angle;
}
