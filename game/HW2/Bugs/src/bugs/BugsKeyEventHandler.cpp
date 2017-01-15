/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	BugsKeyEventHandler.cpp

	See BugsKeyEventHandler.h for a class description.
*/
#include <SFML/Audio.hpp>
#include "bugs_VS\stdafx.h"
#include "LuaPlus.h"
using namespace LuaPlus;
#include "stdio.h"
using namespace std;
#include "bugs_VS\stdafx.h"
#include "bugs\BugsGame.h"
#include "bugs\BugsKeyEventHandler.h"
#include "sssf\game\Game.h"
#include "sssf\game\WStringTable.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsTimer.h"
#include "sssf\gsm\ai\bots\RandomFloatingBot.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\sprite\QuadTree.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsTimer.h"
#include <Box2D/Box2D.h>
#include "sssf\gsm\physics\UserData.h"


/*
	handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
	gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
	key states since the last frame, which can allow us to respond to key presses, including when keys
	are held down for multiple frames.
*/
void BugsKeyEventHandler::handleKeyEvents(Game *game)
{
	sf::Music music;


	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
	AnimatedSprite *player = gsm->getSpriteManager()->getPlayer();
	PhysicalProperties *pp = player->getPhysicalProperties();
	Viewport *viewport = game->getGUI()->getViewport();
	b2Body *body = player->getBody();
	list<Bot*> b = gsm->getSpriteManager()->getBots();
	list<Bot*>::iterator it;
	it = b.begin();
	bool leftPressed = false;
	bool rightPressed = false;
	bool upPressed = false;
	bool downPressed = false;
	

	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		leftPressed = false;
		rightPressed = false;
		upPressed = false;
		downPressed = false;

		LuaState* luaPState = LuaState::Create();
		int result = luaPState->DoFile("LuaScript.lua");
		int time = game->getTimer()->getTotalTime();
		time = time / 100;
		LuaObject timeObj = luaPState->GetGlobal("Time");
		timeObj.AssignInteger(luaPState, time);
		int timer1 = timeObj.GetInteger();

		if (player->getHurtable() == 1){
			player->setHurtTimer(time+10);
			player->setHurtable(2);
		}
		if (player->gethurtTimer() == time){
			player->setHurtable(0);
		}
		if (player->getHurtable()==2&&time%2==0){
			player->setAlpha(100);
		}else{
			player->setAlpha(255);
		}

		while (it != b.end())
		{
			Bot *bot2 = (*it);
			int botID=bot2->getSpriteType()->getSpriteTypeID();
			bot2->getPhysicalProperties()->setPosition(bot2->getBody()->GetPosition().x * 64, bot2->getBody()->GetPosition().y * 64);
			it++;
			int dX = ((player->getPhysicalProperties()->getX()) - (bot2->getPhysicalProperties()->getX()));
			int dY = ((player->getPhysicalProperties()->getY()) - (bot2->getPhysicalProperties()->getY()));

			if (botID == 13){
				bot2->getPhysicalProperties()->setPosition(viewport->getViewportX() + 400, viewport->getViewportY() + 5);
				bot2->getBody()->SetLinearVelocity(player->getBody()->GetLinearVelocity());
				if (player->getLives() >= 3){
					bot2->setCurrentState(L"l3");
				}
				if (player->getLives() == 2){
					bot2->setCurrentState(L"l2");
				}
				if (player->getLives() == 1){
					bot2->setCurrentState(L"l1");
				}
			}

			if (botID == 12){
				bot2->getPhysicalProperties()->setPosition(viewport->getViewportX()+500, viewport->getViewportY()+5);
				bot2->getBody()->SetLinearVelocity(player->getBody()->GetLinearVelocity());
				if (player->getHealth() >= 11){
					bot2->setCurrentState(L"h10");
				}
				if (player->getHealth() == 10){
					bot2->setCurrentState(L"h9");
				}
				if (player->getHealth() == 9){
					bot2->setCurrentState(L"h8");
				}
				if (player->getHealth() == 8){
					bot2->setCurrentState(L"h7");
				}
				if (player->getHealth() == 7){
					bot2->setCurrentState(L"h6");
				}
				if (player->getHealth() == 6){
					bot2->setCurrentState(L"h5");
				}
				if (player->getHealth() == 5){
					bot2->setCurrentState(L"h4");
				}
				if (player->getHealth() == 4){
					bot2->setCurrentState(L"h3");
				}
				if (player->getHealth() == 3){
					bot2->setCurrentState(L"h2");
				}
				if (player->getHealth() == 2){
					bot2->setCurrentState(L"h1");
				}
			}

			if (botID == 3){

				if (bot2->getHealth() > 0){
					if (((abs(dX) < 400) && (abs(dY) < 400)) || bot2->getHealth()<3){
						bot2->getBody()->SetLinearVelocity(b2Vec2(((player->getPhysicalProperties()->getX()) - (bot2->getPhysicalProperties()->getX())) / 50, ((player->getPhysicalProperties()->getY()) - (bot2->getPhysicalProperties()->getY())) / 50));
					}

					if ((dX / 50) > 0 && abs(dX) > abs(dY)){
						bot2->setCurrentState(MOVE_RIGHT);
					}
					else if ((dX / 50) < 0 && abs(dX) > abs(dY)){
						bot2->setCurrentState(MOVE_LEFT);
					}
					else if ((dY / 50) > 0 && abs(dX) < abs(dY)){
						bot2->setCurrentState(MOVE_DOWN);
					}
					else if ((dY / 50) < 0 && abs(dX) < abs(dY)){
						bot2->setCurrentState(MOVE_UP);
					}
					else{
						bot2->setCurrentState(IDLE);
					}
					/**
					if (abs(dX) < 80 && abs(dY < 80)){
					player->setHealth(player->getHealth() - 1);
					}
					*/
				}
			}



			if (botID == 4){
				if (bot2->getHealth() > 0){
					if (((abs(dX) < 500) && (abs(dY) < 500)) || bot2->getHealth()<3){
						bot2->getBody()->SetLinearVelocity(b2Vec2(-dX / 120, -dY / 120));
						if ((-dX / 120) > 0 && abs(dX) > abs(dY)){
							bot2->setCurrentState(MOVE_LEFT);
							bot2->setFacingDirection(2);
						}
						else if ((-dX / 120) < 0 && abs(dX) > abs(dY)){
							bot2->setCurrentState(MOVE_RIGHT);
							bot2->setFacingDirection(3);
						}
						else if ((-dY / 120) > 0 && abs(dX) < abs(dY)){
							bot2->setCurrentState(MOVE_UP);
							bot2->setFacingDirection(1);
						}
						else if ((-dY / 120) < 0 && abs(dX) < abs(dY)){
							bot2->setCurrentState(MOVE_DOWN);
							bot2->setFacingDirection(0);
						}
						else{
							bot2->setCurrentState(IDLE);
							bot2->setFacingDirection(0);
						}
					}

					if (time % 25 == 0 && (abs(dX) < 500) && (abs(dY) < 500)){
						GameStateManager *gsm = game->getGSM();
						Physics *physics = game->getGSM()->getPhysics();
						SpriteManager *spriteManager = gsm->getSpriteManager();
						AnimatedSprite *player = spriteManager->getPlayer();

						AnimatedSpriteType *bullet = gsm->getSpriteManager()->getSpriteType(5);

						RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

						PhysicalProperties *pp = bot->getPhysicalProperties();
						//int x = 11 * 64;
						//int y = 2 * 64;

						bot->setSpriteType(bullet);
						bot->setCurrentState(BULLET);
						bot->setAlpha(255);
						//pp->setPosition(x, y);
						spriteManager->addBot(bot, game->getGSM()->getWorld());
						bot->affixTightAABBBoundingVolume();


						b2BodyDef bulletDef;
						bulletDef.type = b2_dynamicBody;

						bulletDef.position.Set(bot2->getBody()->GetPosition().x, bot2->getBody()->GetPosition().y);

						b2Body *bullet2 = physics->getWorld()->CreateBody(&bulletDef);

						b2PolygonShape bulletShape;
						bulletShape.SetAsBox(.1, .1);

						b2FixtureDef bulletFixtureDef;
						bulletFixtureDef.shape = &bulletShape;
						bulletFixtureDef.density = 1;


						bulletFixtureDef.filter.categoryBits = CATEGORY_ALIENBULLET;
						bulletFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_PLAYERBULLET | CATEGORY_HLASER | CATEGORY_ALIENBULLET | CATEGORY_VLASER | CATEGORY_MALIEN | CATEGORY_RALIEN | CATEGORY_LASERRIFLE);


						b2Fixture *alienBullet = bullet2->CreateFixture(&bulletFixtureDef);

						UserData *bub = new UserData();
						bub->setBot(bot);
						bub->isbot = true;

						alienBullet->SetUserData(bub);

						bot->setBody(bullet2);
						if (bot->getFacingDirection() == 0)
							pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64 + 32);
						if (bot->getFacingDirection() == 1)
							pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64 - 32);
						if (bot->getFacingDirection() == 2)
							pp->setPosition(bot->getBody()->GetPosition().x * 64 - 32, bot->getBody()->GetPosition().y * 64);
						if (bot->getFacingDirection() == 3)
							pp->setPosition(bot->getBody()->GetPosition().x * 64 + 32, bot->getBody()->GetPosition().y * 64);
						bot->getBody()->SetLinearVelocity(b2Vec2(((player->getPhysicalProperties()->getX()) - (bot2->getPhysicalProperties()->getX())) / 50, ((player->getPhysicalProperties()->getY()) - (bot2->getPhysicalProperties()->getY())) / 50));
					}

				}
			}
			if (botID == 11 && input->isKeyDownForFirstTime(SPACE_KEY)){
				bot2->setCurrentState(L"FINISH");
			}
			if (botID == 9 && bot2->getHealth() <= 0){
				bot2->getPhysicalProperties()->setPosition(16 * 64, 64);
				bot2->setCurrentState(DEATH);

			}
			if (botID == 10 && bot2->getHealth() <= 0){
				bot2->setCurrentState(DEATH);
			}
			if (botID == 10 && abs(dY) < 256&&bot2->getHealth()>0){
				if (time % 10 == 0){
					GameStateManager *gsm = game->getGSM();
					Physics *physics = game->getGSM()->getPhysics();
					SpriteManager *spriteManager = gsm->getSpriteManager();
					AnimatedSprite *player = spriteManager->getPlayer();

					AnimatedSpriteType *bullet = gsm->getSpriteManager()->getSpriteType(5);

					RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

					PhysicalProperties *pp = bot->getPhysicalProperties();
					//int x = 11 * 64;
					//int y = 2 * 64;

					bot->setSpriteType(bullet);
					bot->setCurrentState(BULLET);
					bot->setAlpha(255);
					//pp->setPosition(x, y);
					spriteManager->addBot(bot, game->getGSM()->getWorld());
					bot->affixTightAABBBoundingVolume();


					b2BodyDef bulletDef;
					bulletDef.type = b2_dynamicBody;

					bulletDef.position.Set(bot2->getBody()->GetPosition().x, bot2->getBody()->GetPosition().y);

					b2Body *bullet2 = physics->getWorld()->CreateBody(&bulletDef);

					b2PolygonShape bulletShape;
					bulletShape.SetAsBox(.1, .1);

					b2FixtureDef bulletFixtureDef;
					bulletFixtureDef.shape = &bulletShape;
					bulletFixtureDef.density = 1;


					bulletFixtureDef.filter.categoryBits = CATEGORY_ALIENBULLET;
					bulletFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_PLAYERBULLET | CATEGORY_HLASER | CATEGORY_ALIENBULLET | CATEGORY_VLASER | CATEGORY_MALIEN | CATEGORY_RALIEN | CATEGORY_LASERRIFLE);


					b2Fixture *alienBullet = bullet2->CreateFixture(&bulletFixtureDef);

					UserData *bub = new UserData();
					bub->setBot(bot);
					bub->isbot = true;

					alienBullet->SetUserData(bub);

					bot->setBody(bullet2);
						pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64);
					bot->getBody()->SetLinearVelocity(b2Vec2(((player->getPhysicalProperties()->getX()) - (bot2->getPhysicalProperties()->getX())) / 35, ((player->getPhysicalProperties()->getY()) - (bot2->getPhysicalProperties()->getY())) / 35));
				}
			}

			if (botID == 9&& dY<704&&bot2->getHealth()>0){
				bot2->getPhysicalProperties()->setPosition(16 * 64, 64);
				if (time % 30 == 0){
					Shoot(game, 23.2, 4.3, 0, 12);
					Shoot(game, 24.6, 4.3, 0, 12);

					Shoot(game, 23.1, 3.6, -6, 10);
					Shoot(game, 24.7, 3.6, 6, 10);

					Shoot(game, 23.2, 3, -15, 8);
					Shoot(game, 24.6, 3, 15, 8);
				}
				if (time % 45 == 0){
					Shoot(game, 23.2, 4.3, ((player->getPhysicalProperties()->getX()) - 24*64) / 40, ((player->getPhysicalProperties()->getY()) - 2*64) / 40);
					Shoot(game, 24.6, 4.3, ((player->getPhysicalProperties()->getX()) - 24 * 64) / 40, ((player->getPhysicalProperties()->getY()) - 2 * 64) / 40);

					Shoot(game, 23.1, 3.6, ((player->getPhysicalProperties()->getX()) - 24 * 64) / 40, ((player->getPhysicalProperties()->getY()) - 2 * 64) / 40);
					Shoot(game, 24.7, 3.6, ((player->getPhysicalProperties()->getX()) - 24 * 64) / 40, ((player->getPhysicalProperties()->getY()) - 2 * 64) / 40);

					Shoot(game, 23.2, 3, ((player->getPhysicalProperties()->getX()) - 24 * 64) / 40, ((player->getPhysicalProperties()->getY()) - 2 * 64) / 40);
					Shoot(game, 24.6, 3, ((player->getPhysicalProperties()->getX()) - 24 * 64) / 40, ((player->getPhysicalProperties()->getY()) - 2 * 64) / 40);
				}
				if (dY<256&&time%4==0){
					Shoot(game, 23.2, 4.3, -20, -3);
					Shoot(game, 24.6, 4.3, 20, -3);

					Shoot(game, 23.1, 3.6, -20, -3);
					Shoot(game, 24.7, 3.6, 20, -3);

					Shoot(game, 23.2, 3, -20, -3);
					Shoot(game, 24.6, 3, 20, -3);
				}
			}

		}
		
		player->getPhysicalProperties()->setPosition(player->getBody()->GetPosition().x * 64, player->getBody()->GetPosition().y * 64);
	
		//if (input->isKeyDownForFirstTime(T_KEY))
		//{
		//	gsm->getPhysics()->activateForSingleUpdate();
		//}
		//if (input->isKeyDownForFirstTime(D_KEY))
		//{
		//	viewport->toggleDebugView();
		//	game->getGraphics()->toggleDebugTextShouldBeRendered();
		//}
	     if (input->isKeyDownForFirstTime(L_KEY))
		{
			game->getGraphics()->togglePathfindingGridShouldBeRendered();
		}
		if (input->isKeyDownForFirstTime(F_KEY))
		{
			game->getGraphics()->togglePathfindingPathShouldBeRendered();
		}
		if (player->getHealth() > 0)
		{
			double vX = 0;
			double vY = 0;

			if (input->isKeyDown(A_KEY)){
				vX = -10;
				leftPressed = true;
			} if (input->isKeyDown(D_KEY)){
				vX = 10;
				rightPressed = true;
			}if (input->isKeyDown(W_KEY)){
				vY = -10;
				upPressed = true;
			} if (input->isKeyDown(S_KEY)){
				vY = 10;
				downPressed = true;
			}

			body->SetLinearVelocity(b2Vec2(vX, vY));
			if (vX > 0){
				player->setCurrentState(MOVE_RIGHT);
				player->setFacingDirection(3);
			}
			else if (vX < 0){
				player->setCurrentState(MOVE_LEFT);
				player->setFacingDirection(2);
			}
			else if (vY > 0){
				player->setCurrentState(MOVE_DOWN);
				player->setFacingDirection(0);
			}
			else if (vY < 0){
				player->setCurrentState(MOVE_UP);
				player->setFacingDirection(1);
			}
			else{
				if (player->getFacingDirection() == 1)
					player->setCurrentState(L"IDLE_BACK");
				else if (player->getFacingDirection() == 2)
					player->setCurrentState(L"IDLE_LEFT");
				else if (player->getFacingDirection() == 3)
					player->setCurrentState(L"IDLE_RIGHT");
				else player->setCurrentState(IDLE);
			}
		}
		if (input->isKeyDownForFirstTime(SPACE_KEY))
		{
			


			if (player->getWeapon() == 0){
				GameStateManager *gsm = game->getGSM();
				Physics *physics = game->getGSM()->getPhysics();
				SpriteManager *spriteManager = gsm->getSpriteManager();
				AnimatedSprite *player = spriteManager->getPlayer();

				AnimatedSpriteType *bullet = gsm->getSpriteManager()->getSpriteType(2);

				RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

				PhysicalProperties *pp = bot->getPhysicalProperties();
				//int x = 11 * 64;
				//int y = 2 * 64;

				bot->setSpriteType(bullet);
				bot->setCurrentState(BULLET);
				bot->setAlpha(255);
				//pp->setPosition(x, y);
				spriteManager->addBot(bot, game->getGSM()->getWorld());
				bot->affixTightAABBBoundingVolume();


				b2BodyDef bulletDef;
				bulletDef.type = b2_dynamicBody;

				if (player->getFacingDirection()==3)
				{
					player->setCurrentState(L"SHOOT_RIGHT");
					bulletDef.position.Set(player->getBody()->GetPosition().x + 1, player->getBody()->GetPosition().y - 0.4);
				}

				else if (player->getFacingDirection() == 2)
				{
					player->setCurrentState(L"SHOOT_LEFT");
					bulletDef.position.Set(player->getBody()->GetPosition().x - 1, player->getBody()->GetPosition().y - 0.4);
				}

				else if (player->getFacingDirection() == 1)
				{
					player->setCurrentState(L"SHOOT_UP");
					bulletDef.position.Set(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y - 1);
				}

				else if (player->getFacingDirection() == 0)
				{
					player->setCurrentState(L"SHOOT_DOWN");
					bulletDef.position.Set(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y + 1);
				}



				b2Body *bullet2 = physics->getWorld()->CreateBody(&bulletDef);

				b2PolygonShape bulletShape;
				bulletShape.SetAsBox(.1, .1);

				b2FixtureDef bulletFixtureDef;
				bulletFixtureDef.shape = &bulletShape;
				bulletFixtureDef.density = 1;

				bulletFixtureDef.filter.categoryBits = CATEGORY_PLAYERBULLET;
				bulletFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_HLASER | CATEGORY_VLASER | CATEGORY_LASERRIFLE);

				b2Fixture *bulletF = bullet2->CreateFixture(&bulletFixtureDef);

				UserData *bu = new UserData();
				bu->setBot(bot);
				bu->isbot = true;

				bulletF->SetUserData(bu);

				bot->setBody(bullet2);
				pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64);

				if (player->getFacingDirection() == 2)
				{
					bot->getBody()->SetLinearVelocity(b2Vec2(-25.0, 0.0));
				}

				else if (player->getFacingDirection() == 3)
				{
					bot->getBody()->SetLinearVelocity(b2Vec2(25.0, 0.0));
				}

				else if (player->getFacingDirection() == 1)
				{
					bot->getBody()->SetLinearVelocity(b2Vec2(0.0, -25.0));
				}

				else if (player->getFacingDirection() == 0)
				{
					bot->getBody()->SetLinearVelocity(b2Vec2(0.0, 25.0));
				}



			}

			if (player->getWeapon() == 2){
				GameStateManager *gsm = game->getGSM();
				Physics *physics = game->getGSM()->getPhysics();
				SpriteManager *spriteManager = gsm->getSpriteManager();
				AnimatedSprite *player = spriteManager->getPlayer();

				//int x = 11 * 64;
				//int y = 2 * 64;

				if (player->getFacingDirection() == 3 || player->getFacingDirection() == 2){

						AnimatedSpriteType *bullet = gsm->getSpriteManager()->getSpriteType(7);
						RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

						PhysicalProperties *pp = bot->getPhysicalProperties();
						bot->setSpriteType(bullet);
						bot->setCurrentState(BULLET);
						bot->setAlpha(255);
						//pp->setPosition(x, y);
						spriteManager->addBot(bot, game->getGSM()->getWorld());
						bot->affixTightAABBBoundingVolume();


						b2BodyDef bulletDef;
						bulletDef.type = b2_dynamicBody;

						if (player->getFacingDirection()==3)
						{
							player->setCurrentState(L"SHOOT_RIGHT");
							bulletDef.position.Set(player->getBody()->GetPosition().x + 1, player->getBody()->GetPosition().y - 0.4);
						}

						else if (player->getFacingDirection() == 2)
						{
							player->setCurrentState(L"SHOOT_LEFT");
							bulletDef.position.Set(player->getBody()->GetPosition().x - 1, player->getBody()->GetPosition().y - 0.4);
						}

						else if (player->getFacingDirection() == 1)
						{
							player->setCurrentState(L"SHOOT_UP");
							bulletDef.position.Set(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y - 1 );
						}

						else if (player->getFacingDirection() == 0)
						{
							player->setCurrentState(L"SHOOT_DOWN");
							bulletDef.position.Set(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y + 1 );
						}



						b2Body *bullet2 = physics->getWorld()->CreateBody(&bulletDef);

						b2PolygonShape bulletShape;
						bulletShape.SetAsBox(.1, .1);

						b2FixtureDef bulletFixtureDef;
						bulletFixtureDef.shape = &bulletShape;
						bulletFixtureDef.density = 1;

						bulletFixtureDef.filter.categoryBits = CATEGORY_PLAYERBULLET;
						bulletFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_HLASER | CATEGORY_VLASER | CATEGORY_LASERRIFLE);

						b2Fixture *bulletF = bullet2->CreateFixture(&bulletFixtureDef);

						UserData *bu3 = new UserData();
						bu3->setBot(bot);
						bu3->isbot = true;

						bulletF->SetUserData(bu3);

						bot->setBody(bullet2);
						pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64);

						if (player->getFacingDirection() == 2)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(-100.0, 0.0));
						}

						else if (player->getFacingDirection() == 3)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(100.0, 0.0));
						}

						else if (player->getFacingDirection() == 1)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(0.0, -100.0));
						}

						else if (player->getFacingDirection() == 0)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(0.0, 100.0));
						
					}
				}
				else if (player->getFacingDirection() == 1 || player->getFacingDirection() == 0){
					

						AnimatedSpriteType *bullet = gsm->getSpriteManager()->getSpriteType(8);
						RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

						PhysicalProperties *pp = bot->getPhysicalProperties();
						bot->setSpriteType(bullet);
						bot->setCurrentState(BULLET);
						bot->setAlpha(255);
						//pp->setPosition(x, y);
						spriteManager->addBot(bot, game->getGSM()->getWorld());
						bot->affixTightAABBBoundingVolume();


						b2BodyDef bulletDef;
						bulletDef.type = b2_dynamicBody;

						if (player->getFacingDirection() == 3)
						{
							player->setCurrentState(L"SHOOT_RIGHT");
							bulletDef.position.Set(player->getBody()->GetPosition().x + 1, player->getBody()->GetPosition().y - 0.4);
						}

						else if (player->getFacingDirection() == 2)
						{
							player->setCurrentState(L"SHOOT_LEFT");
							bulletDef.position.Set(player->getBody()->GetPosition().x - 1 , player->getBody()->GetPosition().y - 0.4);
						}

						else if (player->getFacingDirection() == 1)
						{
							player->setCurrentState(L"SHOOT_UP");
							bulletDef.position.Set(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y - 1);
						}

						else if (player->getFacingDirection() == 0)
						{
							player->setCurrentState(L"SHOOT_DOWN");
							bulletDef.position.Set(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y + 1 );
						}



						b2Body *bullet2 = physics->getWorld()->CreateBody(&bulletDef);

						b2PolygonShape bulletShape;
						bulletShape.SetAsBox(.1, .1);

						b2FixtureDef bulletFixtureDef;
						bulletFixtureDef.shape = &bulletShape;
						bulletFixtureDef.density = 1;

						bulletFixtureDef.filter.categoryBits = CATEGORY_PLAYERBULLET;
						bulletFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_HLASER | CATEGORY_VLASER | CATEGORY_LASERRIFLE);

						b2Fixture *bulletF = bullet2->CreateFixture(&bulletFixtureDef);

						UserData *bu2 = new UserData();
						bu2->setBot(bot);
						bu2->isbot = true;

						bulletF->SetUserData(bu2);

						bot->setBody(bullet2);
						pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64);

						if (player->getFacingDirection() == 2)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(-100.0, 0.0));
						}

						else if (player->getFacingDirection() == 3)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(100.0, 0.0));
						}

						else if (player->getFacingDirection() == 1)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(0.0, -100.0));
						}

						else if (player->getFacingDirection() == 0)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(0.0, 100.0));
						}
					}
				
			}


			if (player->getWeapon() >= 3){
				GameStateManager *gsm = game->getGSM();
				Physics *physics = game->getGSM()->getPhysics();
				SpriteManager *spriteManager = gsm->getSpriteManager();
				AnimatedSprite *player = spriteManager->getPlayer();

				AnimatedSpriteType *bullet = gsm->getSpriteManager()->getSpriteType(7);


				for (int i = 0; i < 3; i++){

					if (player->getFacingDirection() == 3 || player->getFacingDirection() == 2){

						AnimatedSpriteType *bullet = gsm->getSpriteManager()->getSpriteType(7);
						RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

						PhysicalProperties *pp = bot->getPhysicalProperties();
						bot->setSpriteType(bullet);
						bot->setCurrentState(BULLET);
						bot->setAlpha(255);
						//pp->setPosition(x, y);
						spriteManager->addBot(bot, game->getGSM()->getWorld());
						bot->affixTightAABBBoundingVolume();


						b2BodyDef bulletDef;
						bulletDef.type = b2_dynamicBody;

						if (player->getFacingDirection() == 3)
						{
							player->setCurrentState(L"SHOOT_RIGHT");
							bulletDef.position.Set(player->getBody()->GetPosition().x + 1, player->getBody()->GetPosition().y - 0.4);
						}

						else if (player->getFacingDirection() == 2)
						{
							player->setCurrentState(L"SHOOT_LEFT");
							bulletDef.position.Set(player->getBody()->GetPosition().x - 1, player->getBody()->GetPosition().y - 0.4);
						}

						else if (player->getFacingDirection() == 1)
						{
							player->setCurrentState(L"SHOOT_UP");
							bulletDef.position.Set(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y - 1);
						}

						else if (player->getFacingDirection() == 0)
						{
							player->setCurrentState(L"SHOOT_DOWN");
							bulletDef.position.Set(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y + 1);
						}



						b2Body *bullet2 = physics->getWorld()->CreateBody(&bulletDef);

						b2PolygonShape bulletShape;
						bulletShape.SetAsBox(.1, .1);

						b2FixtureDef bulletFixtureDef;
						bulletFixtureDef.shape = &bulletShape;
						bulletFixtureDef.density = 1;

						bulletFixtureDef.filter.categoryBits = CATEGORY_PLAYERBULLET;
						bulletFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_HLASER | CATEGORY_VLASER | CATEGORY_LASERRIFLE);

						b2Fixture *bulletF = bullet2->CreateFixture(&bulletFixtureDef);

						UserData *bu3 = new UserData();
						bu3->setBot(bot);
						bu3->isbot = true;

						bulletF->SetUserData(bu3);

						bot->setBody(bullet2);
						pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64);

						if (player->getFacingDirection() == 2)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(-100.0, 10.0 - i * 10));
						}

						else if (player->getFacingDirection() == 3)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(100.0, 10.0 - i * 10));
						}

						else if (player->getFacingDirection() == 1)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(10.0 - i * 10, -100));
						}

						else if (player->getFacingDirection() == 0)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(10.0 - i * 10, 100));

						}
					}
					else if (player->getFacingDirection() == 1 || player->getFacingDirection() == 0){


						AnimatedSpriteType *bullet = gsm->getSpriteManager()->getSpriteType(8);
						RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

						PhysicalProperties *pp = bot->getPhysicalProperties();
						bot->setSpriteType(bullet);
						bot->setCurrentState(BULLET);
						bot->setAlpha(255);
						//pp->setPosition(x, y);
						spriteManager->addBot(bot, game->getGSM()->getWorld());
						bot->affixTightAABBBoundingVolume();


						b2BodyDef bulletDef;
						bulletDef.type = b2_dynamicBody;

						if (player->getFacingDirection() == 3)
						{
							player->setCurrentState(L"SHOOT_RIGHT");
							bulletDef.position.Set(player->getBody()->GetPosition().x + 1, player->getBody()->GetPosition().y - 0.4);
						}

						else if (player->getFacingDirection() == 2)
						{
							player->setCurrentState(L"SHOOT_LEFT");
							bulletDef.position.Set(player->getBody()->GetPosition().x - 1, player->getBody()->GetPosition().y - 0.4);
						}

						else if (player->getFacingDirection() == 1)
						{
							player->setCurrentState(L"SHOOT_UP");
							bulletDef.position.Set(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y - 1);
						}

						else if (player->getFacingDirection() == 0)
						{
							player->setCurrentState(L"SHOOT_DOWN");
							bulletDef.position.Set(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y + 1);
						}



						b2Body *bullet2 = physics->getWorld()->CreateBody(&bulletDef);

						b2PolygonShape bulletShape;
						bulletShape.SetAsBox(.1, .1);

						b2FixtureDef bulletFixtureDef;
						bulletFixtureDef.shape = &bulletShape;
						bulletFixtureDef.density = 1;

						bulletFixtureDef.filter.categoryBits = CATEGORY_PLAYERBULLET;
						bulletFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_HLASER | CATEGORY_VLASER | CATEGORY_LASERRIFLE);

						b2Fixture *bulletF = bullet2->CreateFixture(&bulletFixtureDef);

						UserData *bu2 = new UserData();
						bu2->setBot(bot);
						bu2->isbot = true;

						bulletF->SetUserData(bu2);

						bot->setBody(bullet2);
						pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64);
						if (player->getFacingDirection() == 2)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(-100.0, 10.0 - i * 10));
						}

						else if (player->getFacingDirection() == 3)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(100.0, 10.0 - i * 10));
						}

						else if (player->getFacingDirection() == 1)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(10.0 - i * 10, -100));
						}

						else if (player->getFacingDirection() == 0)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(10.0 - i * 10, 100));

						}
					}
				}


			}

			if (player->getWeapon() == 1){
				GameStateManager *gsm = game->getGSM();
				Physics *physics = game->getGSM()->getPhysics();
				SpriteManager *spriteManager = gsm->getSpriteManager();
				AnimatedSprite *player = spriteManager->getPlayer();

				AnimatedSpriteType *bullet = gsm->getSpriteManager()->getSpriteType(2);

				for (int i = 0; i < 3; i++){

						AnimatedSpriteType *bullet = gsm->getSpriteManager()->getSpriteType(2);
						RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

						PhysicalProperties *pp = bot->getPhysicalProperties();
						bot->setSpriteType(bullet);
						bot->setCurrentState(BULLET);
						bot->setAlpha(255);
						//pp->setPosition(x, y);
						spriteManager->addBot(bot, game->getGSM()->getWorld());
						bot->affixTightAABBBoundingVolume();


						b2BodyDef bulletDef;
						bulletDef.type = b2_dynamicBody;

						if (player->getFacingDirection() == 3)
						{
							player->setCurrentState(L"SHOOT_RIGHT");
							bulletDef.position.Set(player->getBody()->GetPosition().x + 1, player->getBody()->GetPosition().y - 0.4);
						}

						else if (player->getFacingDirection() == 2)
						{
							player->setCurrentState(L"SHOOT_LEFT");
							bulletDef.position.Set(player->getBody()->GetPosition().x - 1, player->getBody()->GetPosition().y - 0.4);
						}

						else if (player->getFacingDirection() == 1)
						{
							player->setCurrentState(L"SHOOT_UP");
							bulletDef.position.Set(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y - 1);
						}

						else if (player->getFacingDirection() == 0)
						{
							player->setCurrentState(L"SHOOT_DOWN");
							bulletDef.position.Set(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y + 1);
						}



						b2Body *bullet2 = physics->getWorld()->CreateBody(&bulletDef);

						b2PolygonShape bulletShape;
						bulletShape.SetAsBox(.1, .1);

						b2FixtureDef bulletFixtureDef;
						bulletFixtureDef.shape = &bulletShape;
						bulletFixtureDef.density = 1;

						bulletFixtureDef.filter.categoryBits = CATEGORY_PLAYERBULLET;
						bulletFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_HLASER | CATEGORY_VLASER | CATEGORY_LASERRIFLE);

						b2Fixture *bulletF = bullet2->CreateFixture(&bulletFixtureDef);

						UserData *bu3 = new UserData();
						bu3->setBot(bot);
						bu3->isbot = true;

						bulletF->SetUserData(bu3);

						bot->setBody(bullet2);
						pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64);

						if (player->getFacingDirection() == 2)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(-25.0, 5.0 - i * 5));
						}

						else if (player->getFacingDirection() == 3)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(25.0, 5.0 - i * 5));
						}

						else if (player->getFacingDirection() == 1)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(5.0 - i * 5, -25));
						}

						else if (player->getFacingDirection() == 0)
						{
							bot->getBody()->SetLinearVelocity(b2Vec2(5.0 - i * 5, 25));

						}
				}


			}


		}


		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;
		if (input->isKeyDown(UP_KEY))
		{
			viewportVy -= MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(DOWN_KEY))
		{
			viewportVy += MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(LEFT_KEY))
		{
			viewportVx -= MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(RIGHT_KEY))
		{
			viewportVx += MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}

		

		if (viewportMoved)
		{
			viewport->moveViewport((int)floor(viewportVx + 0.5f), (int)floor(viewportVy + 0.5f), game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
		
		}
		
		else 
		{
			
			viewport->moveViewport((((player->getBody()->GetPosition().x * 64) - (viewport->getViewportWidth() / 2)) - viewport->getViewportX()), (((player->getBody()->GetPosition().y * 64) - (viewport->getViewportHeight() / 2)) - viewport->getViewportY()), game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
		
		}


			
		
	}

	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if ((input->isKeyDownForFirstTime(C_KEY))
		&& input->isKeyDown(VK_SHIFT))
	{
		Cursor *cursor = game->getGUI()->getCursor();
		unsigned int id = cursor->getActiveCursorID();
		id++;
		if (id == cursor->getNumCursorIDs())
			id = 0;		
		cursor->setActiveCursorID(id);
	}

	// LET'S MESS WITH THE TARGET FRAME RATE IF THE USER PRESSES THE HOME OR END KEYS
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();
	int fps = timer->getTargetFPS();

	// THIS SPEEDS UP OUR GAME LOOP AND THUS THE GAME, NOTE THAT WE COULD ALTERNATIVELY SCALE
	// DOWN THE GAME LOGIC (LIKE ALL VELOCITIES) AS WE SPEED UP THE GAME. THAT COULD PROVIDE
	// A BETTER PLAYER EXPERIENCE
	if (input->isKeyDown(VK_HOME) && (fps < MAX_FPS))
		timer->setTargetFPS(fps + FPS_INC);

	// THIS SLOWS DOWN OUR GAME LOOP, BUT WILL NOT GO BELOW 5 FRAMES PER SECOND
	else if (input->isKeyDown(VK_END) && (fps > MIN_FPS))
		timer->setTargetFPS(fps - FPS_INC);



	if (input->isKeyDownForFirstTime(P_KEY))
	{
		b2World* b2world = gsm->getPhysics()->getWorld();
		while (b2world->GetBodyCount() != 0){
			b2Body* body = b2world->GetBodyList();
			b2world->DestroyBody(body);
		}
		gsm->unloadCurrentLevel();
		game->getGraphics()->clearWorldTextures();

		if (player->getCurrentLevel() == 1){
			player->setHurtable(0);
			game->getDataLoader()->loadWorld3(game, game->getCurrentLevelFileName());

		}
		if (player->getCurrentLevel()==0){
			player->setHurtable(0);
			game->getDataLoader()->loadWorld2(game, game->getCurrentLevelFileName());
			
		}
		
		
	}
	/*
	if (player->getHealth() == 1){
		player->setHealth(11);
		player->setLives(player->getLives()-1);
		player->setHurtable(0);
		player->setWeapon(0);
		b2World* b2world = gsm->getPhysics()->getWorld();
		while (b2world->GetBodyCount() != 0){
			b2Body* body = b2world->GetBodyList();
			b2world->DestroyBody(body);
		}
		gsm->unloadCurrentLevel();
		if (player->getCurrentLevel() == 0)
            game->getDataLoader()->loadWorld(game, game->getCurrentLevelFileName());
		if (player->getCurrentLevel() == 1)
			game->getDataLoader()->loadWorld2(game, game->getCurrentLevelFileName());
		if (player->getCurrentLevel() == 2)
			game->getDataLoader()->loadWorld3(game, game->getCurrentLevelFileName());
	}
	*/

	if (player->getCurrentLevel() == 1 && (player->getPhysicalProperties()->getX() >= 45 * 64)){
		b2World* b2world = gsm->getPhysics()->getWorld();
		while (b2world->GetBodyCount() != 0){
			b2Body* body = b2world->GetBodyList();
			b2world->DestroyBody(body);
		}
		gsm->unloadCurrentLevel();
		game->getGraphics()->clearWorldTextures();
		player->setHurtable(0);
		game->getDataLoader()->loadWorld3(game, game->getCurrentLevelFileName());
	}

	if (player->getCurrentLevel() == 0 && (player->getPhysicalProperties()->getX() >= 47*64)){
		b2World* b2world = gsm->getPhysics()->getWorld();
		while (b2world->GetBodyCount() != 0){
			b2Body* body = b2world->GetBodyList();
			b2world->DestroyBody(body);
		}
		gsm->unloadCurrentLevel();
		game->getGraphics()->clearWorldTextures();
		player->setHurtable(0);
			game->getDataLoader()->loadWorld2(game, game->getCurrentLevelFileName());
	}


	if (player->getHealth() == 1){
		b2World* b2world = gsm->getPhysics()->getWorld();
		while (b2world->GetBodyCount() != 0){
			b2Body* body = b2world->GetBodyList();
			b2world->DestroyBody(body);
		}
		gsm->unloadCurrentLevel();
		game->getGraphics()->clearWorldTextures();
		player->setHurtable(0);
		player->setLives(player->getLives() - 1);
		player->setDeathCounter(player->getDeathCounter() + 1);
		if (player->getCurrentLevel()==0)
		game->getDataLoader()->loadWorld(game, game->getCurrentLevelFileName());
		if (player->getCurrentLevel() == 1)
			game->getDataLoader()->loadWorld2(game, game->getCurrentLevelFileName());
		if (player->getCurrentLevel() == 2)
			game->getDataLoader()->loadWorld3(game, game->getCurrentLevelFileName());
	}

	if (player->getLives() == 0){
		GameStateManager *gsm = game->getGSM();
		game->setCurrentLevelFileName(W_LEVEL_1_NAME);
		
		player->setDeathCounter(0);
		b2World* b2world = gsm->getPhysics()->getWorld();
		while (b2world->GetBodyCount() != 0){
			b2Body* body = b2world->GetBodyList();
			b2world->DestroyBody(body);
		}
		gsm->unloadCurrentLevel();
		game->quitGame();

	
	}

	



}

