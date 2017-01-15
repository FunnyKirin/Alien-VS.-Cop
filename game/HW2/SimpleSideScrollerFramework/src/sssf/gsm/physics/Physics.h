/*
Author: Richard McKenna

Physics.h

This class will be used to manage all game collisions
and physics. This includes collision detection, and resolution.
Each frame we will update the collided objects velocities and
positions accordingly.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Collision.h"
#include "sssf\gsm\physics\CollidableObject.h"
#include "sssf\gsm\physics\CollisionListener.h"
#include "sssf\gsm\world\World.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gsm\world\Tile.h"
#include <Box2D/Box2D.h>
#include "sssf\gsm\physics\AVCContactListener.h"
#include <unordered_set>

const float EPSILON = 0.00001f;
const float DEFAULT_GRAVITY = 0.3f;
const float BUFFER_BETWEEN_OBJECTS = 0.2f;
const float NUDGE_VELOCITY = 0.3f;
const float ENERGY_LOSS = 0.95f;






class Physics
{
	//private:
public:


	b2World *m_world;




	// THIS IS APPLIED TO EVERY SPRITE EVERY FRAME
	float gravity;

	// THIS IS THE TIME IN THIS FRAME THAT HAS PASSED SO FAR,
	// NOT THAT ALL GAME OBJECTS WILL BE BROUGHT UP TO THIS MOMENT
	//float currentCollisionTime;

	// THIS IS THE CUSTOM EVENT HANDLER FOR PROVIDING GAME-SPECIFIC
	// EVENTS TO COLLISIONS BETWEEN GAME OBJECTS
	//CollisionListener *collisionListener;

	// WE USE LISTS BECAUSE WE ONLY REMOVE AND ADD FROM THE END FOR THESE 
	//list<Collision*> recycledCollisions;
	//list<CollidableObject*> recycledCollidableObjectsList;

	// WE USE A LIST BECAUSE WE'LL NEED TO PERIODICALLY REMOVE
	// FROM THE CENTER, AND WHILE WE DO SORT IT, WE DON'T SEARCH
	// IT FOR SINGLE ELEMENTS
	//list<Collision*> activeCollisions;

	// SWEEP AND PRUNE STUFF
	//map<unsigned int, vector<CollidableObject*>*> sortedSweptShapes;

	// THIS MAKES SURE WE AREN'T CHECKING A SLIGHLY PENETRATING COLLISION
	// PAIR OVER AND OVER AGAIN. THAT SCENARIO IS VERY HARD TO AVOID
	// IN A CONTINUOUS SYSTEM
	//map<CollidableObject*, set<Tile*>> spriteToTileCollisionsThisFrame;

	// USED FOR TESTING PHYSICS BY TURNING IT ON AND OFF IN VARIOUS WAYS
	bool activated;
	bool activatedForSingleUpdate;

	// CONSTRUCDT/DESTRUCTOR
	Physics();
	~Physics();

	// INLINED GET/SET METHODS
	//float				getGravity()					{ return gravity;					}
	//CollisionListener*	getCollisionListener()			{ return collisionListener;			}
	bool				isActivated()					{ return activated; }
	bool				isActivatedForSingleUpdate()	{ return activatedForSingleUpdate; }
	//void				activateForSingleUpdate()										{ activatedForSingleUpdate = true;				}
	//void				setGravity(float initGravity)									{ gravity = initGravity;						}
	//void				setCollisionListener(CollisionListener *initCollisionListener)	{ collisionListener = initCollisionListener;	}



	unordered_set<b2Body*> bodies;

	void setBodyList(unordered_set<b2Body*> bodyList)
	{
		bodies = bodyList;
	}

	unordered_set<b2Body*> getBodyList()
	{
		return bodies;
	}

	unordered_set<b2Body*> deadBodies;

	void setBodyList2(unordered_set<b2Body*> bodyList)
	{
		deadBodies = bodyList;
	}

	unordered_set<b2Body*> getBodyList2()
	{
		return deadBodies;
	}








	void setWorld(b2World *world)
	{
		m_world = world;
	}

	b2World* getWorld()
	{
		return m_world;
	}

	// PUBLIC METHODS DEFINED INSIDE Physics.cpp - YOU ARE WELCOME TO ADD MORE OR CHANGE WHAT YOU LIKE

	//void removeCollidableObject(CollidableObject *collidableObjectToRemove);

	void update(Game *game);


	// HELPER METHODS DEFINED INSIDE Physics.cpp - YOU ARE WELCOME TO ADD MORE AS YOU SEE FIT
private:



	//void removeCollidableObject(unsigned int ordering, CollidableObject *co);



};

/*
Provides a custom criteria for sorting for our swepts shapes when
we want the sorted by left edge.
*/


