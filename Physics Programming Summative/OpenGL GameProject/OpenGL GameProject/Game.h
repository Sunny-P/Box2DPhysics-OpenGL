#pragma once
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include "Dependencies/Box2D/Box2D.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/types.h>

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ModelMesh.h"
#include "Physics2DWorldContactListener.h"

class Camera;
class Player;
class TextLabel;
class CubeMesh;
class Mesh_Sphere;
class Texture;
class CubeMap;
class Quad;
class Model;
class Enemy;
class WorldObject;
class Audio;

class BasicShapes2DPhysics;

using namespace std;

struct textureDetails {
	Texture* texture;
	GLuint tex;
};

enum GameState {
	DEFAULT = 0
};

class Game
{
public:
	Game(int _width, int _height);
	~Game();

	void init();
	void render();
	void update();

	void RenderGameText();
	void RenderMainMenuText();
	void RenderGameScene();

	void UpdateGameText();

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

	

	void HoverMenuButton(int x, int y);
	void ClickMenuButton(int x, int y);

	void SetAmbientLight(float strength, glm::vec3 colour);

	textureDetails CreateTexture(std::string _filePath, GLint _textureWrapType = GL_CLAMP_TO_EDGE);
	Model* LoadModel(std::string _filePath, lightInfo _lighting);

	void CreateTextLabels();

	void PopulateCubemapFilePathVec();

	template<class T>
	void RenderObjectOutlined(T objectToBeOutlined, GLuint _program, float outlineWidth);

	template<class T>
	bool UpdateMousePicking(T objectToCheckClicked);

	void Restart();

	void EvaluatePhysicsWorldContact();

protected:
	GLuint program;
	GLuint cubemapProgram;
	GLuint colourProgram;
	GLuint phongProgram;
	GLuint reflectPhongProgram;
	GLuint outlineProgram;
	GLuint fogProgram;

	GLfloat currentTime;
	GLfloat deltaTime;
	GLfloat previousTimeStamp;

	ShaderLoader * shaderLoader;

	//Audio* audioSys;

	int width, height;

	Camera* camera;
	int mouseX, mouseY;

	lightInfo lighting;

	bool outlineObjects;
	bool wireframeOn;
	bool scissorTestOn;

	//float keyDelay = 0.0f;
	//float keyDelayWaitTime;

	// Mousepicking
	glm::vec3 rayDirection;

	// GAMESTATE
	GameState currentGameState;
	float shootDelay;
	float currentShootDelay;

	// SKYBOX
	CubeMap* skybox;
	std::vector<std::string> vecCubeMapFilePaths;

	// 2D BACKGROUND
	Quad* background2D;

	// PLAYER
	// Player class object used to posess specific chosen Model object
	
	// ENEMIES/NPC's
	

	// WORLD OBJECTS
	//CubeMesh* cube;
	BasicShapes2DPhysics* leftBoundWall;
	BasicShapes2DPhysics* ground;
	BasicShapes2DPhysics* holdingBlockBot;
	BasicShapes2DPhysics* holdingBlockRight;
	std::vector<BasicShapes2DPhysics*> birdsVec;
	std::vector<BasicShapes2DPhysics*> pigsVec;

	BasicShapes2DPhysics* glassBlock1;
	BasicShapes2DPhysics* glassBlock2;
	BasicShapes2DPhysics* glassBlock3;
	BasicShapes2DPhysics* glassBlock4;

	BasicShapes2DPhysics* crateBlock1;
	BasicShapes2DPhysics* crateBlock2;
	BasicShapes2DPhysics* crateBlock3;
	BasicShapes2DPhysics* crateBlock4;
	BasicShapes2DPhysics* crateBlock5;

	Quad* slingShot;
	bool newBirdSpawned = false;

	// Score / Lives
	TextLabel* ammoLeft;
	int startingAmmo;
	int currentAmmo;

	// MENU
	

	// PHYSICS
	b2Vec2 gravity;
	b2World* physicsWorld;
	Physics2DWorldContactListener physicsWorldContactListener;
	int velocityIterations;
	int positionIterations;

	b2RevoluteJointDef revJointDef;
	b2RevoluteJoint* revJoint;
	b2PulleyJoint* pulleyJoint;

	float physicsDeltaT;

	// Lighting
	float ambientStr;
	glm::vec3 ambientColour;
	glm::vec3 ambient;
	glm::vec3 lightColour;
	glm::vec3 lightPos;

	// Specular strength
	float specularStr;
	
	// Textures
	std::map<std::string, textureDetails> textureMap;

	/*  Model Data  */
	std::map<std::string, Model*> modelMap;
	
	enum InputState
	{
		UP,
		DOWN,
		UP_FIRST,
		DOWN_FIRST,
		INPUT_ESC = 27
	};
	InputState Keystate[255];

	enum InputMouse
	{
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT
	};
	InputState MouseState[3];

	InputState SpecialState[21];

	int randN, randL;
};

// Not to be used in 2D projects. 3D function
template<class T>
inline void Game::RenderObjectOutlined(T objectToBeOutlined, GLuint _program, float outlineWidth)
{
	glEnable(GL_STENCIL_TEST);	//enable stencil buffer 

	glStencilFunc(GL_ALWAYS, 1, 0xFF);	//specify condition for stencil pass
	glStencilMask(0xFF);	//enable writing to stencil buffer 

	objectToBeOutlined->render(_program);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);	//disable writing to stencil buffer 

	objectToBeOutlined->IncrementScale(outlineWidth, outlineWidth, outlineWidth);
	objectToBeOutlined->update();
	objectToBeOutlined->render(outlineProgram);

	objectToBeOutlined->IncrementScale(-outlineWidth, -outlineWidth, -outlineWidth);
	objectToBeOutlined->update();

	glStencilFunc(GL_ALWAYS, 1, 0xFF);	//specify condition for stencil pass
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_STENCIL_TEST);
}

// TODO: Add width var and get/set funcs to Quads
template<class T>
inline bool Game::UpdateMousePicking(T objectToCheckClicked)
{
	// screen pos
	glm::vec2 normalisedScreenPos = glm::vec2(Input::GetInstance()->mousePosition.x, Input::GetInstance()->mousePosition.y);

	// screen pos to Proj Space
	glm::vec4 clipCoords = glm::vec4(normalisedScreenPos.x, normalisedScreenPos.y, -1.0f, 1.0f);

	// Proj Space to eye space
	glm::mat4 invProjMat = glm::inverse(camera->GetProjectionMatrix());
	glm::vec4 eyeCoords = invProjMat * clipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

	// eye space to world space
	glm::mat4 invViewMat = glm::inverse(camera->GetViewMatrix());
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	rayDirection = glm::normalize(glm::vec3(rayWorld));

	// check intersection with other objects

	float radius = objectToCheckClicked->GetWidth() * 0.8;
	glm::vec3 v = objectToCheckClicked->GetPosition() - camera->GetCamPos();

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2 * glm::dot(v, rayDirection);
	float c = glm::dot(v, v) - radius * radius;
	float d = b * b - 4 * a * c;

	if (d > 0)
	{
		float x1 = (b - sqrt(d)) / 2;
		float x2 = (b + sqrt(d)) / 2;

		if (x1 >= 0 && x2 >= 0)
		{
			return true;	// intersects
		}
		if (x1 < 0 && x2 >= 0)
		{
			return true;	// intersects
		}
	}
	else if (d <= 0)
	{
		return false;		// No intersection
	}
}
