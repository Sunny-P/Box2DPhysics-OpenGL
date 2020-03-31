#pragma once
#include <iostream>
#include <string.h>
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
		// Enemy is getting hit by another physics body
		if (contact->GetFixtureA()->GetBody()->GetUserData() == "enemy")
		{
			if (impulse->normalImpulses[0] >= 150.0f)
			{
				//contact->GetFixtureB()->GetBody()->GetWorld()->DestroyBody(contact->GetFixtureA()->GetBody());
				std::cout << "Pig Died" << std::endl;
			}
		}
	}
};

