#pragma once
#include "Input.h"
#include <iostream>
#include "Camera.h"

#include "Utility.h"



Input * Input::instance = NULL;


Input * Input::GetInstance()
{
	if (instance == NULL)
	{
		instance = new Input();
	}
	return instance;
}


void Input::KeyboardDown(unsigned char key, int x, int y)
{
	if (GetInstance()->keyState[key] != DOWN)
	{
		GetInstance()->keyState[key] = DOWN_FIRST;
	}
	
}

void Input::KeyboardUp(unsigned char key, int x, int y)
{
	if (GetInstance()->keyState[key] != UP)
	{
		GetInstance()->keyState[key] = UP_FIRST;
	}
	
}

void Input::SpecialKeyboardDown(int key, int x, int y)
{
	if (GetInstance()->specialKeys[key] != DOWN)
	{
		GetInstance()->specialKeys[key] = DOWN_FIRST;
	}
}

void Input::SpecialKeyboardUp(int key, int x, int y)
{
	if (GetInstance()->specialKeys[key] != UP)
	{
		GetInstance()->specialKeys[key] = UP_FIRST;
	}
}

void Input::MouseClick(int button, int state, int x, int y)
{
	// Don't allow array overflow
	if (button >= 3)
		return;

	GetInstance()->mouseState[button] = (state == GLUT_DOWN) ? DOWN : UP;
}

void Input::MouseActiveMove(int x, int y)
{
	GetInstance()->ScreenSpaceToWorldSpace(x, y);
}

void Input::MousePassiveMove(int x, int y)
{

	GetInstance()->ScreenSpaceToWorldSpace(x, y);
}

int Input::GetMouseState(int button)
{
	return GetInstance()->mouseState[button];
}

char Input::GetKeyState(char key)
{
	InputState tempState = GetInstance()->keyState[key];

	// Check what the state is
	if (tempState == DOWN_FIRST)
	{
		GetInstance()->keyState[key] = DOWN;
	}
	else if (tempState == UP_FIRST)
	{
		GetInstance()->keyState[key] = UP;
	}

	return tempState;
}

int Input::GetSpecialKeyState(int key)
{
	InputState tempState = GetInstance()->specialKeys[key];

	// Check what the state is
	if (tempState == DOWN_FIRST)
	{
		GetInstance()->specialKeys[key] = DOWN;
	}
	else if (tempState == UP_FIRST)
	{
		GetInstance()->specialKeys[key] = UP;
	}

	return tempState;
}

glm::vec2 Input::GetMousePosition()
{
	return GetInstance()->mousePosition;
}

char Input::GetLastKeyDown()
{
	// Iterate through the keys
	for (int i = 0; i < 255; ++i)
	{
		// Return the key that is pressed
		if (GetInstance()->keyState[i] == DOWN)
		{
			return i;
		}
	}

	return ' ';
}

void Input::ScreenSpaceToWorldSpace(int x, int y)
{
	float halfScreenWidth = (float)SCR_WIDTH / 2; // TODO move to Utility
	float halfScreenHeight = (float)SCR_HEIGHT / 2;

	GetInstance()->mousePosition.x = (2.0f * (float)x) / (float)SCR_WIDTH -1.0f;
	GetInstance()->mousePosition.y = 1.0f - (2.0f * (float)y) / (float)SCR_HEIGHT;
	
}

void Input::ShutDown()
{
	delete instance;
	instance = NULL;
}



