/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	BugsButtonEventHandler.cpp

	See BugsButtonEventHandler.h for a class description.
*/

#include "bugs_VS\stdafx.h"
#include "bugs\BugsGame.h"
#include "bugs\BugsButtonEventHandler.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"
#include <Box2D\Box2D.h>


void BugsButtonEventHandler::handleButtonEvents(	Game *game, 
													wstring command)
{
	// THE USER PRESSED THE Exit BUTTON ON THE MAIN MENU,
	// SO LET'S SHUTDOWN THE ENTIRE APPLICATION
	if (command.compare(W_EXIT_COMMAND) == 0)
	{
		game->shutdown();
	}
	// THE USER PRESSED THE MOUSE BUTTON ON THE SPLASH
	// SCREEN, SO LET'S GO TO THE MAIN MENU
	else if (command.compare(W_GO_TO_MM_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToMainMenu();
	}
	// THE USER PRESSED THE Start BUTTON ON THE MAIN MENU,
	// SO LET'S START THE GAME FROM THE FIRST LEVEL
	else if (command.compare(W_START_COMMAND) == 0)
	{

		game->startGame();
		game->getGSM()->getSpriteManager()->getPlayer()->setDeathCounter(0);
	}
	// THE USER PRESSED THE Quit BUTTON ON THE IN-GAME MENU,
	// SO LET'S UNLOAD THE LEVEL AND RETURN TO THE MAIN MENU
	else if (command.compare(W_QUIT_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		game->setCurrentLevelFileName(W_LEVEL_1_NAME);
		b2World* b2world = gsm->getPhysics()->getWorld();
		while (b2world->GetBodyCount() != 0){
			b2Body* body = b2world->GetBodyList();
			b2world->DestroyBody(body);
		}
		gsm->unloadCurrentLevel();

		game->quitGame();
	}
}