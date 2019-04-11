#include "Enemy.h"
#include "Model.h"
#include "Camera.h"

#include <time.h>
#include <algorithm>

Enemy::Enemy(Model * _enemyModel, Camera* _camera)
{
	camera = _camera;
	maxSpeed = 3.0f;
	currentSpeed = 0.0f;
	turnSpeed = 120.0f;

	wanderInit = false;
	wanderRadius = 8.0f;
	wanderDist = 15.0f;
	wanderAngle = 0.0f;

	arrivalDistance = 5.0f;

	containInit = false;

	maxForce = 0.3f;

	enemyObj = _enemyModel;
	position = _enemyModel->GetPosition();
	rotation = _enemyModel->GetRotation();
	rotationAngle = _enemyModel->GetRotationAngle();
	scale = glm::vec3(0.01f, 0.01f, 0.01f);

	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	shininess = 20.0f;

	currentState = WANDER;
}

Enemy::~Enemy()
{
}

void Enemy::init()
{
}

void Enemy::render()
{
	enemyObj->Render(modelMatrix);
}

void Enemy::update(float _deltaTime, bool wanderContained, float leftX, float rightX, float topZ, float botZ)
{
	maxForce = 5.0f;
	maxForce *= _deltaTime;

	velocity += acceleration * _deltaTime;
	velocity = glm::clamp(velocity, -maxSpeed * _deltaTime, maxSpeed * _deltaTime);
	position += velocity;
	acceleration *= 0;

	HeadingAngle();
	switch (currentState)
	{
	case 0:	// WANDER
		Wander(_deltaTime);
		if (wanderContained)
		{
			Containment(_deltaTime, leftX, rightX, topZ, botZ);
		}
		break;
	case 1:	// SEEK
		Seek(_deltaTime);
		break;
	case 2:	// ARRIVAL
		Arrival(_deltaTime);
		break;
	case 3:	// CONTAINMENT
		Containment(_deltaTime, leftX, rightX, topZ, botZ);
		break;
	case 4:	// SEEK_OBST_AVOID

		break;
	case 5: // IDLE

		break;
	default:
		break;
	}
	
	UpdateModelMatrix();

	//Seek();
}

glm::vec3 Enemy::GetPosition()
{
	return glm::vec3();
}

glm::vec3 Enemy::GetRotation()
{
	return glm::vec3();
}

float Enemy::GetRotationAngle()
{
	return 0.0f;
}

void Enemy::SetCurrentState(State _newState)
{
	currentState = _newState;
}

void Enemy::HeadingAngle()
{
	if (targetPosition != position)
	{
		rotationAngle = glm::degrees(glm::atan(velocity.x / velocity.z));// +180.0f;
		if (velocity.z < 0.0f)
		{
			rotationAngle += 180.0f;
		}

		enemyObj->SetRotationAngle(rotationAngle);

		/*float dotProd = position.x*targetPosition.x + position.y*targetPosition.y + position.z*targetPosition.z;
		float positionMag, targetPosMag;
		positionMag = sqrt(position.x*position.x + position.z*position.z);
		targetPosMag = sqrt(targetPosition.x*targetPosition.x + targetPosition.z*targetPosition.z);
		rotationAngle = glm::degrees(glm::acos(dotProd / (positionMag*targetPosMag)));*/
	}
}

void Enemy::SetTargetLocation(glm::vec3 _targetLocation)
{
	targetPosition = _targetLocation - position;
	distance = sqrt(pow(_targetLocation.x - position.x, 2.0f) + pow(_targetLocation.z - position.z, 2.0f));
	if (distance <= arrivalDistance)
	{
		if (distance <= 1.0f)
		{
			currentSpeed = 0.0f;
		}
		else
		{
			currentSpeed = maxSpeed * (distance / arrivalDistance);
		}
	}
}

void Enemy::Seek(float _deltaTime)
{
	targetPosition.x = glm::normalize(targetPosition.x);
	targetPosition.z = glm::normalize(targetPosition.z);

	targetPosition *= maxSpeed * _deltaTime;

	steer = targetPosition - velocity;
	steer = glm::clamp(steer, -maxForce, maxForce);

	ApplyForce(steer);
}

void Enemy::Wander(float _deltaTime)
{
	// If we define Wandering as the following:
	// “Wandering is a type of random steering which has some long term order : 
	// the steering direction on one frame is related to the steering direction on the next frame.
	//This produces more interesting motion than, for example, simply generating a random steering direction each frame.” -Craig Reynolds

	// Get the first initial targetPosition when wandering
	if (wanderInit == false)
	{
		// Based on the distance it looks ahead to wander to, get a point randomly on the radius from the wander distance.
		float min, max;
		min = position.x - (wanderDist - wanderRadius);
		max = position.x + (wanderDist + wanderRadius);
		// Determine x of target wander vector
		targetPosition.x = min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));

		min = position.z - (wanderDist - wanderRadius);
		max = position.z + (wanderDist + wanderRadius);
		// Determine z of target wander vector
		targetPosition.z = min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
		wanderInit = true;
	}
	// Continue wandering
	else
	{
		// From the targetPosition it is wandering to, get a point randomly on the radius from the wander position
		//float angleToPoint;
		//glm::vec3 circleCentre = glm::normalize(targetPosition - position);
		//circleCentre *= wanderDist;
		//angleToPoint = glm::atan((targetPosition.z - position.z) / (targetPosition.x - position.x));

		float min, max;
		min = targetPosition.x - wanderRadius;
		max = targetPosition.x + wanderRadius;
		// Determine x of target wander vector
		targetPosition.x = min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));

		min = targetPosition.z - wanderRadius;
		max = targetPosition.z + wanderRadius;
		// Determine z of target wander vector
		targetPosition.z = min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
	}
	//targetPosition.x = glm::normalize(targetPosition.x);
	//targetPosition.z = glm::normalize(targetPosition.z);
	//glm::normalize(targetPosition);
	targetPosition *= (maxSpeed * _deltaTime);

	steer = targetPosition - velocity;
	steer = glm::clamp(steer, -maxForce, maxForce);
	ApplyForce(steer);
}

