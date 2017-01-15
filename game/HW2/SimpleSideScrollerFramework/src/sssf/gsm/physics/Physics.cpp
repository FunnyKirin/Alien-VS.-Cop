/*
Author: Richard McKenna
Stony Brook University
Computer Science Department

Physics.cpp

See Physics.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gsm\physics\Collision.h"
#include "sssf\gsm\physics\CollidableObject.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\gsm\world\SparseLayer.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gsm\world\Tile.h"
#include "sssf\gsm\world\World.h"
#include "sssf\gsm\world\WorldLayer.h"
#include <vector>
#include <list>
#include <Box2D\Box2D.h>
#include "sssf\gsm\physics\AVCContactListener.h"






/*
The constructor initializes the data structures and loads
the necessary ones with recyclable objects for collision tests.
*/


Physics::Physics()
{
	/*
	// DEFAULT GRAVITY IS 1.0f
	gravity = DEFAULT_GRAVITY;

	// POPULATE THEM WITH 1000 OBJECTS TO SHARE
	// WHY 1000? BECAUSE I HAD TO PICK SOME NUMBER BIG ENOUGH
	// THAT WE'LL NEVER REACH, NOT THE MOST EFFICIENT TECHNIQUE,
	// BUT WE CAN CUSTOMIZE IT VIA A BUDGET IF WE KNEW WHAT
	// THE GAME'S NEEDS ARE
	for (int i = 0; i < 1000; i++)
	{
	// THESE ARE DUMMY OBJECTS USED FOR TILES, SINCE
	// TILES DON'T KNOW THEIR OWN LOCATIONS
	CollidableObject *co = new CollidableObject();
	co->setCurrentlyCollidable(true);
	co->getPhysicalProperties()->setVelocity(0.0f, 0.0f);
	recycledCollidableObjectsList.push_back(co);

	// THESE ARE ALL THE COLLISIONS WE'LL USE
	Collision *c = new Collision();
	recycledCollisions.push_back(c);
	}

	// NOW MAKE THE SWEEP AND PRUNE VECTORS. AGAIN, THESE GUYS KEEP ALL
	// SPRITE OBJECTS SORTED BY LEFT AND RIGHT SWEPT SHAPE EDGES SUCH THAT
	// WE MAY EASILY TEST FOR NEIGHBOR TO NEIGHBOR SPRITE COLLISIONS RATHER
	// THAN DO SOME CRAZY N! COMPUTATION
	sortedSweptShapes[LEFT_EDGE] = new vector<CollidableObject*>();
	sortedSweptShapes[RIGHT_EDGE] = new vector<CollidableObject*>();
	*/

	b2Vec2 gravity;

	gravity.Set(0.0f, 0.0f);

	m_world = new b2World(gravity);
	m_world->SetContinuousPhysics(true);
	m_world->SetAutoClearForces(true);

}


Physics::~Physics()
{

}


/*
Informs the physics system of additional dynamic, collidable object
to keep track of. Note that this method should not be used mid-collision,
as a result of a collition event, since it does not update all the
swept shape positions. Also note that it does not test to see if the
added object overlaps an object already in the game.
*/
/*
void Physics::addCollidableObject(CollidableObject *collidableObjectToAdd)
{

b2BodyDef myBodyDef;
myBodyDef.type = b2_dynamicBody;

b2Body* dynamicBody = m_world->CreateBody(&myBodyDef);
b2PolygonShape boxShape;
boxShape.SetAsBox(1, 1);

b2FixtureDef boxFixtureDef;
boxFixtureDef.shape = &boxShape;
boxFixtureDef.density = 1;
dynamicBody->CreateFixture(&boxFixtureDef);




}
*/

/*
Removes a dynamic, collidable object from the physics system. Note that
this method should not be used mid-collision, as a result of a collition
event, since it does not update all the swept shape positions. Also note
that it does not test to see if the added object overlaps an object already
in the game.
*/
//void Physics::removeCollidableObject(CollidableObject *collidableObjectToRemove)
//{
//map<CollidableObject*,set<Tile*>>::iterator tileSetToRemoveIt = spriteToTileCollisionsThisFrame.find(collidableObjectToRemove);
//spriteToTileCollisionsThisFrame.erase(tileSetToRemoveIt);
//removeCollidableObject(LEFT_EDGE,	collidableObjectToRemove);
//removeCollidableObject(RIGHT_EDGE,	collidableObjectToRemove);
//}



/*
This is where all game physics starts each frame. It is called each frame
by the game statem manager after player input and AI have been processed. It
updates the physical state of all dynamic objects in the game and
moves all objects to their end of frame positions, updates all necessary
object velocities, and calls all collision event handlers.
*/
void Physics::update(Game *game)
{
	m_world->Step(1.0f / 60.0f, 6, 2);

	


	unordered_set<b2Body*>::iterator it;
	it = bodies.begin();

	while (it != bodies.end())
	{
		b2Body *body = (*it);
		m_world->DestroyBody(body);
		it++;
	}

	bodies.clear();


	unordered_set<b2Body*>::iterator it2;
	it2 = deadBodies.begin();

	while (it2 != deadBodies.end())
	{
		b2Body *body2 = (*it2);
		m_world->DestroyBody(body2);
		it2++;
	}

	deadBodies.clear();


	







}






//void Physics::removeCollidableObject(unsigned int ordering, CollidableObject *co)
//{
//	vector<CollidableObject*> *sweptShapes = sortedSweptShapes[ordering];
//	unsigned int index = co->getSweepAndPruneIndex(ordering);
//	sweptShapes->erase(sweptShapes->begin() + index);
//}


