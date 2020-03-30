#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include <fmod.hpp>

#include "Game.h"
#include "Input.h"
#include "Utility.h"

void init();
void render(void);
void update();

// keyboard input
void processInput();
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
// Mouse input
void mouseClick(int button, int state, int x, int y);
void mousePassiveMove(int x, int y);
void mouseMove(int x, int y);

void SpecialDown(int key, int x, int y);
void SpecialUp(int key, int x, int y);

void Shutdown();

Game* game;

//FMOD_RESULT result;
//FMOD::System* audioSystem;
//FMOD::Channel* channel;
//FMOD::Sound* Bone_Dance;
//FMOD::Sound* Deadlands;
//FMOD::Sound* Distant_Land;
//FMOD::Sound* Stagnant;
//FMOD::Sound* Morytania;
//
//std::vector<FMOD::Sound*> backgroundMusicVec;
//
//bool audioInit();
//void audioPlaySound(FMOD::Sound * _track, FMOD::Channel* _channel, float volume);
//
//int playIndex;

int main(int argc, char **argv)
{
	srand((unsigned)time(0));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(50, 20);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutCreateWindow(" ");

	glClearColor(1.0, 0.0, 1.0, 1.0);	// clear colour

	glewInit();
	game = new Game(SCR_WIDTH, SCR_HEIGHT);
	init();

	// register callbacks
	glutDisplayFunc(render);
	glutIdleFunc(update);

	// register function pointers for callbacks
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mousePassiveMove);
	glutMotionFunc(mouseMove);
	glutSpecialFunc(SpecialDown);
	glutSpecialUpFunc(SpecialUp);

	glutCloseFunc(Shutdown);
	glutMainLoop();	// ensure this is always the last glut line called
	return(0);
}

void init()
{
	// Enabling all openGL testing funcs necessary

	//----Culling----
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	// 3D - Depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);	//enable stencil buffer 
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	//stPass, dpFail, bothPass 

	// Alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutSetOption(GLUT_MULTISAMPLE, 8); 
	glEnable(GL_MULTISAMPLE);

	game->init();
}

void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(1.0, 0.0, 1.0, 1.0);	// Debug clear colour
	//glClearColor(0.6, 0.6, 0.6, 1.0);	// Fog clear colour

	game->render();

	glutSwapBuffers();
}

void update()
{
	// may need a little restructuring regarding key presses
	processInput();

	game->update();

	glutPostRedisplay();	// render function is called
}

void processInput()
{
	game->processInput();
}

void keyboardDown(unsigned char key, int x, int y)
{
	//game->keyboardDown(key, x, y);
	Input::KeyboardDown(key, x, y);
}

void keyboardUp(unsigned char key, int x, int y)
{
	//game->keyboardUp(key, x, y);
	Input::KeyboardUp(key, x, y);
}

void mouseClick(int button, int state, int x, int y)
{
	//game->mouseClick(button, state, x, y);
	Input::MouseClick(button, state, x, y);
}

void mousePassiveMove(int x, int y)
{
	game->mousePassiveMove(x, y);
	//Input::MousePassiveMove(x, y);
}

void mouseMove(int x, int y)
{
	game->mouseMove(x, y);
	Input::MouseActiveMove(x, y);
}

void SpecialDown(int key, int x, int y)
{
	//game->SpecialDown(key, x, y);
	Input::SpecialKeyboardDown(key, x, y);
}

void SpecialUp(int key, int x, int y)
{
	//game->SpecialUp(key, x, y);
	Input::SpecialKeyboardUp(key, x, y);
}

void Shutdown()
{
	//game->Shutdown();
	std::cout << "Game Shutdown" << std::endl;
	delete game;
	game = 0;
}

//bool audioInit()
//{
//	//FMOD_RESULT result;
//	result = FMOD::System_Create(&audioSystem);
//	if (result != FMOD_OK)
//	{
//		return false;
//	}
//
//	result = audioSystem->init(100, FMOD_INIT_NORMAL, 0);
//	if (result != FMOD_OK)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//void audioPlaySound(FMOD::Sound * _track, FMOD::Channel * _channel, float volume)
//{
//	result = audioSystem->playSound(_track, 0, false, &_channel);
//	result = channel->setVolume(volume);
//}
