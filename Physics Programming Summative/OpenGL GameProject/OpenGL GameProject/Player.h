#pragma once
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include <cmath>
#include <math.h>

class Model;
class Camera;

class Player
{
public:
	Player();
	~Player();

	void init(Model* _playerObj);
	void render();
	void update(GLfloat deltaTime);

	void SetControlledCam(Camera* camera);

	// keyboard input
	void processInput(bool _birdsEye = false);
	void keyboardDown(unsigned char key, int x, int y);
	void keyboardUp(unsigned char key, int x, int y);

	int GetCurrentLives();
	void IncrementLives(int _changeInLives);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	float GetRotationAngle();

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

	glm::vec3 GetScale();

protected:
	Model* playerObj;
	Camera* controlledCam;
	bool rotateLeft;
	bool rotateRight;

	float speed;
	float turnSpeed;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	float rotationAngle;

	float shininess;

	float distance;
	float newX, newZ;
	glm::vec3 moveVec;

	glm::mat4 modelMatrix;
	void UpdateModelMatrix();

	enum InputState
	{
		UP,
		DOWN,
		UP_FIRST,
		DOWN_FIRST
	};

	InputState Keystate[255];

	GLfloat deltaTime;

	int maxLives;
	int currentLives;
};