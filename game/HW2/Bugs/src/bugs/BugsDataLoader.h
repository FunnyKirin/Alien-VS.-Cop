/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	BugsDataLoader.h

	This class provides a custom importer for the BugsGame to import
	game data, gui data, and world (i.e. level) data.
*/
#pragma once
#include "bugs_VS\stdafx.h"
#include "sssf\data_loader\GameDataLoader.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"
#include <Box2D/Box2D.h>

class BugsDataLoader : public GameDataLoader
{
private:
	// WE NEED THESE GUYS TO INIT OUR WINDOWS WINDOW
	HINSTANCE hInstance;
	int nCmdShow;

public:
	BugsDataLoader()	{}
	~BugsDataLoader()	{}

	const short CATEGORY_PLAYER = 0x0001;  // 0000000000000001 in binary
	const short CATEGORY_MALIEN = 0x0002; // 0000000000000010 in binary
	const short CATEGORY_RALIEN = 0x0004;
	const short CATEGORY_PLAYERBULLET = 0x0008;
	const short CATEGORY_ALIENBULLET = 0x0010;
	const short CATEGORY_HLASER = 0x0020;
	const short CATEGORY_VLASER = 0x0040;
	const short CATEGORY_LASERRIFLE = 0x0080;



	// INLINED MUTATOR METHOD
	void initWinHandle(HINSTANCE initHInstance, int initNCmdShow)
	{
		hInstance = initHInstance;
		nCmdShow = initNCmdShow;
	}


	// THESE SHOULD BE LOADING DATA FROM FILES. THEY
	// ARE ALL DEFINED IN BugsDataLoader.cpp
	void loadGame(Game *game, wstring gameInitFile);
	void loadGUI(Game *game, wstring guiInitFile);
	void loadWorld(Game *game, wstring levelInitFile);

	// THESE ARE HARD-CODED EXAMPLES OF GUI DATA LOADING
	void hardCodedLoadGUIExample(Game *game);

	// SOME HELPER METHODS FOR SETTING UP THE GAME
	void initColors(GameGraphics *graphics,	map<wstring,wstring> *properties);
	void initViewport(GameGUI *gui, map<wstring,wstring> *properties);
	void makeRandomJumpingBot(Game *game, AnimatedSpriteType *randomJumpingBotType, float initX, float initY);
	//void spacePressed(Game *game);

	// AND SOME HELPER METHODS FOR LOADING THE GUI
	void initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initMainMenu(GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void makeBot(Game *game, int, int, int, wstring);
	void makeTurret(Game *game, int, int, int, wstring);
	void makeWall(Game *game, int x, int y);
	void BugsDataLoader::loadWorld2(Game *game, wstring levelInitFile);
	void BugsDataLoader::loadWorld3(Game *game, wstring levelInitFile);

};