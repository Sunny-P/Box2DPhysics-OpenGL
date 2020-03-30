#pragma once
#include <iostream>
#include "Dependencies/Box2D/Dynamics/b2WorldCallbacks.h"
#include "BasicShapes2DPhysics.h"

class Physics2DWorldContactListener :
	public b2ContactListener
{
	void BeginContact(b2Contact* contact)
	{
		
	}

	void EndContact(b2Contact* contact)
	{

	}

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{

	}

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		// TODO: GET UserData properly. User data not being got proper
		BasicShapes2DPhysics* shapeA = nullptr;
		shapeA = (BasicShapes2DPhysics*)contact->GetFixtureA()->GetBody()->GetUserData();
		BasicShapes2DPhysics* shapeB = nullptr;
		shapeB = (BasicShapes2DPhysics*)contact->GetFixtureB()->GetBody()->GetUserData();

		if (shapeA != nullptr)
		{
			if (shapeA->GetObjectType() == ENEMY)
			{
				std::cout << "PostSolve callback::Enemy UserData got" << std::endl;
			}
		}
	}
};

