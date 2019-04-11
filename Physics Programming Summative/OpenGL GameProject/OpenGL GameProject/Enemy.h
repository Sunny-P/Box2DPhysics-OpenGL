#pragma once
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

class Model;
class Camera;

enum State {
	WANDER = 0,
	SEEK,
	ARRIVAL,
	CONTAINMENT,
	SEEK_OBST_AVOID,
	IDLE
};

class Enemy {
public:
	Enemy(Model* _enemyModel, Camera* _camera);
	~Enemy();

	void init();
	void render();
	void update(float _deltaTime, bool wanderContained = false,float leftX = 0.0f, float rightX = 0.0f, float topZ = 0.0f, float botZ = 0.0f);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	float GetRotationAngle();

	void SetCurrentState(State _newState);
	void HeadingAngle();

	void SetTargetLocation(glm::vec3 _targetLocation);
	void Seek(float _deltaTime);
	void Wander(float _deltaTime);
	void Arrival(float _deltaTime);
	void Containment(float _deltaTime, float leftX = 0.0f, float rightX = 0.0f, float topZ = 0.0f, float botZ = 0.0f);
	void ObstacleAvoidance(float _deltaTime);

	void IncrementVelocity(glm::vec3 _velInc);

	void SetShininess(float newShininess);
	float GetShininess();

	void IncrementPosition(float _incXBy, float _incYBy, float _incZBy);
	void IncrementRotation(float _degrees);
	void IncrementScale(float _incXBy, float _incYBy, float _incZBy);

	void SetRotationAngle(float _angle);

	void IncrementPositionVec(glm::vec3 _incrementVec);

	void SetPosition(float _XPos, float _YPos, float _ZPos);
	void SetRotation(float _angle);
	void SetScale(float _XScale, float _YScale, float _ZScale);

	State GetCurrentState();

	glm::vec3 GetScale();

protected:
	Model * enemyObj;
	Camera* camera;

	float maxSpeed;
	float currentSpeed;

	float wanderRadius;
	float wanderDist;
	bool wanderInit;
	float wanderAngle;

	bool containInit;

	float arrivalDistance;
	float distance;

	float turnSpeed;

	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 steer;
	float maxForce;

	glm::vec3 targetPosition;
	glm::vec3 futurePosition;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	float rotationAngle;

	float shininess;

	glm::vec3 moveVec;

	glm::mat4 modelMatrix;
	void UpdateModelMatrix();

	State currentState;

	void ApplyForce(glm::vec3 force);
};