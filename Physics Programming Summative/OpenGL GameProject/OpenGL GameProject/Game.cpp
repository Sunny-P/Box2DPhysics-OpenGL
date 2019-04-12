#pragma once
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include "Game.h"
#include "Camera.h"
#include "Player.h"
#include "TextLabel.h"
#include "CubeMesh.h"
#include "Mesh_Sphere.h"
#include "texture.h"
#include "CubeMap.h"
#include "Quad.h"
#include "Model.h"
#include "Enemy.h"
#include "Audio.h"
#include "WorldObject.h"
#include "Input.h"
#include "BasicShapes2DPhysics.h"

Game::Game(int _width, int _height)
{
	/*playerTextureFilePath = "Resources/Textures/Player_Ship.png";
	backgroundFilePath = "Resources/Textures/water.png";*/
	currentGameState = DEFAULT;

	//audioSys = new Audio();

	width = _width;
	height = _height;

	// Default values for light
	ambientStr = 0.3f;
	ambientColour = glm::vec3(0.75f, 0.75f, 0.75f);
	ambient = ambientStr * ambientColour;

	//lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	lightPos = glm::vec3(-500.0f, 500.0f, 500.0f);

	specularStr = 1.0f;

	// Structure with light information to pass into models as uniforms for model shader
	// so lighting can work on models
	lighting.ambient = ambient;
	lighting.ambientColour = ambientColour;
	lighting.ambientStr = ambientStr;
	lighting.lightColour = lightColour;
	lighting.lightPos = lightPos;
	lighting.specularStr = specularStr;

	PopulateCubemapFilePathVec();

	textureDetails tempDetails;
	tempDetails = CreateTexture("Resources/Textures/Objects/crate3.jpg");
	//cube = new CubeMesh(tempDetails.tex);
	/*cube->SetScale(1.0f, 7.5f, 10.0f);
	cube->IncrementPositionVec(glm::vec3(50.0f, 3.25f, 0.0f));*/

	camera = new Camera(ORTHO);

	// Change lightColour so enemies can be shaded differently
	//lighting.lightColour = glm::vec3(0.0f, 0.502f, 0.0f);

	skybox = new CubeMap(camera, vecCubeMapFilePaths);

	outlineObjects = false;
	wireframeOn = false;
	scissorTestOn = false;

	tempDetails = CreateTexture("Resources/Textures/mountainsSky.jpg");
	background2D = new Quad(tempDetails.tex);
	background2D->SetScale(2000.0f, 100.0f, 1000.0f);
	background2D->IncrementPositionVec(glm::vec3(CENTRE_X, CENTRE_Y - 50.0f, -5000.0f));
	
	startingAmmo = 6;
	currentAmmo = startingAmmo;
	//keyDelayWaitTime = 0.1f;
}

Game::~Game()
{
	//delete audioSys;

	for (auto it = textureMap.begin(); it != textureMap.end(); ++it)
	{
		delete it->second.texture;
	}
	textureMap.clear();

	for (auto it = modelMap.begin(); it != modelMap.end(); ++it)
	{
		delete it->second;
	}
	modelMap.clear();

	delete camera;
	delete background2D;

	delete physicsWorld;

	//delete cube;
	// PHYSICS STATICS
	delete holdingBlockBot;
	delete holdingBlockRight;
	delete leftBoundWall;
	delete ground;

	// PHYSICS DYNAMICS
	for (int it = 0; it != birdsVec.size(); ++it)
	{
		delete birdsVec.at(it);
	}
	birdsVec.clear();

	for (int it = 0; it != firedBirds.size(); ++it)
	{
		delete firedBirds.at(it);
	}
	firedBirds.clear();

	for (int it = 0; it != pigsVec.size(); ++it)
	{
		delete pigsVec.at(it);
	}
	pigsVec.clear();

	delete glassBlock1;
	delete glassBlock2;
	delete glassBlock3;
	delete glassBlock4;

	delete crateBlock1;
	delete crateBlock2;
	delete crateBlock3;
	delete crateBlock4;

	delete slingShot;

	delete ammoLeft;
}

