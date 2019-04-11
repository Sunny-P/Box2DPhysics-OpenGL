#include "Camera.h"
#include "Player.h"

Camera::Camera(Player* _selectedPlayer)
{
	selectedPlayer = _selectedPlayer;

	currentViewType = PERSP;
	radiusFromPlayer = 3.25f;
	rotationOffsetFromPlayer = 0.0f;
	rotationAngle = 0.0f;
	//camPos = glm::vec3(0.0f, 3.0f, 0.0f);
	//camLookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	camPos = selectedPlayer->GetPosition();
	camPos.y = 1.0f;
	camPos.z += radiusFromPlayer;
	camPos.x += radiusFromPlayer;
	camLookDir = selectedPlayer->GetPosition();
	camUpDir = glm::vec3(0.0f, 1.0f, 0.0f);

	timeElapsed = (GLfloat)0.0f;
}

Camera::Camera(viewType _viewType)
{
	currentViewType = _viewType;

	camPos = glm::vec3(0.0f, 0.0f, 5.0f);
	camLookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	camUpDir = glm::vec3(0.0f, 1.0f, 0.0f);

	timeElapsed = (GLfloat)0.0f;
}

Camera::~Camera()
{
}

void Camera::update(GLfloat _deltaTime)
{
	deltaTime = _deltaTime;

	GLfloat radius = 3.5f;

	glm::vec3 pointAhead = glm::vec3(camPos.x, camPos.y * 0.5f, camPos.z + 2.0f);

	switch (currentViewType)
	{
	case 0:	// Orthographic cam setting
		view = lookAt(camPos, camPos + camLookDir, camUpDir);
		
		proj = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.1f, 10000.0f);
		break;

	case 1:	// Perspective
		// Cam rotating around point
		// Comment out if unnecessary
		timeElapsed += _deltaTime * 0.075f;
		
		//camPos.y = 1.5f;
		//camPos.x = sin(timeElapsed) *radius;
		//camPos.z = cos(timeElapsed) *radius;

		view = lookAt(camPos, pointAhead, camUpDir);
		proj = glm::perspective(90.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		break;

	case 2:	// Perspective but following player object

		//oldRotationAngle = selectedPlayer->GetRotationAngle();
		rotationAngle = (selectedPlayer->GetRotationAngle() + rotationOffsetFromPlayer) + 90.0f;

		//oldPos.x = (cos(glm::radians(-oldRotationAngle)) * radiusFromPlayer) + selectedPlayer->GetPosition().x;
		//oldPos.y = (sin(glm::radians(-oldRotationAngle)) * radiusFromPlayer) + selectedPlayer->GetPosition().z;

		camPos.x = (cos(glm::radians(-rotationAngle)) * radiusFromPlayer) + selectedPlayer->GetPosition().x;
		camPos.z = (sin(glm::radians(-rotationAngle)) * radiusFromPlayer) + selectedPlayer->GetPosition().z;
		//camPos = selectedPlayer->GetPosition() + glm::vec3(6 * cos(rotationAngle), 4, 6 * sin(rotationAngle));

		view = lookAt(camPos, selectedPlayer->GetPosition(), camUpDir);
		proj = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		break;

	default:
		break;
	}
}

//glm::mat4 Camera::GetCamProj()
//{
//	return proj;
//}
//
//glm::mat4 Camera::GetCamView()
//{
//	return view;
//}

glm::mat4 Camera::GetPVMatrix()
{
	return proj * view;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return proj;
}

glm::mat4 Camera::GetViewMatrix()
{
	return view;
}

void Camera::SetCamPos(glm::vec3 newPos)
{
	camPos = newPos;
}

glm::vec3 Camera::GetCamPos()
{
	return camPos;
}

void Camera::SetCamLookDir(glm::vec3 newLookDir)
{
	camLookDir = newLookDir;
}

glm::vec3 Camera::GetCamLookDir()
{
	return camLookDir;
}

void Camera::SetCamUpDir(glm::vec3 newUpDir)
{
	camUpDir = newUpDir;
}

glm::vec3 Camera::GetCamUpDir()
{
	return camUpDir;
}

void Camera::SetCamViewType(viewType _viewType)
{
	currentViewType = _viewType;
}

void Camera::IncrementPos(float incX, float incY, float incZ)
{
	camPos.x += incX;
	camPos.y += incY;
	camPos.z += incZ;
}

int Camera::GetCamViewType()
{
	return (int)currentViewType;
}

void Camera::processInput()
{
	if (currentViewType == PERSP_FOLLOW_PLAYER)
	{
		// Extend camera distance from player and increase height
		if (Keystate['='] == DOWN)
		{
			if (camPos.y < 5.0f)
			{
				radiusFromPlayer += 2.0f * deltaTime;
				camPos.y += 1.5f * deltaTime;
			}
		}
		// Decrease camera distance from player and decrease height
		if (Keystate['-'] == DOWN)
		{
			if (camPos.y > 0.5f)
			{
				radiusFromPlayer -= 2.0f * deltaTime;
				camPos.y -= 1.5f * deltaTime;
			}
		}

		// Increase camera height and decrease distance from player
		if (Keystate['['] == DOWN)
		{
			if (camPos.y < 5.0f)
			{
				radiusFromPlayer -= 2.0f * deltaTime;
				camPos.y += 1.5f * deltaTime;
			}
		}
		// Decrease camera height and increase distance from player
		if (Keystate[']'] == DOWN)
		{
			if (camPos.y > 0.5f)
			{
				radiusFromPlayer += 2.0f * deltaTime;
				camPos.y -= 1.5f * deltaTime;
			}
		}

		// Rotate camera around player
		if (Keystate['k'] == DOWN)
		{
			rotationOffsetFromPlayer -= 90.0f * deltaTime;
		}
		if (Keystate['l'] == DOWN)
		{
			rotationOffsetFromPlayer += 90.0f * deltaTime;
		}

		// Reset camera to original position to player
		if (Keystate['.'] == DOWN)
		{
			rotationOffsetFromPlayer = 0.0f;
			camPos = selectedPlayer->GetPosition();
			camPos.y = 2.0f;
			camPos.z += radiusFromPlayer;
			camPos.x += radiusFromPlayer;
		}
	}
}

void Camera::keyboardDown(unsigned char key, int x, int y)
{
	Keystate[key] = DOWN;
}

void Camera::keyboardUp(unsigned char key, int x, int y)
{
	Keystate[key] = UP;
}