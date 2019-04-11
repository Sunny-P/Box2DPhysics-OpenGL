#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include "Player.h"
#include "Camera.h"
#include "Model.h"

Player::Player()
{
	maxLives = 3;
	currentLives = maxLives;

	speed = 5.0f;
	turnSpeed = 180.0f;

	shininess = 35.0f;
}

Player::~Player()
{
	
}

void Player::init(Model* _playerObj)
{
	playerObj = _playerObj;

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = playerObj->GetRotation();
	rotationAngle = playerObj->GetRotationAngle();
	scale = glm::vec3(0.01f, 0.01f, 0.01f);
}

void Player::render()
{
	if (playerObj != nullptr)
	{
		playerObj->Render(modelMatrix);
	}
}

void Player::update(GLfloat _deltaTime)
{
	
	deltaTime = _deltaTime;
	if (playerObj != nullptr)
	{
		position = playerObj->GetPosition();
		rotation = playerObj->GetRotation();
		rotationAngle = playerObj->GetRotationAngle();

		float cubeRot = playerObj->GetRotationAngle();
		distance = speed * deltaTime;
		moveVec.y = 0.0f;

		float x1, x2, z1, z2;
		x1 = controlledCam->GetCamPos().x;
		z1 = controlledCam->GetCamPos().z;
		x2 = position.x;
		z2 = position.z;

		newX = x1 - x2;
		newZ = z1 - z2;
		float u;
		u = sqrt(pow(newX, 2) + pow(newZ, 2));
		newX = newX / u;
		newZ = newZ / u;

		float newAngle;
		newAngle = turnSpeed * deltaTime;
		glm::vec3 tempLoc = position;
		if (rotateLeft == true)
		{
			//playerCube->IncrementPosition(-tempLoc.x, -tempLoc.y, -tempLoc.z);
			playerObj->IncrementRotation(newAngle);
			//playerCube->IncrementPosition(tempLoc.x, tempLoc.y, tempLoc.z);
		}
		else if (rotateRight == true)
		{
			//playerCube->IncrementPosition(-tempLoc.x, -tempLoc.y, -tempLoc.z);
			playerObj->IncrementRotation(-newAngle);
			//playerCube->IncrementPosition(tempLoc.x, tempLoc.y, tempLoc.z);
		}
	}
	UpdateModelMatrix();
}

void Player::SetControlledCam(Camera * camera)
{
	controlledCam = camera;
}