void Enemy::Arrival(float _deltaTime)
{
	glm::vec3 desiredLoc;
	
	if (distance <= arrivalDistance)
	{
		// If the distance between the "Enemy" and the targetLocation is less than the determined radius it needs to start arriving (the 'arrivalDistance')
		// reduce the desired location dependant on how close it has gotten, slowing it down the closer it gets
		desiredLoc = targetPosition / distance*(maxSpeed * _deltaTime)*(distance / arrivalDistance);
		//targetPosition = targetPosition / distance*(maxSpeed * _deltaTime)*(distance / arrivalDistance);
	}
	else
	{
		// Otherwise, move at maxSpeed
		desiredLoc = targetPosition / distance*(maxSpeed * _deltaTime);
		//targetPosition = targetPosition /  distance*(maxSpeed * _deltaTime);
	}
	
	steer = desiredLoc - velocity;
	steer = glm::clamp(steer, -maxForce, maxForce);
	ApplyForce(steer);
}

void Enemy::Containment(float _deltaTime, float leftX, float rightX, float topZ, float botZ)
{
	// If we define Containment as:
	// "If a vehicle comes within a distance 'd' of a wall, it desires to move at maximum speed in the opposite direction of the wall".

	// 4.0f is our distance from the wall
	if (position.x > -4.0f + rightX)
	{
		if (currentState == WANDER)
		{
			if (position.x < -4.0f - rightX)
			targetPosition = glm::vec3(-maxSpeed, velocity.y, velocity.z);
		}
		else
		{
			targetPosition = glm::vec3(-maxSpeed, velocity.y, velocity.z);
		}
	}
	if (position.x < 4.0f + leftX)
	{
		if (currentState == WANDER)
		{
			if (position.x > 4.0f - leftX)
			{
				targetPosition = glm::vec3(maxSpeed, velocity.y, velocity.z);
			}
		}
		else
		{
			targetPosition = glm::vec3(maxSpeed, velocity.y, velocity.z);
		}
	}
	if (position.z > -4.0f + botZ)
	{
		if (currentState == WANDER)
		{
			if (position.z < -4.0f - botZ)
			{
				targetPosition = glm::vec3(velocity.x, velocity.y, -maxSpeed);
			}
		}
		else
		{
			targetPosition = glm::vec3(velocity.x, velocity.y, -maxSpeed);
		}
	}
	if (position.z < 4.0f + topZ)
	{
		if (currentState == WANDER)
		{
			if (position.z > 4.0f - topZ)
			{
				targetPosition = glm::vec3(velocity.x, velocity.y, maxSpeed);
			}
		}
		else
		{
			targetPosition = glm::vec3(velocity.x, velocity.y, maxSpeed);
		}
	}

	targetPosition.x = glm::normalize(targetPosition.x);
	targetPosition.z = glm::normalize(targetPosition.z);
	targetPosition *= maxSpeed * _deltaTime;

	steer = targetPosition - velocity;
	steer = glm::clamp(steer, -maxForce, maxForce);

	ApplyForce(steer);
}

void Enemy::ObstacleAvoidance(float _deltaTime)
{

}

void Enemy::IncrementVelocity(glm::vec3 _velInc)
{
	velocity += _velInc;
}

void Enemy::UpdateModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(), position)
				* glm::rotate(glm::mat4(), glm::radians(rotationAngle), rotation)
				* glm::scale(glm::mat4(), scale);
}

void Enemy::ApplyForce(glm::vec3 force)
{
	acceleration += force;
}

void Enemy::SetShininess(float newShininess)
{
	shininess = newShininess;
}

float Enemy::GetShininess()
{
	return shininess;
}

void Enemy::IncrementPosition(float _incXBy, float _incYBy, float _incZBy)
{
	position.x += _incXBy;
	position.y += _incYBy;
	position.z += _incZBy;
}

void Enemy::IncrementRotation(float _degrees)
{
	rotationAngle += _degrees;
}

void Enemy::IncrementScale(float _incXBy, float _incYBy, float _incZBy)
{
	scale.x += _incXBy;
	scale.y += _incYBy;
	scale.z += _incZBy;
}

void Enemy::SetRotationAngle(float _angle)
{
	rotationAngle = _angle;
}

void Enemy::IncrementPositionVec(glm::vec3 _incrementVec)
{
	position += _incrementVec;
}

void Enemy::SetPosition(float _XPos, float _YPos, float _ZPos)
{
	position.x += _XPos;
	position.y += _YPos;
	position.z += _ZPos;
}

void Enemy::SetRotation(float _angle)
{
	rotationAngle = _angle;
}

void Enemy::SetScale(float _XScale, float _YScale, float _ZScale)
{
	scale.x = _XScale;
	scale.y = _YScale;
	scale.z = _ZScale;
}

State Enemy::GetCurrentState()
{
	return currentState;
}

glm::vec3 Enemy::GetScale()
{
	return scale;
}