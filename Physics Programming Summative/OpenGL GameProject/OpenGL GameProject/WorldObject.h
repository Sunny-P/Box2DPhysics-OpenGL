#pragma once
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

class Model;

class WorldObject {
public:
	WorldObject(Model* _objectModel, bool _isCollectable);
	~WorldObject();

	void render();
	void update(float deltaTime);

	void IncrementPosition(glm::vec3 incVec);
	void IncrementRotation(glm::vec3 degreesVec);
	void IncrementScale(glm::vec3 incScale);

	void SetPosition(glm::vec3 positionVec);
	void SetRotationAngle(glm::vec3 degreesVec);
	void SetScale(glm::vec3 scaleVec);

	void SetShininess(float newShininess);
	float GetShininess();

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	float GetRotationAngle();

protected:
	Model* objectModel = nullptr;

	float width, height, length;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 zRotation;
	glm::vec3 xRotation;
	glm::vec3 scale;

	float yRotationAngle;
	float zRotationAngle;
	float xRotationAngle;

	float shininess;

	bool isCollectable;

	glm::mat4 modelMatrix;
	void UpdateModelMatrix();
};