void Game::init()
{
	previousTimeStamp = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	ShaderLoader shaderLoader;
	program = shaderLoader.CreateProgram("Resources/Shaders/VertexShader.vs", "Resources/Shaders/FragmentShader.fs");
	reflectPhongProgram = shaderLoader.CreateProgram("Resources/Shaders/BlinnPhong+Reflect-VertexShader.vs", "Resources/Shaders/BlinnPhong+Reflect-FragmentShader.fs");
	phongProgram = shaderLoader.CreateProgram("Resources/Shaders/BlinnPhong-VertexShader.vs", "Resources/Shaders/BlinnPhong-FragmentShader.fs");
	cubemapProgram = shaderLoader.CreateProgram("Resources/Shaders/CubeMap/CubeMapVS.vs", "Resources/Shaders/CubeMap/CubeMapFS.fs");
	outlineProgram = shaderLoader.CreateProgram("Resources/Shaders/VertexShader.vs", "Resources/Shaders/OutlineShader.fs");
	fogProgram = shaderLoader.CreateProgram("Resources/Shaders/FogVertexShader.vs", "Resources/Shaders/FogFragmentShader.fs");

	//cube->init();

	CreateTextLabels();

	//-- Default values for Physics ---------------------------
	gravity = b2Vec2(0.0f, -9.8f);
	physicsWorld = new b2World(gravity);

	velocityIterations = 8;
	positionIterations = 3;
	//---------------------------------------------------------

	textureDetails tempDetails;
	tempDetails = CreateTexture("Resources/Textures/Objects/crate3.jpg");

	ground = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE, true);
	ground->SetScale((float)SCR_WIDTH, 20.0f, 1.0f);
	ground->SetPosition((float)CENTRE_X, 3.0f, 0.0f);

	leftBoundWall = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE, true);
	leftBoundWall->SetScale(20.0f, 650.0f, 1.0f);
	leftBoundWall->SetPosition(0.0f, (float)CENTRE_Y, 0.0f);

	holdingBlockBot = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE, true);
	holdingBlockBot->SetScale((float)SCR_WIDTH, 5.0f, 1.0f);
	holdingBlockBot->SetPosition((float)CENTRE_X, (float)SCR_HEIGHT - 45.0f, 0.0f);

	holdingBlockRight = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE, true);
	holdingBlockRight->SetScale(10.0f, 50.0f, 1.0f);
	holdingBlockRight->SetPosition((float)SCR_WIDTH - 5.0f, (float)SCR_HEIGHT - 25.0f, 0.0f);

	for (int i = 0; i < 6; ++i)
	{
		if (i < 3)
		{
			switch (i)
			{
			case 0:
				tempDetails = CreateTexture("Resources/Textures/redAngryBird.png");
				break;
			case 1:
				tempDetails = CreateTexture("Resources/Textures/blueAngryBird.png");
				break;
			case 2:
				tempDetails = CreateTexture("Resources/Textures/blackAngryBird.png");
				break;
			default:
				tempDetails = CreateTexture("Resources/Textures/redAngryBird.png");
				break;
			}
			birdsVec.push_back(new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, CIRCLE));
		}
		else
		{
			int random = rand() % 6 + 1;
			if (random > 4)
			{
				tempDetails = CreateTexture("Resources/Textures/redAngryBird.png");
			}
			else if (random > 2)
			{
				tempDetails = CreateTexture("Resources/Textures/blueAngryBird.png");
			}
			else
			{
				tempDetails = CreateTexture("Resources/Textures/blackAngryBird.png");
			}
			birdsVec.push_back(new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, CIRCLE));
		}
		float rF = 10.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30.0f - 10.0f)));
		birdsVec.back()->SetRadius(rF);
		birdsVec.back()->SetPosition(50.0f + (rF* 2.0f) * i, (float)SCR_HEIGHT, 0.0f);
		birdsVec.back()->SetRestitution(0.25f);
	}

	tempDetails = CreateTexture("Resources/Textures/angryBirdsSlingshot.png");
	slingShot = new Quad(tempDetails.tex);
	slingShot->SetScale(70.0f, 1.0f, 160.0f);
	slingShot->SetPosition(250.0f, 80.0f + 13.0f, 0.0f);

	tempDetails = CreateTexture("Resources/Textures/angryBirdsPig.png");
	for (int i = 0; i < 8; i++)
	{
		if (i == 0)
		{
			pigsVec.push_back(new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE));
			pigsVec.back()->SetRadius(18.0f);
			pigsVec.back()->SetPosition((float)CENTRE_X - 70.0f + (35.0f * 2.0f) * i, 70.0f, 0.0f);
			pigsVec.back()->SetFriction(1.0f);
		}
		else
		{

			pigsVec.push_back(new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, CIRCLE));
			pigsVec.back()->SetRadius(25.0f);
			pigsVec.back()->SetPosition((float)CENTRE_X + (35.0f * 2.0f) * i, 70.0f, 0.0f);
		}
	}

	tempDetails = CreateTexture("Resources/Textures/glassTexture.png");
	glassBlock1 = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE);
	glassBlock1->SetScale(320.0f, 25.0f, 1.0f);
	glassBlock1->SetPosition((float)CENTRE_X - 110.0f, 88.0f, 0.0f);
	glassBlock1->SetFriction(1.0f);

	glassBlock2 = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE);
	glassBlock2->SetScale(15.0f, 150.0f, 1.0f);
	glassBlock2->SetPosition((float)CENTRE_X + 250.0f, 88.0f, 0.0f);

	glassBlock3 = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE);
	glassBlock3->SetScale(15.0f, 150.0f, 1.0f);
	glassBlock3->SetPosition((float)CENTRE_X + 380.0f, 88.0f, 0.0f);

	glassBlock4 = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE);
	glassBlock4->SetScale(25.0f, 25.0f, 1.0f);
	glassBlock4->SetPosition((float)CENTRE_X - 200.0f, 100.0f, 0.0f);

	tempDetails = CreateTexture("Resources/Textures/Objects/crate3.jpg");
	crateBlock1 = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE);
	crateBlock1->SetScale(225.0f, 20.0f, 1.0f);
	crateBlock1->SetPosition((float)CENTRE_X + 278.0f, 173.0f, 0.0f);

	crateBlock2 = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE, true);
	crateBlock2->SetScale(25.0f, 30.0f, 1.0f);
	crateBlock2->SetPosition((float)CENTRE_X, (float)CENTRE_Y, 0.0f);

	crateBlock3 = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE);
	crateBlock3->SetScale(100.0f, 30.0f, 1.0f);
	crateBlock3->SetPosition((float)CENTRE_X - 45.0f, (float)CENTRE_Y, 0.0f);

	crateBlock4 = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE);
	crateBlock4->SetScale(50.0f, 50.0f, 1.0f);
	crateBlock4->SetPosition((float)CENTRE_X + 220.0f, 190.0f, 0.0f);

	crateBlock5 = new BasicShapes2DPhysics(tempDetails.tex, physicsWorld, SQUARE);
	crateBlock5->SetScale(70.0f, 80.0f, 1.0f);
	crateBlock5->SetPosition((float)CENTRE_X - 8.0f, CENTRE_Y - 30.0f, 0.0f);

	// Revolute Joint
	revJointDef.Initialize(crateBlock2->GetPhysicsBody(), crateBlock3->GetPhysicsBody(), crateBlock2->GetPhysicsBody()->GetWorldCenter());
	revJointDef.lowerAngle = 0.0f * b2_pi;
	revJointDef.upperAngle = glm::radians(360.0f);
	revJointDef.enableLimit = true;
	revJointDef.maxMotorTorque = 20.0f;
	revJointDef.motorSpeed = 30.0f;
	revJointDef.enableMotor = true;

	//Pulley Joint
	b2Vec2 anchor1 = glassBlock4->GetPhysicsBody()->GetWorldCenter();
	b2Vec2 anchor2 = crateBlock4->GetPhysicsBody()->GetWorldCenter();
	b2Vec2 groundAnchor1(glassBlock4->GetPhysicsBody()->GetTransform().p.x, 4.0f);
	b2Vec2 groundAnchor2((float)CENTRE_X + 200.0f, 5.0f);
	float32 ratio = 1.0f;
	b2PulleyJointDef pulleyJointDef;
	pulleyJointDef.Initialize(glassBlock4->GetPhysicsBody(), crateBlock4->GetPhysicsBody(), groundAnchor1, groundAnchor2, anchor1, anchor2, ratio);
	pulleyJoint = (b2PulleyJoint*)physicsWorld->CreateJoint(&pulleyJointDef);

	revJoint = (b2RevoluteJoint*)physicsWorld->CreateJoint(&revJointDef);
	//physicsCircle->SetDensity(1.0f);
}

