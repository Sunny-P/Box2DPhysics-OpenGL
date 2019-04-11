#include "Model.h"

int Model::GetMeshSize()
{
	return meshes.size();
}

void Model::SetActiveCamera(Camera * _camera)
{
	this->camera = _camera;
}


glm::vec3 Model::GetPosition()
{
	return position;
}

glm::vec3 Model::GetRotation()
{
	return rotation;
}

float Model::GetRotationAngle()
{
	return rotationAngle;
}

void Model::SetShininess(float newShininess)
{
	shininess = newShininess;
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].SetShininess(shininess);
	}
}

float Model::GetShininess()
{
	return shininess;
}

void Model::IncrementPosition(float _incXBy, float _incYBy, float _incZBy)
{
	position.x += _incXBy;
	position.y += _incYBy;
	position.z += _incZBy;
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		//this->meshes[i].IncrementPosition(_incXBy, _incYBy, _incZBy);
		this->meshes[i].SetPosition(position.x, position.y, position.z);
	}
}

void Model::IncrementRotation(float _degrees)
{
	rotationAngle += _degrees;
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].SetRotation(rotationAngle);
	}
}

void Model::IncrementScale(float _incXBy, float _incYBy, float _incZBy)
{
	scale.x += _incXBy;
	scale.y += _incYBy;
	scale.z += _incZBy;
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].SetScale(scale);
	}
}

void Model::SetRotationAngle(float _angle)
{
	rotationAngle = _angle;
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].SetRotationAngle(rotationAngle);
	}
}

void Model::IncrementPositionVec(glm::vec3 _incrementVec)
{
	position += _incrementVec;
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		//this->meshes[i].IncrementPosition(_incXBy, _incYBy, _incZBy);
		this->meshes[i].SetPosition(position.x, position.y, position.z);
	}
}

void Model::SetPosition(float _XPos, float _YPos, float _ZPos)
{
	position.x += _XPos;
	position.y += _YPos;
	position.z += _ZPos;
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		//this->meshes[i].IncrementPosition(_incXBy, _incYBy, _incZBy);
		this->meshes[i].SetPosition(position.x, position.y, position.z);
	}
}

void Model::SetRotation(float _angle)
{
	rotationAngle = _angle;
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].SetRotation(rotationAngle);
	}
}

void Model::SetScale(float _XScale, float _YScale, float _ZScale)
{
	scale.x = _XScale;
	scale.y = _YScale;
	scale.z = _ZScale;
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].SetScale(scale);
	}
}

glm::vec3 Model::GetScale()
{
	return scale;
}
