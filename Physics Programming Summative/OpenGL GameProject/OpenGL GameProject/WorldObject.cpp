#include "WorldObject.h"
#include "Model.h"

WorldObject::WorldObject(Model * _objectModel, bool _isCollectable)
{
	objectModel = _objectModel;

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	zRotation = glm::vec3(0.0f, 0.0f, 1.0f);
	xRotation = glm::vec3(1.0f, 0.0f, 0.0f);

	width = 0.2f;
	height = 0.2f;
	length = 0.2f;
	scale = glm::vec3(width, height, length);

	xRotationAngle = 0.0f;
	yRotationAngle = 0.0f;
	zRotationAngle = 0.0f;

	shininess = 20.0f;

	isCollectable = _isCollectable;
}

WorldObject::~WorldObject()
{
}

void WorldObject::render()
{
	if (objectModel != nullptr)
	{
		objectModel->Render(modelMatrix);
	}
}

void WorldObject::update(float deltaTime)
{
	if (isCollectable)
	{
		// Have the gameObjects rotate if they are collectables
		// One full rotation every 4 seconds
		yRotationAngle += 90.0f * deltaTime;
	}

	UpdateModelMatrix();
}

void WorldObject::IncrementPosition(glm::vec3 incVec)
{
	position += incVec;
}

void WorldObject::IncrementRotation(glm::vec3 degreesVec)
{
	xRotationAngle += degreesVec.x;
	yRotationAngle += degreesVec.y;
	zRotationAngle += degreesVec.z;
}

void WorldObject::IncrementScale(glm::vec3 incScale)
{
	scale += incScale;
	length += incScale.z;
	height += incScale.y;
	width += incScale.x;
}

void WorldObject::SetPosition(glm::vec3 positionVec)
{
	position = positionVec;
}

void WorldObject::SetRotationAngle(glm::vec3 degreesVec)
{
	xRotationAngle = degreesVec.x;
	yRotationAngle = degreesVec.y;
	zRotationAngle = degreesVec.z;
}

void WorldObject::SetScale(glm::vec3 scaleVec)
{
	scale = scaleVec;
	length = scaleVec.z;
	height = scaleVec.y;
	width = scaleVec.x;
}

void WorldObject::SetShininess(float newShininess)
{
	shininess = newShininess;
}

float WorldObject::GetShininess()
{
	return shininess;
}

glm::vec3 WorldObject::GetPosition()
{
	return position;
}

glm::vec3 WorldObject::GetRotation()
{
	return rotation;
}

float WorldObject::GetRotationAngle()
{
	return yRotationAngle;
}

void WorldObject::UpdateModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(), position)
				* (glm::rotate(glm::mat4(), glm::radians(yRotationAngle), rotation) * glm::rotate(glm::mat4(), glm::radians(zRotationAngle), zRotation) * glm::rotate(glm::mat4(), glm::radians(xRotationAngle), xRotation))
				* glm::scale(glm::mat4(), scale);
}