void Game::render()
{
	// Render Skybox no matter what
	//skybox->Render(cubemapProgram);
	//background2D->render(program);

	switch (currentGameState)
	{
	case DEFAULT:
		slingShot->render(program);

		ground->render(program);
		leftBoundWall->render(program);
		holdingBlockBot->render(program);
		holdingBlockRight->render(program);

		glassBlock1->render(program);
		glassBlock2->render(program);
		glassBlock3->render(program);
		glassBlock4->render(program);

		crateBlock1->render(program);
		crateBlock2->render(program);
		crateBlock3->render(program);
		crateBlock4->render(program);
		crateBlock5->render(program);

		RenderGameScene();
		break;

	default:
		break;
	}

	RenderGameText();
}

void Game::update()
{
	//audioSys->Update();
	// Calc deltaTime
	currentTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentTime - previousTimeStamp) * 0.001f;
	previousTimeStamp = currentTime;

	physicsDeltaT = deltaTime * 2.0f;

	glUseProgram(phongProgram);
	GLuint currentTimeL = glGetUniformLocation(phongProgram, "deltaTime");
	glUniform1f(currentTimeL, deltaTime);

	glUseProgram(program);
	glUniform1f(glGetUniformLocation(program, "deltaTime"), deltaTime);

	glUseProgram(phongProgram);
	// Light uniforms
	glUniform3fv(glGetUniformLocation(phongProgram, "ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(phongProgram, "lightColour"), 1, glm::value_ptr(lightColour));
	glUniform3fv(glGetUniformLocation(phongProgram, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform1f(glGetUniformLocation(phongProgram, "lightSpecStr"), specularStr);

	// Always update camera and skybox
	//skybox->Update();

	glm::vec3 camPos = camera->GetCamPos();

	camera->update(deltaTime);
	GLuint camPVMatLoc = glGetUniformLocation(phongProgram, "PVMatrix");
	glUniformMatrix4fv(camPVMatLoc, 1, GL_FALSE, glm::value_ptr(camera->GetPVMatrix()));
	glUniform3fv(glGetUniformLocation(phongProgram, "camPos"), 1, glm::value_ptr(camera->GetCamPos()));

	glUseProgram(outlineProgram);
	camPVMatLoc = glGetUniformLocation(outlineProgram, "PVMatrix");
	glUniformMatrix4fv(camPVMatLoc, 1, GL_FALSE, glm::value_ptr(camera->GetPVMatrix()));

	glUseProgram(fogProgram);
	camPVMatLoc = glGetUniformLocation(fogProgram, "PVMatrix");
	glUniformMatrix4fv(camPVMatLoc, 1, GL_FALSE, glm::value_ptr(camera->GetPVMatrix()));
	glUniform3fv(glGetUniformLocation(fogProgram, "camPos"), 1, glm::value_ptr(camera->GetCamPos()));

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "PVMatrix"), 1, GL_FALSE, glm::value_ptr(camera->GetPVMatrix()));

	glUseProgram(reflectPhongProgram);
	// Light uniforms
	glUniform3fv(glGetUniformLocation(reflectPhongProgram, "ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(reflectPhongProgram, "lightColour"), 1, glm::value_ptr(lightColour));
	glUniform3fv(glGetUniformLocation(reflectPhongProgram, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform1f(glGetUniformLocation(reflectPhongProgram, "lightSpecStr"), specularStr);

	glUniformMatrix4fv(glGetUniformLocation(reflectPhongProgram, "PVMatrix"), 1, GL_FALSE, glm::value_ptr(camera->GetPVMatrix()));
	glUniform3fv(glGetUniformLocation(reflectPhongProgram, "camPos"), 1, glm::value_ptr(camera->GetCamPos()));

	background2D->update();

	switch (currentGameState)
	{
	case DEFAULT:
		physicsWorld->Step((float32)physicsDeltaT, (int32)velocityIterations, (int32)positionIterations);

		for (unsigned int i = 0; i < birdsVec.size(); i++)
		{
			birdsVec.at(i)->update();
		}

		for (unsigned int i = 0; i < firedBirds.size(); i++)
		{
			firedBirds.at(i)->update();
		}

		for (unsigned int i = 0; i < pigsVec.size(); i++)
		{
			pigsVec.at(i)->update();
		}

		slingShot->update();

		leftBoundWall->update();
		ground->update();
		holdingBlockBot->update();
		holdingBlockRight->update();

		glassBlock1->update();
		glassBlock2->update();
		glassBlock3->update();
		glassBlock4->update();

		crateBlock1->update();
		crateBlock2->update();
		crateBlock3->update();
		crateBlock4->update();
		crateBlock5->update();
		break;

	default:
		break;
	}

	UpdateGameText();

	/*if (keyDelay > 0.0f)
	{
		keyDelay -= deltaTime;
	}
	else if (keyDelay < 0.0f)
	{
		keyDelay = 0.0f;
	}*/
}

void Game::RenderGameText()
{
	ammoLeft->render();
}

void Game::RenderMainMenuText()
{
	
}

void Game::RenderGameScene()
{
	if (scissorTestOn)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor((GLint)(CENTRE_X-87.5f), (GLint)(CENTRE_Y-87.5f), 175, 175);
	}
	else if (!scissorTestOn)
	{
		glDisable(GL_SCISSOR_TEST);
	}

	if (wireframeOn)
	{
		glPolygonMode(GL_FRONT, GL_LINE);
	}
	else if (!wireframeOn)
	{
		
		glPolygonMode(GL_FRONT, GL_FILL);
	}

	if (outlineObjects)
	{
		
	}
	else if (!outlineObjects)
	{
		
	}

	for (unsigned int i = 0; i < birdsVec.size(); i++)
	{
		birdsVec.at(i)->render(program);
	}

	for (unsigned int i = 0; i < firedBirds.size(); i++)
	{
		firedBirds.at(i)->render(program);
	}

	for (unsigned int i = 0; i < pigsVec.size(); i++)
	{
		pigsVec.at(i)->render(program);
	}
}

void Game::UpdateGameText()
{
	ammoLeft->SetText("Birds Left: " + std::to_string(currentAmmo));
}

void Game::processInput()
{
	switch (currentGameState)
	{
	case DEFAULT:
		camera->processInput();

		// Quit program using ESC
		if (Input::GetKeyState(INPUT_ESC) == DOWN_FIRST)
		{
			Shutdown();
		}

		// F key to turn wireframing on/off
		if ((Input::GetKeyState('f') == DOWN_FIRST) || (Input::GetKeyState('F') == DOWN_FIRST))
		{
			if (wireframeOn)
			{
				wireframeOn = false;
				//keyDelay = keyDelayWaitTime;
			}
			else if (!wireframeOn)
			{
				wireframeOn = true;
				//keyDelay = keyDelayWaitTime;
			}
		}

		// O key to turn outlining on/off
		if ((Input::GetKeyState('o') == DOWN_FIRST) || (Input::GetKeyState('O') == DOWN_FIRST))
		{
			if (outlineObjects)
			{
				outlineObjects = false;
				//keyDelay = keyDelayWaitTime;
			}
			else if (!outlineObjects)
			{
				outlineObjects = true;
				//keyDelay = keyDelayWaitTime;
			}
		}

		// T key to turn scissor testing on/off
		if ((Input::GetKeyState('t') == DOWN_FIRST) || (Input::GetKeyState('T') == DOWN_FIRST))
		{
			if (scissorTestOn)
			{
				scissorTestOn = false;
				//keyDelay = keyDelayWaitTime;
			}
			else if (!scissorTestOn)
			{
				scissorTestOn = true;
				//keyDelay = keyDelayWaitTime;
			}
		}

		// Camera movement
		/*if ((Input::GetKeyState('w') == DOWN) || (Input::GetKeyState('W') == DOWN))
		{
			camera->SetCamPos(camera->GetCamPos() + glm::vec3(0.0f, 0.0f, 0.25f));
		}
		if ((Input::GetKeyState('s') == DOWN) || (Input::GetKeyState('S') == DOWN))
		{
			camera->SetCamPos(camera->GetCamPos() + glm::vec3(0.0f, 0.0f, -0.25f));
		}*/

		// R key to restart scene
		if ((Input::GetKeyState('r') == DOWN_FIRST) || (Input::GetKeyState('R') == DOWN_FIRST))
		{
			Restart();
			//keyDelay = keyDelayWaitTime;
		}

		// Check mouse click
		//if (Input::GetMouseState(MOUSE_LEFT) == DOWN_FIRST)
		//{
		//	// Mouse picking code example from previous working code
		//	// If "forwardButton" is clicked
		//	/*if (UpdateMousePicking(forwardButton))
		//	{
		//		// This moved the camera forwards some
		//		camera->SetCamPos(camera->GetCamPos() + glm::vec3(0.0f, 0.0f, 0.25f));
		//		// I did not use deltaTime here. This line should have been:
		//		camera->SetCamPos(camera->GetCamPos() + glm::vec3(0.0f, 0.0f, 0.25f * deltaTime));
		//		// Albeit, it would have moved slower, but the value is only that low because I did not use deltaTime
		//	}*/
		//	if (!birdsVec.empty())
		//	{
		//		firedBirds.push_back(birdsVec.back());
		//		birdsVec.pop_back();
		//	}

		//}
		if (Input::GetMouseState(MOUSE_LEFT) == DOWN)
		{
			//if ((float)mouseX < 250.0f)
			//{
				//std::cout << mouseX << std::endl;
				//std::cout << mouseY << std::endl;
				if (currentAmmo > 0)
				{
					std::cout << "setting position of bird at back of birds vector" << std::endl;
					birdsVec.back()->SetPosition((float)mouseX, (float)SCR_HEIGHT - (float)mouseY, 0.0f);
				}
			//}
		}
		if (Input::GetMouseState(MOUSE_LEFT) == UP_FIRST)
		{
			if ((float)mouseX < 250.0f)
			{
				if (currentAmmo > 0)
				{
					currentAmmo--;
					std::cout << "back bird pushed into fired vector and popped out birds vector" << std::endl;
					firedBirds.push_back(birdsVec.back());
					birdsVec.pop_back();
					if (!firedBirds.empty())
					{
						std::cout << "force applied to bird at back of fired birds vector" << std::endl;
						firedBirds.back()->GetPhysicsBody()->ApplyForce(b2Vec2(mouseX - slingShot->GetXPosition(), ((float)SCR_HEIGHT - mouseY) - (slingShot->GetYPosition() + (slingShot->GetYScale()*0.5f))), firedBirds.back()->GetPhysicsBody()->GetWorldCenter(), true);
					}
				}
			}
		}
		
		break;
	default:
		break;
	}
}

void Game::keyboardDown(unsigned char key, int x, int y)
{
	Keystate[key] = DOWN;
	camera->keyboardDown(key, x, y);
}

void Game::keyboardUp(unsigned char key, int x, int y)
{
	Keystate[key] = UP;
	camera->keyboardUp(key, x, y);
}

void Game::mouseClick(int button, int state, int x, int y)
{
	if (button >= 3)
	{
		return;
	}
	//std::cout << "Clicked x: " << x << " | y: " << y << std::endl;
	MouseState[button] = (state == GLUT_DOWN) ? DOWN : UP;
	if (state == GLUT_DOWN && button == MOUSE_LEFT)
	{
		switch (currentGameState)
		{
		case DEFAULT:

			break;

		default:
			break;
		}
	}
}

void Game::mousePassiveMove(int _x, int _y)
{
	//std::cout << "Passive x: " << _x << " | y: " << _y << std::endl;
	//HoverMenuButton(_x, _y);
	//Input::MousePassiveMove(_x, _y);
	mouseX = _x;
	mouseY = _y;
}

void Game::mouseMove(int x, int y)
{
	//std::cout << "Clicked x: " << x << " | y: " << y << std::endl;
	mouseX = x;
	mouseY = y;
}

void Game::SpecialDown(int key, int x, int y)
{
	SpecialState[key] = DOWN;
}

void Game::SpecialUp(int key, int x, int y)
{
	SpecialState[key] = UP;
}

void Game::Shutdown()
{
	glutLeaveMainLoop();
}

void Game::HoverMenuButton(int _x, int _y)
{
	switch (currentGameState)
	{
	case DEFAULT:

		break;

	default:
		break;
	}
}

void Game::ClickMenuButton(int x, int y)
{
	//audioSys->playMenuClick();
	switch (currentGameState)
	{
	case 0:	// GAME_SCENE
	{
		break;
	}
	default:
		break;
	}
}

void Game::SetAmbientLight(float strength, glm::vec3 colour)
{
	ambientStr = strength;
	ambientColour = colour;

	ambient = ambientStr * ambientColour;
}

textureDetails Game::CreateTexture(std::string _filePath, GLint _textureWrapType)
{
	// Check if texture already exists
	for (auto it = textureMap.begin(); it != textureMap.end(); ++it)
	{
		// If texture already exists, return this texture
		if (it->first == _filePath)
		{
			return it->second;
		}
	}
	// else texture doesn't exist
	textureDetails details;
	Texture* newTex;
	newTex = new Texture(_filePath, _textureWrapType);
	newTex->init();
	details.texture = newTex;
	details.tex = newTex->GetTexture();
	textureMap.insert(std::pair<std::string, textureDetails>(_filePath, details));

	return details;
}

Model * Game::LoadModel(std::string _filePath, lightInfo _lighting)
{
	// Check if model already exists
	for (auto it = modelMap.begin(); it != modelMap.end(); ++it)
	{
		// Model has already been loaded before, return a pointer to model
		if (it->first == _filePath)
		{
			return it->second;
		}
	}
	// else model has not been loaded before
	Model* model;
	model = new Model(_filePath, camera, _lighting);
	modelMap.insert(std::pair<std::string, Model*>(_filePath, model));

	return model;
}

void Game::CreateTextLabels()
{
	// SCORE LABEL EXAMPLE
	// Score
	//scoreText = new TextLabel(score + std::to_string(scoreNum), "Resources/Fonts/R-2014.ttf", glm::vec2(-380.0f, 265.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	//scoreText->SetScale(0.5f);
	ammoLeft = new TextLabel("Birds Left: " + std::to_string(currentAmmo), "Resources/Fonts/arial.ttf", glm::vec2(-CENTRE_X + 185.0f, (float)CENTRE_Y - 45.0f));
	ammoLeft->SetScale(0.3f);
}

void Game::PopulateCubemapFilePathVec()
{
	// Only need to use filename. CubeMap already uses directory prefix
	std::string filePath;

	// Right
	// Starscape
	filePath = "right.jpg";
	vecCubeMapFilePaths.push_back(filePath);

	// Left
	filePath = "left.jpg";
	vecCubeMapFilePaths.push_back(filePath);

	// Top
	filePath = "top.jpg";
	vecCubeMapFilePaths.push_back(filePath);

	// Bottom
	filePath = "bottom.jpg";
	vecCubeMapFilePaths.push_back(filePath);

	// Back
	filePath = "back.jpg";
	vecCubeMapFilePaths.push_back(filePath);

	// Front
	filePath = "front.jpg";
	vecCubeMapFilePaths.push_back(filePath);
}

void Game::Restart()
{
	camera->SetCamPos(glm::vec3(0.0f, 1.5f, -5.0f));

	outlineObjects = false;
	wireframeOn = false;
	scissorTestOn = false;
}