void BugsKeyEventHandler::Shoot(Game *game, double x, double y, double dX, double dY){

	GameStateManager *gsm = game->getGSM();
	Physics *physics = game->getGSM()->getPhysics();
	SpriteManager *spriteManager = gsm->getSpriteManager();
	AnimatedSprite *player = spriteManager->getPlayer();

	AnimatedSpriteType *bullet = gsm->getSpriteManager()->getSpriteType(5);

	RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

	PhysicalProperties *pp = bot->getPhysicalProperties();
	//int x = 11 * 64;
	//int y = 2 * 64;

	bot->setSpriteType(bullet);
	bot->setCurrentState(BULLET);
	bot->setAlpha(255);
	//pp->setPosition(x, y);
	spriteManager->addBot(bot, game->getGSM()->getWorld());
	bot->affixTightAABBBoundingVolume();
	b2BodyDef bulletDef;
	bulletDef.type = b2_dynamicBody;
	bulletDef.position.Set(x, y);
	b2Body *bullet2 = physics->getWorld()->CreateBody(&bulletDef);
	b2PolygonShape bulletShape;
	bulletShape.SetAsBox(.1, .1);
	b2FixtureDef bulletFixtureDef;
	bulletFixtureDef.shape = &bulletShape;
	bulletFixtureDef.density = 1;
	bulletFixtureDef.filter.categoryBits = CATEGORY_ALIENBULLET;
	bulletFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_PLAYERBULLET | CATEGORY_HLASER | CATEGORY_ALIENBULLET | CATEGORY_VLASER | CATEGORY_MALIEN | CATEGORY_RALIEN | CATEGORY_LASERRIFLE);


	b2Fixture *alienBullet = bullet2->CreateFixture(&bulletFixtureDef);

	UserData *bub = new UserData();
	bub->setBot(bot);
	bub->isbot = true;

	alienBullet->SetUserData(bub);
	bot->setBody(bullet2);
	pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64);
	bot->getBody()->SetLinearVelocity(b2Vec2(dX, dY));
}