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
		//if (contact->GetFixtureA()->GetBody()->GetUserData() == "enemy")
		//{
		//	if (impulse->normalImpulses[0] >= 150.0f)
		//	{
		//		//contact->GetFixtureB()->GetBody()->GetWorld()->DestroyBody(contact->GetFixtureA()->GetBody());
		//		std::cout << "Pig Died" << std::endl;
		//	}
		//}
		BasicShapes2DPhysics* userDataA = (BasicShapes2DPhysics*)contact->GetFixtureA()->GetBody()->GetUserData();
		BasicShapes2DPhysics* userDataB = (BasicShapes2DPhysics*)contact->GetFixtureB()->GetBody()->GetUserData();

		// 0 == BIRD
		// 1 == ENEMY
		// 2 == DESTRUCTIBLE_OBJECT
		if (userDataA->GetObjectType() == 1)
		{
			if (impulse->normalImpulses[0] >= 250000.0f)
			{
				std::cout << "Pig Died" << std::endl;
				userDataA->SetLifetime(0.0f);
			}
		}
		else if (userDataA->GetObjectType() == 0)
		{
			if (userDataB->GetObjectType() == 1)
			{
				std::cout << "Pig Died" << std::endl;
				userDataB->SetLifetime(0.0f);
			}
		}
		else if (userDataA->GetObjectType() == 2)
		{
			if (userDataB->GetObjectType() == 0)
			{
				if (impulse->normalImpulses[0] >= 45000.0f)
				{
					userDataA->SetLifetime(0.0f);
				}
			}
		}
		
		if (userDataB->GetObjectType() == 1)
		{
			if (impulse->normalImpulses[0] >= 250000.0f)
			{
				std::cout << "Pig Died" << std::endl;
				userDataB->SetLifetime(0.0f);
			}
		}
		else if (userDataB->GetObjectType() == 0)
		{
			if (userDataA->GetObjectType() == 1)
			{
				std::cout << "Pig Died" << std::endl;
				userDataA->SetLifetime(0.0f);
			}
		}
		else if (userDataB->GetObjectType() == 2)
		{
			if (userDataA->GetObjectType() == 0)
			{
				if (impulse->normalImpulses[0] >= 45000.0f)
				{
					
					userDataB->SetLifetime(0.0f);
				}
			}
		}

		//std::cout << "normalImpulse: " << impulse->normalImpulses[0] << std::endl;
	}
};