void Player::processInput(bool _birdsEye)
{
	if (playerObj != nullptr)
	{

		if (Keystate['w'] == DOWN)
		{
			if (_birdsEye)
			{
				moveVec.x = 0.0f;
				moveVec.z = -distance;
			}
			else
			{

				moveVec.x = newX * -distance;
				moveVec.z = newZ * -distance;
			}

			position += moveVec;
			playerObj->IncrementPositionVec(moveVec);
		}
		if (Keystate['a'] == DOWN)
		{
			if (_birdsEye)
			{
				moveVec.x = -distance;
				moveVec.z = 0.0f;

				position += moveVec;
				playerObj->IncrementPositionVec(moveVec);
			}
			else
			{
				rotateLeft = true;
			}
		}
		else
		{
			if (_birdsEye)
			{

			}
			else
			{
				rotateLeft = false;
			}
		}
		if (Keystate['s'] == DOWN)
		{
			if (_birdsEye)
			{
				moveVec.z = distance;
				moveVec.x = 0.0f;
			}
			else
			{
				moveVec.x = newX * distance;
				moveVec.z = newZ * distance;
			}

			position += moveVec;
			playerObj->IncrementPositionVec(moveVec);
		}
		if (Keystate['d'] == DOWN)
		{
			if (_birdsEye)
			{
				moveVec.x = distance;
				moveVec.z = 0.0f;

				position += moveVec;
				playerObj->IncrementPositionVec(moveVec);
			}
			else
			{
				rotateRight = true;
			}
		}
		else
		{
			if (_birdsEye)
			{

			}
			else
			{
				rotateRight = false;
			}
		}
		// Strafing
		if (Keystate['q'] == DOWN && ((Keystate['w'] != DOWN) && (Keystate['s'] != DOWN)))
		{
			if (_birdsEye)
			{
				//rotateLeft = true;
			}
			else
			{
				moveVec.x = -newZ * distance;
				moveVec.z = newX * distance;

				position += moveVec;
				playerObj->IncrementPositionVec(moveVec);
			}
		}
		else if (Keystate['q'] == DOWN && ((Keystate['w'] == DOWN) || (Keystate['s'] == DOWN)))
		{
			if (_birdsEye)
			{
				//rotateLeft = true;
			}
			else
			{
				float u;
				u = sqrt(pow(newX, 2) + pow(newZ, 2));

				moveVec.x = (-newZ / u) * distance;
				moveVec.z = (newX / u) * distance;

				//glm::normalize(moveVec);
				position += moveVec;
				playerObj->IncrementPositionVec(moveVec);
			}
		}
		if (Keystate['e'] == DOWN && ((Keystate['w'] != DOWN) && (Keystate['s'] != DOWN)))
		{
			if (_birdsEye)
			{
				//rotateRight = true;
			}
			else
			{
				moveVec.x = newZ * distance;
				moveVec.z = -newX * distance;

				position += moveVec;
				playerObj->IncrementPositionVec(moveVec);
			}
		}
		else if (Keystate['e'] == DOWN && ((Keystate['w'] == DOWN) || (Keystate['s'] == DOWN)))
		{
			if (_birdsEye)
			{
				//rotateRight = true;
			}
			else
			{
				float u;
				u = sqrt(pow(newX, 2) + pow(newZ, 2));

				moveVec.x = (newZ / u) * distance;
				moveVec.z = (-newX / u) * distance;

				//glm::normalize(moveVec);
				position += moveVec;
				playerObj->IncrementPositionVec(moveVec);
			}
		}
	}
}

void Player::keyboardDown(unsigned char key, int x, int y)
{
	Keystate[key] = DOWN;
}

void Player::keyboardUp(unsigned char key, int x, int y)
{
	Keystate[key] = UP;
}

int Player::GetCurrentLives()
{
	return currentLives;
}

void Player::IncrementLives(int _changeInLives)
{
	currentLives += _changeInLives;
}

glm::vec3 Player::GetPosition()
{
	return position;
}

glm::vec3 Player::GetRotation()
{
	return rotation;
}

float Player::GetRotationAngle()
{
	return rotationAngle;
}

void Player::UpdateModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(), position)
				* glm::rotate(glm::mat4(), glm::radians(rotationAngle), rotation)
				* glm::scale(glm::mat4(), scale);
}

void Player::SetShininess(float newShininess)
{
	shininess = newShininess;
}

float Player::GetShininess()
{
	return shininess;
}

void Player::IncrementPosition(float _incXBy, float _incYBy, float _incZBy)
{
	position.x += _incXBy;
	position.y += _incYBy;
	position.z += _incZBy;
}

void Player::IncrementRotation(float _degrees)
{
	rotationAngle += _degrees;
}

void Player::IncrementScale(float _incXBy, float _incYBy, float _incZBy)
{
	scale.x += _incXBy;
	scale.y += _incYBy;
	scale.z += _incZBy;
}

void Player::SetRotationAngle(float _angle)
{
	rotationAngle = _angle;
}

void Player::IncrementPositionVec(glm::vec3 _incrementVec)
{
	position += _incrementVec;
}

void Player::SetPosition(float _XPos, float _YPos, float _ZPos)
{
	position.x += _XPos;
	position.y += _YPos;
	position.z += _ZPos;
}

void Player::SetRotation(float _angle)
{
	rotationAngle = _angle;
}

void Player::SetScale(float _XScale, float _YScale, float _ZScale)
{
	scale.x = _XScale;
	scale.y = _YScale;
	scale.z = _ZScale;
}

glm::vec3 Player::GetScale()
{
	return scale;
}