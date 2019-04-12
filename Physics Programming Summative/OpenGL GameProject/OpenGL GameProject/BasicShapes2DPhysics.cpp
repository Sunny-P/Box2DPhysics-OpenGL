#pragma once
#include "BasicShapes2DPhysics.h"
#include "texture.h"

BasicShapes2DPhysics::BasicShapes2DPhysics(GLuint _tex, b2World* physicsWorld, Shape shapeType, bool isStaticBody)
{
	isActive = true;

	tex = _tex;

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	xRotation = glm::vec3(1.0f, 0.0f, 0.0f);
	yRotation = glm::vec3(0.0f, 1.0f, 0.0f);
	zRotation = glm::vec3(0.0f, 0.0f, 1.0f);
	scale = glm::vec3(0.5f, 0.5f, 0.5f);

	yRotationAngle = 0.0f;
	zRotationAngle = 0.0f;
	xRotationAngle = 90.0f;

	world = physicsWorld;
	if (!isStaticBody)
	{
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(position.x, position.y);
		body = world->CreateBody(&bodyDef);
	}
	else
	{
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(position.x, position.y);
		body = world->CreateBody(&bodyDef);
	}

	physicsShapeType = shapeType;

	dynamicBox.SetAsBox((float32)(scale.x * 0.5f), (float32)(scale.z * 0.5f));
	physicsCircle.m_p.Set((float32)0.0f, (float32)0.0f);
	physicsCircle.m_radius = (float32)(scale.x * 0.5f);
	if (physicsShapeType == SQUARE)
	{
		fixtureDef.shape = &dynamicBox;
	}
	else if (physicsShapeType == CIRCLE)
	{
		fixtureDef.shape = &physicsCircle;
	}

	density = 1.0f;
	friction = 0.3f;
	restitution = 0.0f;

	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	//fixture = body->CreateFixture(&fixtureDef);
	body->CreateFixture(&fixtureDef);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	indexCount = sizeof(indices) / sizeof(GLuint);
	drawType = GL_TRIANGLES;
}

BasicShapes2DPhysics::~BasicShapes2DPhysics()
{

}

