#pragma once
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include "Utility.h"

class Player;

enum viewType {
	ORTHO = 0,
	PERSP,
	PERSP_FOLLOW_PLAYER,
};

class Camera
{
public:
	Camera(Player* _selectedPlayer);
	Camera(viewType _viewType = PERSP);
	~Camera();

	//void init();
	void update(GLfloat _deltaTime);

	

	//glm::mat4 GetCamProj();
	//glm::mat4 GetCamView();
	

	glm::mat4 GetPVMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	void SetCamPos(glm::vec3 newPos);
	glm::vec3 GetCamPos();

	void SetCamLookDir(glm::vec3 newLookDir);
	glm::vec3 GetCamLookDir();

	void SetCamUpDir(glm::vec3 newUpDir);
	glm::vec3 GetCamUpDir();

	void SetCamViewType(viewType _viewType);

	void IncrementPos(float incX, float incY, float incZ);

	int GetCamViewType();

	void processInput();
	void keyboardDown(unsigned char key, int x, int y);
	void keyboardUp(unsigned char key, int x, int y);

protected:
	viewType currentViewType;
	glm::vec3 camPos;
	glm::vec3 camLookDir;
	glm::vec3 camUpDir;

	glm::mat4 view;
	glm::mat4 proj;

	GLfloat deltaTime;
	GLfloat radiusFromPlayer;
	GLfloat rotationOffsetFromPlayer;
	GLfloat timeElapsed;

	//float oldRotationAngle;
	//glm::vec2 oldPos;
	float rotationAngle;

	Player* selectedPlayer;

	enum InputState
	{
		UP,
		DOWN,
		UP_FIRST,
		DOWN_FIRST
	};

	InputState Keystate[255];
};