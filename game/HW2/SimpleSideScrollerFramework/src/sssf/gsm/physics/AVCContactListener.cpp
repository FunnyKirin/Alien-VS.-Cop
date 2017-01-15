#include "stdio.h"
using namespace std;
//#include "LuaPlus.h"
//using namespace LuaPlus;
#include <Box2D\Box2D.h>
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\physics\AVCContactListener.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\physics\UserData.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\ai\bots\RandomFloatingBot.h"
#include <unordered_set>



void AVCContactListener::BeginContact(b2Contact* contact) {

	/*LuaState* luaPState = LuaState::Create();
	int result = luaPState->DoFile("LuaScript.lua");
	// Get a global variable
	LuaObject healthObj = luaPState->GetGlobal("PlayerHealth");
	int health = healthObj.GetInteger();*/

	b2World *world = game->getGSM()->getPhysics()->getWorld();


	unordered_set<b2Body*> bodiesToRemove = game->getGSM()->getPhysics()->getBodyList();


	UserData *udB = (UserData*)contact->GetFixtureB()->GetUserData();
	UserData *udA = (UserData*)contact->GetFixtureA()->GetUserData();

	AnimatedSprite *player1 = nullptr;
	AnimatedSprite *player2 = nullptr;
	Bot *bot1 = nullptr;
	Bot *bot2 = nullptr;

	AnimatedSpriteType *bot1Sprite = nullptr;
	AnimatedSpriteType *bot2Sprite = nullptr;

	AnimatedSpriteType *player1Sprite = nullptr;
	AnimatedSpriteType *player2Sprite = nullptr;

	AnimatedSpriteType *cop = nullptr;
	AnimatedSpriteType *meleeAlien = nullptr;
	AnimatedSpriteType *rangeAlien = nullptr;
	AnimatedSpriteType *copBullet = nullptr;
	AnimatedSpriteType *alienBullet = nullptr;
	AnimatedSpriteType *HLaser = nullptr;
	AnimatedSpriteType *VLaser = nullptr;
	AnimatedSpriteType *turret = nullptr;
	AnimatedSpriteType *boss = nullptr;
	AnimatedSpriteType *laserRifle = nullptr;

	cop = game->getGSM()->getSpriteManager()->getSpriteType(0);
	meleeAlien = game->getGSM()->getSpriteManager()->getSpriteType(3);
	rangeAlien = game->getGSM()->getSpriteManager()->getSpriteType(4);
	copBullet = game->getGSM()->getSpriteManager()->getSpriteType(2);
	alienBullet = game->getGSM()->getSpriteManager()->getSpriteType(5);
	HLaser = game->getGSM()->getSpriteManager()->getSpriteType(7);
	VLaser = game->getGSM()->getSpriteManager()->getSpriteType(8);
	turret = game->getGSM()->getSpriteManager()->getSpriteType(10);
	boss = game->getGSM()->getSpriteManager()->getSpriteType(9);
	laserRifle = game->getGSM()->getSpriteManager()->getSpriteType(6);

	if (udB == nullptr && udA == nullptr)
	{
		return;
	}

	if (udA == nullptr || udB == nullptr)
	{
		if (udB == nullptr)
		{
			if (!udA->isbot)
			{
				return;
			}

			if (udA->getBot()->getSpriteType() == game->getGSM()->getSpriteManager()->getSpriteType(2)
				|| udA->getBot()->getSpriteType() == game->getGSM()->getSpriteManager()->getSpriteType(5)
				|| udA->getBot()->getSpriteType() == game->getGSM()->getSpriteManager()->getSpriteType(6)
				|| udA->getBot()->getSpriteType() == game->getGSM()->getSpriteManager()->getSpriteType(7)
				|| udA->getBot()->getSpriteType() == game->getGSM()->getSpriteManager()->getSpriteType(8))
			{
				bodiesToRemove.insert(udA->getBot()->getBody());
			}

		}

		else if (udA == nullptr)
		{
			if (!udB->isbot)
			{
				return;
			}

			if (udB->getBot()->getSpriteType() == game->getGSM()->getSpriteManager()->getSpriteType(2)
				|| udB->getBot()->getSpriteType() == game->getGSM()->getSpriteManager()->getSpriteType(5)
				|| udB->getBot()->getSpriteType() == game->getGSM()->getSpriteManager()->getSpriteType(6)
				|| udB->getBot()->getSpriteType() == game->getGSM()->getSpriteManager()->getSpriteType(7)
				|| udB->getBot()->getSpriteType() == game->getGSM()->getSpriteManager()->getSpriteType(8))
			{
				bodiesToRemove.insert(udB->getBot()->getBody());
			}

		}
		game->getGSM()->getPhysics()->setBodyList(bodiesToRemove);
		return;
	}



	if (udA->isbot && udB->isbot)
	{
		bot1 = udA->getBot();
		bot2 = udB->getBot();


		bot1Sprite = bot1->getSpriteType();
		bot2Sprite = bot2->getSpriteType();




		if (bot1Sprite == copBullet || bot2Sprite == copBullet)
		{
			if (bot1Sprite == copBullet)
			{
				bodiesToRemove.insert(bot1->getBody());
				if (bot2Sprite == meleeAlien || bot2Sprite == rangeAlien || bot2Sprite == boss || bot2Sprite == turret)
				{
					bot2->setHealth(bot2->getHealth() - 1);

					if (bot2->getHealth() == 0)
					{
						bot2->setCurrentState(L"DEATH");

					}
				}
			}
			else if (bot2Sprite == copBullet)
			{
				bodiesToRemove.insert(bot2->getBody());
				if (bot1Sprite == meleeAlien || bot1Sprite == rangeAlien || bot1Sprite == boss || bot1Sprite == turret)
				{
					bot1->setHealth(bot1->getHealth() - 1);

					if (bot1->getHealth() == 0)
					{
						bot1->setCurrentState(L"DEATH");

					}
				}
			}
			game->getGSM()->getPhysics()->setBodyList(bodiesToRemove);
			return;
		}

		if (bot1Sprite == HLaser || bot2Sprite == HLaser)
		{
			if (bot1Sprite == HLaser)
			{
				bodiesToRemove.insert(bot1->getBody());
				if (bot2Sprite == meleeAlien || bot2Sprite == rangeAlien || bot2Sprite == boss || bot2Sprite == turret)
				{
					bot2->setHealth(bot2->getHealth() - 2);

					if (bot2->getHealth() <= 0)
					{
						bot2->setCurrentState(L"DEATH");

					}
				}
			}
			else if (bot2Sprite == HLaser)
			{
				bodiesToRemove.insert(bot2->getBody());
				if (bot1Sprite == meleeAlien || bot1Sprite == rangeAlien || bot1Sprite == boss || bot1Sprite == turret)
				{
					bot1->setHealth(bot1->getHealth() - 2);

					if (bot1->getHealth() <= 0)
					{
						bot1->setCurrentState(L"DEATH");

					}
				}
			}
			game->getGSM()->getPhysics()->setBodyList(bodiesToRemove);
			return;
		}


		if (bot1Sprite == VLaser || bot2Sprite == VLaser)
		{
			if (bot1Sprite == VLaser)
			{
				bodiesToRemove.insert(bot1->getBody());
				if (bot2Sprite == meleeAlien || bot2Sprite == rangeAlien || bot2Sprite == boss || bot2Sprite == turret)
				{
					bot2->setHealth(bot2->getHealth() - 2);

					if (bot2->getHealth() <= 0)
					{
						bot2->setCurrentState(L"DEATH");

					}
				}
			}
			else if (bot2Sprite == VLaser)
			{
				bodiesToRemove.insert(bot2->getBody());
				if (bot1Sprite == meleeAlien || bot1Sprite == rangeAlien || bot1Sprite == boss || bot1Sprite == turret)
				{
					bot1->setHealth(bot1->getHealth() - 2);

					if (bot1->getHealth() <= 0)
					{
						bot1->setCurrentState(L"DEATH");

					}
				}
			}
			game->getGSM()->getPhysics()->setBodyList(bodiesToRemove);
			return;
		}




	}

	if ((udA->isbot || udB->isbot) && (!udA->isbot || !udB->isbot))
	{
		if (udA->isbot)
		{
			bot1 = udA->getBot();
			player2 = udB->getSprite();
			bot1Sprite = bot1->getSpriteType();
			player2Sprite = player2->getSpriteType();
		}
		else if (udB->isbot)
		{
			bot2 = udB->getBot();
			player1 = udA->getSprite();
			bot2Sprite = bot2->getSpriteType();
			player1Sprite = player1->getSpriteType();
		}


		if (bot1Sprite == alienBullet || bot2Sprite == alienBullet)
		{
			if (bot1Sprite == alienBullet)
			{
				bodiesToRemove.insert(bot1->getBody());


				if (player2Sprite == cop && player2->getHurtable() == 0)
				{
					player2->setHealth(player2->getHealth() - 1);
					player2->setHurtable(1);

					if (player2->getHealth() <= 0)
					{
						player2->setCurrentState(L"DEATH");
						
						bodiesToRemove.insert(player2->getBody());
					}
				}
			}
			else if (bot2Sprite == alienBullet)
			{
				bodiesToRemove.insert(bot2->getBody());
				if (player1Sprite == cop&&player1->getHurtable() == 0)
				{
					player1->setHealth(player1->getHealth() - 1);
					player1->setHurtable(1);
					if (player1->getHealth() <= 0)
					{
						player1->setCurrentState(L"DEATH");
						bodiesToRemove.insert(player1->getBody());
					}
				}
			}
			game->getGSM()->getPhysics()->setBodyList(bodiesToRemove);
			return;
		}

		if (bot1Sprite == meleeAlien || bot2Sprite == meleeAlien)
		{
			if (bot1Sprite == meleeAlien&&bot1->getHealth()>0)
			{

				if (player2Sprite == cop && player2->getHurtable() == 0)
				{
					player2->setHealth(player2->getHealth() - 1);
					player2->setHurtable(1);

				}
			}
			else if (bot2Sprite == meleeAlien&&bot2->getHealth()>0)
			{

				if (player1Sprite == cop && player1->getHurtable() == 0)
				{
					player1->setHealth(player1->getHealth() - 1);
					player1->setHurtable(1);

				}
			}
			game->getGSM()->getPhysics()->setBodyList(bodiesToRemove);
			return;
		}

		if (bot1Sprite == laserRifle || bot2Sprite == laserRifle)
		{
			if (bot1Sprite == laserRifle)
			{
				if (player2Sprite == cop)
				{
					bodiesToRemove.insert(bot1->getBody());
					player2->setWeapon(player2->getWeapon()+1);
				}
			}
			else if (bot2Sprite == laserRifle)
			{

				if (player1Sprite == cop)
				{

					bodiesToRemove.insert(bot2->getBody());
					player1->setWeapon(player1->getWeapon() + 1);

				}
			}
			game->getGSM()->getPhysics()->setBodyList(bodiesToRemove);
			return;
		}



	}












}









/*
void EndContact(b2Contact* contact) {
//check if fixture A was the foot sensor
void* fixtureUserData = contact->GetFixtureA()->GetUserData();
if ((int)fixtureUserData == 3)
fixturesUnderfoot.erase(contact->GetFixtureB());//A is foot so B is ground
//check if fixture B was the foot sensor
fixtureUserData = contact->GetFixtureB()->GetUserData();
if ((int)fixtureUserData == 3)
fixturesUnderfoot.erase(contact->GetFixtureA());//B is foot so A is ground
}
*/