void BasicShapes2DPhysics::render(GLuint _program)
{
	// Disable any openGL funcs unwanted/unnecessary for object
	//glDisable(GL_BLEND);
	if (isActive)
	{
		glUseProgram(_program);

		GLuint playerModelLoc = glGetUniformLocation(_program, "modelMat");
		glUniformMatrix4fv(playerModelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(glGetUniformLocation(_program, "tex"), 0);

		glBindVertexArray(VAO);
		glDrawElements(drawType, indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Reenable disabled func for others
		//glEnable(GL_BLEND);

		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void BasicShapes2DPhysics::update()
{
	if (isActive)
	{
		Update2DRotation();
		UpdatePosition();

		UpdateModelMatrix();
	}
}

glm::mat4 BasicShapes2DPhysics::GetModelMatrix()
{
	return modelMatrix;
}

void BasicShapes2DPhysics::IncrementPosition(float _incXBy, float _incYBy, float _incZBy)
{
	position.x += _incXBy;
	position.y += _incYBy;
	position.z += _incZBy;
}

void BasicShapes2DPhysics::IncrementRotation(float _degrees)
{
	yRotationAngle += _degrees;
}

void BasicShapes2DPhysics::IncrementScale(float _incXBy, float _incYBy, float _incZBy)
{
	scale.x += _incXBy;
	scale.z += _incYBy;
	scale.y += _incZBy;

	//body->DestroyFixture(fixture);
	dynamicBox.SetAsBox(scale.x * 0.5f, scale.z * 0.5f);
	fixtureDef.shape = &dynamicBox;
	//fixture = body->CreateFixture(&fixtureDef);
	body->CreateFixture(&fixtureDef);
}

void BasicShapes2DPhysics::IncrementPosition(glm::vec3 _incrementVec)
{
	position += _incrementVec;

	//world->DestroyBody(body);
	//bodyDef.position.Set((float32)position.x, (float32)position.y);
	//body = world->CreateBody(&bodyDef);
	b2Vec2 tempVec = body->GetTransform().p;
	body->SetTransform(tempVec + b2Vec2(_incrementVec.x, _incrementVec.y), body->GetAngle());

	//body->DestroyFixture(fixture);
	//if (physicsShapeType == SQUARE)
	//{
	//	fixtureDef.shape = &dynamicBox;
	//}
	//else if (physicsShapeType == CIRCLE)
	//{
	//	fixtureDef.shape = &physicsCircle;
	//}
	////fixture = body->CreateFixture(&fixtureDef);
	//body->CreateFixture(&fixtureDef);
}

void BasicShapes2DPhysics::SetPosition(float _XPos, float _YPos, float _ZPos)
{
	position.x = _XPos;
	position.y = _YPos;
	position.z = _ZPos;

	//world->DestroyBody(body);
	//bodyDef.position.Set((float32)position.x, (float32)position.y);
	//body = world->CreateBody(&bodyDef);
	body->SetTransform(b2Vec2(position.x, position.y), body->GetAngle());

	//body->DestroyFixture(fixture);
	/*if (physicsShapeType == SQUARE)
	{
		fixtureDef.shape = &dynamicBox;
	}
	else if (physicsShapeType == CIRCLE)
	{
		fixtureDef.shape = &physicsCircle;
	}*/
	//fixture = body->CreateFixture(&fixtureDef);
	//body->CreateFixture(&fixtureDef);
}

void BasicShapes2DPhysics::SetRotation(glm::vec3 rotationVec)
{
	xRotationAngle = rotationVec.x;
	yRotationAngle = rotationVec.y;
	zRotationAngle = rotationVec.z;
}

void BasicShapes2DPhysics::SetScale(float _XScale, float _YScale, float _ZScale)
{
	scale.x = _XScale;
	scale.z = _YScale;
	scale.y = _ZScale;

	//body->DestroyFixture(fixture);
	dynamicBox.SetAsBox((float32)(scale.x * 0.5f), (float32)(scale.z * 0.5f));
	fixtureDef.shape = &dynamicBox;
	//fixture = body->CreateFixture(&fixtureDef);
	body->CreateFixture(&fixtureDef);
}

void BasicShapes2DPhysics::SetRadius(float radius)
{
	scale.x = radius * 2.0f;
	scale.z = radius * 2.0f;

	//body->DestroyFixture(fixture);
	physicsCircle.m_radius = (float32)radius;
	fixtureDef.shape = &physicsCircle;
	//fixture = body->CreateFixture(&fixtureDef);
	body->CreateFixture(&fixtureDef);
}

void BasicShapes2DPhysics::IncrementRadius(float radius)
{
	scale.x += radius * 2.0f;
	scale.z += radius * 2.0f;

	//body->DestroyFixture(fixture);
	physicsCircle.m_radius += (float32)radius;
	fixtureDef.shape = &physicsCircle;
	//fixture = body->CreateFixture(&fixtureDef);
	body->CreateFixture(&fixtureDef);
}

void BasicShapes2DPhysics::SetDiamater(float diameter)
{
	scale.x = diameter;
	scale.z = diameter;

	//body->DestroyFixture(fixture);
	physicsCircle.m_radius = (float32)(diameter * 0.5f);
	fixtureDef.shape = &physicsCircle;
	//fixture = body->CreateFixture(&fixtureDef);
	body->CreateFixture(&fixtureDef);
}

void BasicShapes2DPhysics::IncrementDiameter(float diameter)
{
	scale.x += diameter;
	scale.z += diameter;

	//body->DestroyFixture(fixture);
	physicsCircle.m_radius += (float32)(diameter * 0.5f);
	fixtureDef.shape = &physicsCircle;
	//fixture = body->CreateFixture(&fixtureDef);
	body->CreateFixture(&fixtureDef);
}

void BasicShapes2DPhysics::SetDensity(float _density)
{
	//body->DestroyFixture(fixture);
	fixtureDef.shape = &physicsCircle;
	fixtureDef.density = _density;
	//fixture = body->CreateFixture(&fixtureDef);
	body->CreateFixture(&fixtureDef);
}

void BasicShapes2DPhysics::SetFriction(float _friction)
{
	//body->DestroyFixture(fixture);
	fixtureDef.shape = &physicsCircle;
	fixtureDef.friction = _friction;
	//fixture = body->CreateFixture(&fixtureDef);
	body->CreateFixture(&fixtureDef);
}

void BasicShapes2DPhysics::SetRestitution(float _restitution)
{
	//body->DestroyFixture(fixture);
	fixtureDef.shape = &physicsCircle;
	fixtureDef.restitution = _restitution;
	//fixture = body->CreateFixture(&fixtureDef);
	body->CreateFixture(&fixtureDef);
}

GLuint BasicShapes2DPhysics::GetTexture()
{
	return tex;
}

b2Body * BasicShapes2DPhysics::GetPhysicsBody() const
{
	return body;
}

Shape BasicShapes2DPhysics::GetShapeType() const
{
	return physicsShapeType;
}

bool BasicShapes2DPhysics::GetIsActive() const
{
	return isActive;
}

void BasicShapes2DPhysics::SetIsActive(bool _isActive)
{
	isActive = _isActive;
}

void BasicShapes2DPhysics::UpdatePosition()
{
	position.x = (float)body->GetPosition().x;
	position.y = (float)body->GetPosition().y;
}

void BasicShapes2DPhysics::Update2DRotation()
{
	zRotationAngle = glm::degrees(body->GetAngle());
}

void BasicShapes2DPhysics::UpdateModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(), position)
		* (glm::rotate(glm::mat4(), glm::radians(yRotationAngle), yRotation) * glm::rotate(glm::mat4(), glm::radians(zRotationAngle), zRotation) * glm::rotate(glm::mat4(), glm::radians(xRotationAngle), xRotation))
		* glm::scale(glm::mat4(), scale);
}
