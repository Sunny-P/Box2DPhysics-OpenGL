#pragma once
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include "Dependencies/Box2D/Box2D.h"

class Texture;

enum Shape {
	SQUARE,
	CIRCLE
};

class BasicShapes2DPhysics {
public:
	BasicShapes2DPhysics(GLuint _tex, b2World* physicsWorld, Shape shapeType, bool isStaticBody = false);
	~BasicShapes2DPhysics();

	void render(GLuint _program);
	void update();

	glm::mat4 GetModelMatrix();

	void IncrementPosition(float _incXBy, float _incYBy, float _incZBy);
	void IncrementRotation(float _degrees);

	// To be used with Rectangular shapes
	void IncrementScale(float _incXBy, float _incYBy, float _incZBy);

	void IncrementPosition(glm::vec3 _incrementVec);

	void SetPosition(float _XPos, float _YPos, float _ZPos);
	void SetRotation(glm::vec3 rotationVec);

	// To be used with Rectangular shapes
	void SetScale(float _XScale, float _YScale, float _ZScale);

	// To be used with Circle shapes
	void SetRadius(float radius);
	// To be used with Circle shapes
	void IncrementRadius(float radius);
	// To be used with Circle shapes
	void SetDiamater(float diameter);
	// To be used with Circle shapes
	void IncrementDiameter(float diameter);

	void SetDensity(float _density);
	void SetFriction(float _friction);
	void SetRestitution(float _restitution);

	GLuint GetTexture();

	b2Body* GetPhysicsBody() const;
	Shape GetShapeType() const;

	bool GetIsActive() const;
	void SetIsActive(bool _isActive);

private:
	// Updates position based on its physics body
	void UpdatePosition();
	void Update2DRotation();

protected:
	bool isActive;
	// PHYSICS
	b2BodyDef bodyDef;
	b2PolygonShape dynamicBox;
	b2CircleShape physicsCircle;
	b2FixtureDef fixtureDef;
	b2Fixture* fixture;
	b2Body* body;

	b2World* world;

	Shape physicsShapeType;

	float32 density;
	float32 friction;
	// Object bounciness
	float32 restitution;

	glm::vec3 position;
	glm::vec3 xRotation;
	glm::vec3 yRotation;
	glm::vec3 zRotation;
	glm::vec3 scale;

	float yRotationAngle;
	float xRotationAngle;
	float zRotationAngle;

	GLuint tex;

	GLuint VAO, VBO, EBO;

	GLfloat vertices[32]{
		// Position				// Colour/Normal		// Tex Coord
		-0.5f, 0.0f, -0.5f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		0.5f, 0.0f, 0.5f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		0.5f, 0.0f, -0.5f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f
	};

	GLuint indices[6]{
		// Winding order
		0, 1, 2,
		0, 2, 3
	};

	int indexCount, drawType;

	glm::mat4 modelMatrix;
	void UpdateModelMatrix();
};