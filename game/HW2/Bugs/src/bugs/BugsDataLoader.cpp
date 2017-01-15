#include "bugs_VS\stdafx.h"
#include "LuaPlus.h"
using namespace LuaPlus;
#include "stdio.h"
using namespace std;

// Bugs GAME INCLUDES
#include "bugs\BugsButtonEventHandler.h"
#include "bugs\BugsDataLoader.h"
#include "bugs\BugsGame.h"
#include "bugs\BugsKeyEventHandler.h"
#include "bugs\BugsTextGenerator.h"

// GAME OBJECT INCLUDES
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\ai\bots\RandomJumpingBot.h"
#include "sssf\gsm\ai\pathfinding\OrthographicGridPathfinder.h"
#include "sssf\gsm\sprite\TopDownSprite.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"

// WINDOWS PLATFORM INCLUDES
#include "sssf\platforms\Windows\WindowsOS.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

// DIRECTX INCLUDES
#include "sssf\platforms\DirectX\DirectXGraphics.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"

// TMX MAP LOADING
#include "tmxmi\TMXMapImporter.h"

// ANIMATED SPRITE TYPE LOADING
#include "psti\PoseurSpriteTypesImporter.h"

#include <Box2D/Box2D.h>
#include "sssf\gsm\ai\bots\RandomFloatingBot.h"

#include "sssf\gsm\physics\UserData.h"


/*
	loadGame - This method loads the setup game data into the game and
	constructs all the needed objects for the game to work.
	*/
void BugsDataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	map<wstring, wstring> *properties = new map<wstring, wstring>();
	loadGameProperties(game, properties, gameInitFile);

	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	wstring titleProp = (*properties)[W_TITLE];
	wstring useFullscreenProp = (*properties)[W_USE_FULLSCREEN_MODE];
	bool useFullscreen = false;
	if (useFullscreenProp.compare(L"true") == 0)
		useFullscreen = true;

	// GET THE SCREEN WIDTH AND HEIGHT
	int screenWidth, screenHeight;
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	WindowsOS *BugsOS = new WindowsOS(hInstance,
		nCmdShow,
		useFullscreen,
		titleProp,
		screenWidth, screenHeight,
		game);

	int textFontSize;
	wstring textFontSizeProp = (*properties)[W_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *BugsGraphics = new DirectXGraphics(game);
	BugsGraphics->init(screenWidth, screenHeight);
	BugsGraphics->initGraphics(BugsOS, useFullscreen);
	BugsGraphics->initTextFont(textFontSize);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(BugsGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *BugsInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *BugsTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initplatforms((GameGraphics*)BugsGraphics,
		(GameInput*)BugsInput,
		(GameOS*)BugsOS,
		(GameTimer*)BugsTimer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	BugsTextGenerator *bugsTextGenerator = new BugsTextGenerator();
	bugsTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(W_DEBUG_FILE);
	text->setTextGenerator((TextGenerator*)bugsTextGenerator);

	// INIT THE VIEWPORT TOO
	initViewport(game->getGUI(), properties);

	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	delete properties;
}

/*
	initColors - this helper method loads the color key, used for loading
	images, and the font color, used for rendering text.
	*/
void BugsDataLoader::initColors(GameGraphics *graphics,
	map<wstring, wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[W_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[W_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[W_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[W_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[W_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[W_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}

/*
	loadGUI - This method loads all the GUI assets described in the guiInitFile
	argument. Note that we are loading all GUI art for all GUIs when the application
	first starts. We'll learn a better technique later in the semester.
	*/
void BugsDataLoader::loadGUI(Game *game, wstring guiInitFile)
{
	// WE'RE JUST GOING TO IGNORE THE GUI FILE FOR NOW.
	// FOR THE MOMENT WE ARE CALLING THIS HARD-CODED GUI LOADER
	hardCodedLoadGUIExample(game);
}

/*
	loadLevel - This method should load the data the level described by the
	levelInitFile argument in to the Game's game state manager.
	*/
void BugsDataLoader::loadWorld(Game *game, wstring levelInitFile)
{

	//AVCContactListener *listener = new AVCContactListener();


	//bugsGame->getGSM()->getPhysics()->getWorld()->SetContactListener(&listener);



	LuaState* luaPState = LuaState::Create();
	int result = luaPState->DoFile("LuaScript.lua");
	// Get a global variable
	LuaObject healthObj = luaPState->GetGlobal("PlayerHealth");
	int health = healthObj.GetInteger();
	LuaObject livesObj = luaPState->GetGlobal("PlayerLives");
	int lives = livesObj.GetInteger();
	GameStateManager *gsm = game->getGSM();
	b2World* b2world = gsm->getPhysics()->getWorld();
	while (b2world->GetBodyCount() != 0){
		b2Body* body = b2world->GetBodyList();
		b2world->DestroyBody(body);
	}

	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	tmxMapImporter.loadWorld(game, W_LEVEL_1_DIR, W_LEVEL_1_NAME);
	TiledLayer* tileLayer = (TiledLayer*)game->getGSM()->getWorld()->getLayers()->at(1);

	//LOADS SURROUNDING MAP TILES
	
	for (int i = 0; i < 47; i++){
		makeWall(game, i, 0);
	}
	for (int i = 0; i < 49; i++){
		makeWall(game, 0, i);
	}

	for (int i = 0; i < 47; i++){
		makeWall(game, i, 48);
	}

	for (int i = 0; i < 43; i++){
		makeWall(game, 46, i);
	}

	for (int i = 46; i < 49; i++){
		makeWall(game, 46, i);
	}


	for (int i = 0; i < 11; i++){
		makeWall(game, i, 14);
	}

	for (int i = 14; i < 45; i++){
		makeWall(game, i, 14);
	}

	for (int i = 2; i < 9; i++){
		makeWall(game, i, 24);
	}

	for (int i = 12; i < 21; i++){
		makeWall(game, i, 24);
	}
	for (int i = 24; i < 39; i++){
		makeWall(game, i, 24);
	}
	for (int i = 42; i < 45; i++){
		makeWall(game, i, 24);
	}

	for (int i = 26; i < 29; i++){
		makeWall(game, 32, i);
	}
	for (int i = 32; i < 41; i++){
		makeWall(game, 32, i);
	}
	for (int i = 44; i < 47; i++){
		makeWall(game, 32, i);
	}
	for (int i = 34; i < 45; i++){
		makeWall(game, i, 36);
	}
	for (int i = 2; i < 5; i++){
		makeWall(game, 24, i);
	}
	for (int i = 8; i < 13; i++){
		makeWall(game, 24, i);
	}


	// SPECIFY WHO WILL DO THE PATHFINDING
	
	World *world = gsm->getWorld();

	OrthographicGridPathfinder *pathfinder = new OrthographicGridPathfinder(game);
	SpriteManager *spriteManager = gsm->getSpriteManager();
	spriteManager->setPathfinder(pathfinder);

	// LOAD THE LEVEL'S SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	// LET'S MAKE A PLAYER SPRITE
	// @TODO - LATER WE'LL LOAD ALL LEVEL DATA FROM A FILE
	Physics *physics = gsm->getPhysics();
	//	physics->setGravity(W_GRAVITY);
	AnimatedSprite *player = spriteManager->getPlayer();

	

	// NOTE THAT RED BOX MAN IS SPRITE ID 1
	game->getGraphics()->toggleDebugTextShouldBeRendered();
	AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(0);
	player->setSpriteType(playerSpriteType);
	player->setHealth(health);
	player->setFacingDirection(0);
	player->setWeapon(0);
	player->setLives(lives-player->getDeathCounter());

	player->setAlpha(255);
	player->setCurrentState(IDLE);
	player->setOnTileThisFrame(false);
	player->setOnTileLastFrame(false);
	player->affixTightAABBBoundingVolume();
	player->setCurrentLevel(0);
	

	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.position.Set(2,2);
	


	b2Body *player2 = physics->getWorld()->CreateBody(&myBodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(.5, .8);
	

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 2;

	boxFixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	boxFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_HLASER | CATEGORY_VLASER | CATEGORY_PLAYERBULLET);

	b2Fixture *playerF = player2->CreateFixture(&boxFixtureDef);

	UserData *pu = new UserData();

	pu->setSprite(player);
	pu->isbot = false;

	playerF->SetUserData(pu);

	player->setBody(player2);


	//makeBot(game, 9, 16, 1, IDLE);

	//first room
	makeBot(game, 3, 10, 4, IDLE);
	makeBot(game, 3, 16, 7, IDLE);
	makeBot(game, 3, 12, 10, IDLE);
	makeBot(game, 3, 20, 4, IDLE);
	makeBot(game, 3, 16, 12, IDLE);
	makeBot(game, 3, 4, 10, IDLE);
	makeBot(game, 3, 14, 3, IDLE);
	makeBot(game, 3, 21, 7, IDLE);
	makeBot(game, 3, 20, 12, IDLE);

	//lobby
	makeBot(game, 3, 8, 18, IDLE);
	makeBot(game, 3, 13, 21, IDLE);
	makeBot(game, 3, 25, 21, IDLE);
	makeBot(game, 3, 42, 18, IDLE);
	makeBot(game, 3, 39, 22, IDLE);
	makeBot(game, 3, 31, 18, IDLE);
	makeBot(game, 3, 17, 18, IDLE);

	makeBot(game, 4, 9, 22, IDLE);
	makeBot(game, 4, 24, 18, IDLE);
	makeBot(game, 4, 36, 19, IDLE);
	makeBot(game, 4, 3, 19, IDLE);


	//right top room
	makeBot(game, 6, 40, 7, L"Weapen");
	makeBot(game, 3, 32, 2, IDLE);
	makeBot(game, 3, 32, 5, IDLE);
	makeBot(game, 3, 32, 8, IDLE);
	makeBot(game, 3, 32, 11, IDLE);
	makeBot(game, 4, 36, 5, IDLE);
	makeBot(game, 4, 36, 10, IDLE);
	makeBot(game, 4, 38, 8, IDLE);

	//large room
	makeBot(game, 3, 13, 29, IDLE);
	makeBot(game, 3, 6, 28, IDLE);
	makeBot(game, 3, 6, 34, IDLE);
	makeBot(game, 3, 6, 39, IDLE);
	makeBot(game, 3, 19, 28, IDLE);
	makeBot(game, 3, 36, 34, IDLE);
	makeBot(game, 3, 29, 28, IDLE);
	makeBot(game, 3, 27, 39, IDLE);
	makeBot(game, 3, 20, 42, IDLE);
	makeBot(game, 3, 14, 43, IDLE);
	makeBot(game, 3, 16, 33, IDLE);
	makeBot(game, 3, 25, 36, IDLE);

	makeBot(game, 4, 8, 30, IDLE);
	makeBot(game, 4, 12, 34, IDLE);
	makeBot(game, 4, 12, 38, IDLE);
	makeBot(game, 4, 8, 42, IDLE);
	makeBot(game, 4, 16, 36, IDLE);
	makeBot(game, 4, 20, 34, IDLE);
	makeBot(game, 4, 20, 38, IDLE);
	makeBot(game, 4, 24, 30, IDLE);
	makeBot(game, 4, 24, 42, IDLE);

	//bottom room 1
	makeBot(game, 3, 37, 28, IDLE);
	makeBot(game, 3, 42, 28, IDLE);

	makeBot(game, 4, 42, 33, IDLE);
	makeBot(game, 4, 43, 30, IDLE);

	//bottom room 2
	makeBot(game, 4, 38, 40, IDLE);
	makeBot(game, 4, 38, 45, IDLE);
	makeBot(game, 4, 42, 40, IDLE);
	makeBot(game, 4, 42, 44, IDLE);



	{AnimatedSpriteType *meleeAlien = gsm->getSpriteManager()->getSpriteType(11);

	RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

	PhysicalProperties *pp = bot->getPhysicalProperties();
	//int x = 11 * 64;
	//int y = 2 * 64;

	bot->setSpriteType(meleeAlien);
	bot->setCurrentState(IDLE);
	bot->setAlpha(255);
	spriteManager->addBot(bot, game->getGSM()->getWorld());
	bot->affixTightAABBBoundingVolume();

	b2BodyDef alienBodyDef;
	alienBodyDef.type = b2_staticBody;
	alienBodyDef.position.Set(2, 2);


	b2Body *meleeAlien2 = physics->getWorld()->CreateBody(&alienBodyDef);

	b2PolygonShape alienShape;
	alienShape.SetAsBox(0, 0);

	b2FixtureDef alienFixtureDef;
	alienFixtureDef.shape = &alienShape;
	alienFixtureDef.density = 1;
	alienFixtureDef.isSensor = true;



	b2Fixture *meleeAlienF = meleeAlien2->CreateFixture(&alienFixtureDef);

	UserData *u6 = new UserData();
	u6->setBot(bot);
	u6->isbot = true;
	meleeAlienF->SetUserData(u6);

	bot->setBody(meleeAlien2);
	pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64);
	}

	
	{	AnimatedSpriteType *meleeAlien8 = gsm->getSpriteManager()->getSpriteType(12);

	RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

	PhysicalProperties *pp = bot->getPhysicalProperties();
	//int x = 11 * 64;
	//int y = 2 * 64;

	bot->setSpriteType(meleeAlien8);
	bot->setCurrentState(L"h1");
	bot->setAlpha(255);
	spriteManager->addBot(bot, game->getGSM()->getWorld());
	bot->affixTightAABBBoundingVolume();

	b2BodyDef alienBodyDef;
	alienBodyDef.type = b2_dynamicBody;
	
	alienBodyDef.position.Set(2, 2);
	b2Body *meleeAlien2 = physics->getWorld()->CreateBody(&alienBodyDef);

	b2PolygonShape alienShape;
	alienShape.SetAsBox(0.001, 0.001);

	b2FixtureDef alienFixtureDef;
	alienFixtureDef.shape = &alienShape;
	alienFixtureDef.density = 1;
	alienFixtureDef.isSensor = true;

	alienFixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_VLASER | CATEGORY_HLASER | CATEGORY_PLAYER | CATEGORY_MALIEN | CATEGORY_RALIEN);

	b2Fixture *meleeAlienF = meleeAlien2->CreateFixture(&alienFixtureDef);

	UserData *u7 = new UserData();
	u7->setBot(bot);
	u7->isbot = true;
	meleeAlienF->SetUserData(u7);

	bot->setBody(meleeAlien2);
	}

	{	AnimatedSpriteType *meleeAlien8 = gsm->getSpriteManager()->getSpriteType(13);

	RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

	PhysicalProperties *pp = bot->getPhysicalProperties();
	//int x = 11 * 64;
	//int y = 2 * 64;

	bot->setSpriteType(meleeAlien8);
	bot->setCurrentState(L"l3");
	bot->setAlpha(255);
	spriteManager->addBot(bot, game->getGSM()->getWorld());
	bot->affixTightAABBBoundingVolume();

	b2BodyDef alienBodyDef;
	alienBodyDef.type = b2_dynamicBody;

	alienBodyDef.position.Set(2, 2);
	b2Body *meleeAlien2 = physics->getWorld()->CreateBody(&alienBodyDef);

	b2PolygonShape alienShape;
	alienShape.SetAsBox(0.001, 0.001);

	b2FixtureDef alienFixtureDef;
	alienFixtureDef.shape = &alienShape;
	alienFixtureDef.density = 1;
	alienFixtureDef.isSensor = true;

	alienFixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_VLASER | CATEGORY_HLASER | CATEGORY_PLAYER | CATEGORY_MALIEN | CATEGORY_RALIEN);

	b2Fixture *meleeAlienF = meleeAlien2->CreateFixture(&alienFixtureDef);

	UserData *u7 = new UserData();
	u7->setBot(bot);
	u7->isbot = true;
	meleeAlienF->SetUserData(u7);

	bot->setBody(meleeAlien2);
	}
	
}


void BugsDataLoader::loadWorld2(Game *game, wstring levelInitFile)
{

	//AVCContactListener *listener = new AVCContactListener();


	//bugsGame->getGSM()->getPhysics()->getWorld()->SetContactListener(&listener);



	LuaState* luaPState = LuaState::Create();
	int result = luaPState->DoFile("LuaScript.lua");
	// Get a global variable
	LuaObject healthObj = luaPState->GetGlobal("PlayerHealth");
	int health = healthObj.GetInteger();
	LuaObject livesObj = luaPState->GetGlobal("PlayerLives");
	int lives = livesObj.GetInteger();



	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	tmxMapImporter.loadWorld(game, W_LEVEL_1_DIR, W_LEVEL_2_NAME);
	TiledLayer* tileLayer = (TiledLayer*)game->getGSM()->getWorld()->getLayers()->at(1);

	//LOADS SURROUNDING MAP TILES

	for (int i = 0; i < 25; i++){
		makeWall(game, i, 0);
	}
	for (int i = 24; i < 37; i++){
		makeWall(game, i, 2);
	}
	for (int i = 24; i < 37; i++){
		makeWall(game, i, 10);
	}
	for (int i = 0; i < 9; i++){
		makeWall(game, i, 12);
	}
	for (int i = 16; i < 24; i++){
		makeWall(game, i, 12);
	}
	for (int i = 24; i < 38; i++){
		makeWall(game, i, 14);
	}
	for (int i = 24; i < 38; i++){
		makeWall(game, i, 28);
	}
	for (int i = 4; i < 15; i++){
		makeWall(game, i, 22);
	}
	for (int i = 4; i < 15; i++){
		makeWall(game, i, 34);
	}
	for (int i = 22; i < 44; i++){
		makeWall(game, i, 36);
	}
	for (int i = 4; i < 44; i++){
		makeWall(game, i, 48);
	}
	for (int i = 0; i < 13; i++){
		makeWall(game, 0, i);
	}
	for (int i = 2; i < 10; i++){
		makeWall(game, 36, i);
	}
	for (int i = 14; i < 48; i++){
		makeWall(game, 2, i);
	}
	for (int i = 14; i < 17; i++){
		makeWall(game, 22, i);
	}
	for (int i = 20; i < 36; i++){
		makeWall(game, 22, i);
	}
	for (int i = 14; i < 28; i++){
		makeWall(game, 38, i);
	}
	for (int i = 36; i < 41; i++){
		makeWall(game, 44, i);
	}
	for (int i = 44; i < 48; i++){
		makeWall(game, 44, i);
	}










	// SPECIFY WHO WILL DO THE PATHFINDING
	GameStateManager *gsm = game->getGSM();
	World *world = gsm->getWorld();

	OrthographicGridPathfinder *pathfinder = new OrthographicGridPathfinder(game);
	SpriteManager *spriteManager = gsm->getSpriteManager();
	spriteManager->setPathfinder(pathfinder);

	// LOAD THE LEVEL'S SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	// LET'S MAKE A PLAYER SPRITE
	// @TODO - LATER WE'LL LOAD ALL LEVEL DATA FROM A FILE
	Physics *physics = gsm->getPhysics();
	//	physics->setGravity(W_GRAVITY);
	AnimatedSprite *player = spriteManager->getPlayer();



	// NOTE THAT RED BOX MAN IS SPRITE ID 1
	AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(0);
	player->setSpriteType(playerSpriteType);
	player->setAlpha(255);
	player->setLives(lives - player->getDeathCounter());
	player->setCurrentState(IDLE);
	player->setOnTileThisFrame(false);
	player->setOnTileLastFrame(false);
	player->affixTightAABBBoundingVolume();
	player->setCurrentLevel(1);


	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.position.Set(3, 3);



	b2Body *player2 = physics->getWorld()->CreateBody(&myBodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(.5, .8);


	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 2;
	b2Fixture *playerF = player2->CreateFixture(&boxFixtureDef);

	UserData *pu = new UserData();

	pu->setSprite(player);
	pu->isbot = false;

	playerF->SetUserData(pu);

	player->setBody(player2);

	makeBot(game, 3, 28, 18, IDLE);
	makeBot(game, 3, 28, 24, IDLE);
	makeBot(game, 3, 33, 18, IDLE);
	makeBot(game, 4, 9, 4, IDLE);
	makeBot(game, 4, 28, 5, IDLE);
	makeBot(game, 4, 28, 8, IDLE);
	makeBot(game, 4, 32, 8, IDLE);
	makeBot(game, 6, 30, 22, L"Weapen");
	makeBot(game, 3, 9, 4, IDLE);
	makeBot(game, 3, 12, 26, IDLE);
	makeBot(game, 3, 12, 30, IDLE);
	makeBot(game, 3, 14, 28, IDLE);
	makeBot(game, 3, 12, 38, IDLE);
	makeBot(game, 3, 14, 40, IDLE);
	makeBot(game, 3, 14, 43, IDLE);
	makeBot(game, 3, 12, 45, IDLE);
	makeBot(game, 3, 6, 9, IDLE);
	makeBot(game, 3, 13, 4, IDLE);
	makeBot(game, 3, 13, 8, IDLE);
	makeBot(game, 4, 12, 11, IDLE);
	makeBot(game, 4, 34, 40, IDLE);
	makeBot(game, 4, 34, 44, IDLE);
	makeBot(game, 4, 18, 8, IDLE);
	makeBot(game, 4, 20, 4, IDLE);

	makeTurret(game, 10, 6, 18, IDLE);
	makeTurret(game, 10, 6, 28, IDLE);
	makeTurret(game, 10, 8, 38, IDLE);
	makeTurret(game, 10, 8, 44, IDLE);
	{	AnimatedSpriteType *meleeAlien8 = gsm->getSpriteManager()->getSpriteType(12);

	RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

	PhysicalProperties *pp = bot->getPhysicalProperties();
	//int x = 11 * 64;
	//int y = 2 * 64;

	bot->setSpriteType(meleeAlien8);
	bot->setCurrentState(L"h1");
	bot->setAlpha(255);
	spriteManager->addBot(bot, game->getGSM()->getWorld());
	bot->affixTightAABBBoundingVolume();

	b2BodyDef alienBodyDef;
	alienBodyDef.type = b2_dynamicBody;

	alienBodyDef.position.Set(2, 2);
	b2Body *meleeAlien2 = physics->getWorld()->CreateBody(&alienBodyDef);

	b2PolygonShape alienShape;
	alienShape.SetAsBox(0.001, 0.001);

	b2FixtureDef alienFixtureDef;
	alienFixtureDef.shape = &alienShape;
	alienFixtureDef.density = 1;
	alienFixtureDef.isSensor = true;

	alienFixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_VLASER | CATEGORY_HLASER | CATEGORY_PLAYER | CATEGORY_MALIEN | CATEGORY_RALIEN);
	



	b2Fixture *meleeAlienF = meleeAlien2->CreateFixture(&alienFixtureDef);

	UserData *u7 = new UserData();
	u7->setBot(bot);
	u7->isbot = true;
	meleeAlienF->SetUserData(u7);

	bot->setBody(meleeAlien2);
	}

	{	AnimatedSpriteType *meleeAlien8 = gsm->getSpriteManager()->getSpriteType(13);

	RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

	PhysicalProperties *pp = bot->getPhysicalProperties();
	//int x = 11 * 64;
	//int y = 2 * 64;

	bot->setSpriteType(meleeAlien8);
	bot->setCurrentState(L"l3");
	bot->setAlpha(255);
	spriteManager->addBot(bot, game->getGSM()->getWorld());
	bot->affixTightAABBBoundingVolume();

	b2BodyDef alienBodyDef;
	alienBodyDef.type = b2_dynamicBody;

	alienBodyDef.position.Set(2, 2);
	b2Body *meleeAlien2 = physics->getWorld()->CreateBody(&alienBodyDef);

	b2PolygonShape alienShape;
	alienShape.SetAsBox(0.001, 0.001);

	b2FixtureDef alienFixtureDef;
	alienFixtureDef.shape = &alienShape;
	alienFixtureDef.density = 1;
	alienFixtureDef.isSensor = true;

	alienFixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_VLASER | CATEGORY_HLASER | CATEGORY_PLAYER | CATEGORY_MALIEN | CATEGORY_RALIEN);

	b2Fixture *meleeAlienF = meleeAlien2->CreateFixture(&alienFixtureDef);

	UserData *u7 = new UserData();
	u7->setBot(bot);
	u7->isbot = true;
	meleeAlienF->SetUserData(u7);

	bot->setBody(meleeAlien2);
	}
}

void BugsDataLoader::loadWorld3(Game *game, wstring levelInitFile)
{

	//AVCContactListener *listener = new AVCContactListener();


	//bugsGame->getGSM()->getPhysics()->getWorld()->SetContactListener(&listener);



	LuaState* luaPState = LuaState::Create();
	int result = luaPState->DoFile("LuaScript.lua");
	// Get a global variable
	LuaObject healthObj = luaPState->GetGlobal("PlayerHealth");
	int health = healthObj.GetInteger();
	LuaObject livesObj = luaPState->GetGlobal("PlayerLives");
	int lives = livesObj.GetInteger();



	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	tmxMapImporter.loadWorld(game, W_LEVEL_1_DIR, W_LEVEL_3_NAME);
	TiledLayer* tileLayer = (TiledLayer*)game->getGSM()->getWorld()->getLayers()->at(1);

	//LOADS SURROUNDING MAP TILES


	// SPECIFY WHO WILL DO THE PATHFINDING
	GameStateManager *gsm = game->getGSM();
	World *world = gsm->getWorld();

	OrthographicGridPathfinder *pathfinder = new OrthographicGridPathfinder(game);
	SpriteManager *spriteManager = gsm->getSpriteManager();
	spriteManager->setPathfinder(pathfinder);

	// LOAD THE LEVEL'S SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	// LET'S MAKE A PLAYER SPRITE
	// @TODO - LATER WE'LL LOAD ALL LEVEL DATA FROM A FILE
	Physics *physics = gsm->getPhysics();
	//	physics->setGravity(W_GRAVITY);
	AnimatedSprite *player = spriteManager->getPlayer();



	// NOTE THAT RED BOX MAN IS SPRITE ID 1
	AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(0);
	player->setSpriteType(playerSpriteType);
	player->setAlpha(255);
	player->setLives(lives - player->getDeathCounter());
	player->setCurrentState(IDLE);
	player->setOnTileThisFrame(false);
	player->setOnTileLastFrame(false);
	player->affixTightAABBBoundingVolume();
	player->setCurrentLevel(2);


	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.position.Set(4, 45);



	b2Body *player2 = physics->getWorld()->CreateBody(&myBodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(.5, .8);


	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 2;
	b2Fixture *playerF = player2->CreateFixture(&boxFixtureDef);

	UserData *pu = new UserData();

	pu->setSprite(player);
	pu->isbot = false;

	playerF->SetUserData(pu);

	player->setBody(player2);

	for (int i = 11; i < 38; i++){
		makeWall(game, i, 0);
	}
	for (int i = 4; i < 10; i++){
		makeWall(game, i, 4);
	}
	for (int i = 38; i < 46; i++){
		makeWall(game, i, 4);
	}
	for (int i = 0; i < 4; i++){
		makeWall(game, 10, i);
	}
	for (int i = 0; i < 4; i++){
		makeWall(game, 39, i);
	}
	for (int i = 4; i < 19; i++){
		makeWall(game, i, 12);
	}
	for (int i = 30; i < 45; i++){
		makeWall(game, i, 12);
	}
	for (int i = 1; i < 19; i++){
		makeWall(game, i, 38);
	}
	for (int i = 30; i < 45; i++){
		makeWall(game, i, 38);
	}
	for (int i = 2; i < 45; i++){
		makeWall(game, i, 48);
	}
	for (int i = 6; i < 11; i++){
		makeWall(game, 2, i);
	}
	for (int i = 6; i < 11; i++){
		makeWall(game, 46, i);
	}
	for (int i = 12; i < 39; i++){
		makeWall(game, 18, i);
	}
	for (int i = 12; i < 39; i++){
		makeWall(game, 30, i);
	}
	for (int i = 40; i < 47; i++){
		makeWall(game, 0, i);
	}
	for (int i = 40; i < 47; i++){
		makeWall(game, 46, i);
	}


	makeBot(game, 6, 42, 43, L"Weapen");
	makeBot(game, 3, 14, 42, IDLE);
	makeBot(game, 3, 14, 45, IDLE);
	makeBot(game, 3, 19, 42, IDLE);
	makeBot(game, 3, 24, 45, IDLE);
	makeBot(game, 4, 30, 42, IDLE);
	makeBot(game, 4, 30, 45, IDLE);
	makeBot(game, 4, 27, 30, IDLE);
	makeBot(game, 4, 21, 30, IDLE);
	makeBot(game, 4, 27, 18, IDLE);
	makeBot(game, 4, 21, 18, IDLE);
	makeBot(game, 3, 22, 20, IDLE);
	makeBot(game, 3, 26, 20, IDLE);
	makeBot(game, 3, 22, 34, IDLE);
	makeBot(game, 3, 26, 34, IDLE);

	{AnimatedSpriteType *meleeAlien = gsm->getSpriteManager()->getSpriteType(9);


	RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

	PhysicalProperties *pp = bot->getPhysicalProperties();
	//int x = 11 * 64;
	//int y = 2 * 64;

	bot->setSpriteType(meleeAlien);
	bot->setCurrentState(IDLE);
	bot->setAlpha(255);
	bot->setHealth(100);
	spriteManager->addBot(bot, game->getGSM()->getWorld());
	bot->affixTightAABBBoundingVolume();


	b2BodyDef alienBodyDef;
	alienBodyDef.type = b2_staticBody;
	alienBodyDef.position.Set(24.5, 3);


	b2Body *meleeAlien2 = physics->getWorld()->CreateBody(&alienBodyDef);

	b2PolygonShape alienShape;
	alienShape.SetAsBox(3, 2);

	b2FixtureDef alienFixtureDef;
	alienFixtureDef.shape = &alienShape;
	alienFixtureDef.density = 1;

	alienFixtureDef.filter.categoryBits = CATEGORY_RALIEN;
	alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_LASERRIFLE | CATEGORY_ALIENBULLET);

	b2Fixture *meleeAlienF = meleeAlien2->CreateFixture(&alienFixtureDef);

	UserData *u6 = new UserData();
	u6->setBot(bot);
	u6->isbot = true;
	meleeAlienF->SetUserData(u6);



	bot->setBody(meleeAlien2);

	}

	{	AnimatedSpriteType *meleeAlien8 = gsm->getSpriteManager()->getSpriteType(12);

	RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

	PhysicalProperties *pp = bot->getPhysicalProperties();
	//int x = 11 * 64;
	//int y = 2 * 64;

	bot->setSpriteType(meleeAlien8);
	bot->setCurrentState(L"h1");
	bot->setAlpha(255);
	spriteManager->addBot(bot, game->getGSM()->getWorld());
	bot->affixTightAABBBoundingVolume();

	b2BodyDef alienBodyDef;
	alienBodyDef.type = b2_dynamicBody;

	alienBodyDef.position.Set(4, 45);
	b2Body *meleeAlien2 = physics->getWorld()->CreateBody(&alienBodyDef);

	b2PolygonShape alienShape;
	alienShape.SetAsBox(0.001, 0.001);

	b2FixtureDef alienFixtureDef;
	alienFixtureDef.shape = &alienShape;
	alienFixtureDef.density = 1;
	alienFixtureDef.isSensor = true;


	alienFixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_VLASER | CATEGORY_HLASER | CATEGORY_PLAYER | CATEGORY_MALIEN | CATEGORY_RALIEN);

	b2Fixture *meleeAlienF = meleeAlien2->CreateFixture(&alienFixtureDef);

	UserData *u7 = new UserData();
	u7->setBot(bot);
	u7->isbot = true;
	meleeAlienF->SetUserData(u7);

	bot->setBody(meleeAlien2);
	}

	{	AnimatedSpriteType *meleeAlien8 = gsm->getSpriteManager()->getSpriteType(13);

	RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

	PhysicalProperties *pp = bot->getPhysicalProperties();
	//int x = 11 * 64;
	//int y = 2 * 64;

	bot->setSpriteType(meleeAlien8);
	bot->setCurrentState(L"l3");
	bot->setAlpha(255);
	spriteManager->addBot(bot, game->getGSM()->getWorld());
	bot->affixTightAABBBoundingVolume();

	b2BodyDef alienBodyDef;
	alienBodyDef.type = b2_dynamicBody;

	alienBodyDef.position.Set(4, 45);
	b2Body *meleeAlien2 = physics->getWorld()->CreateBody(&alienBodyDef);

	b2PolygonShape alienShape;
	alienShape.SetAsBox(0.001, 0.001);

	b2FixtureDef alienFixtureDef;
	alienFixtureDef.shape = &alienShape;
	alienFixtureDef.density = 1;
	alienFixtureDef.isSensor = true;

	alienFixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_VLASER | CATEGORY_HLASER | CATEGORY_PLAYER | CATEGORY_MALIEN | CATEGORY_RALIEN);

	b2Fixture *meleeAlienF = meleeAlien2->CreateFixture(&alienFixtureDef);

	UserData *u7 = new UserData();
	u7->setBot(bot);
	u7->isbot = true;
	meleeAlienF->SetUserData(u7);

	bot->setBody(meleeAlien2);
	}
}


void BugsDataLoader::makeRandomJumpingBot(Game *game, AnimatedSpriteType *randomJumpingBotType, float initX, float initY)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Physics *physics = game->getGSM()->getPhysics();
	RandomJumpingBot *bot = new RandomJumpingBot(physics, 30, 120, 40);
	//physics->addCollidableObject(bot);
	PhysicalProperties *pp = bot->getPhysicalProperties();
	pp->setPosition(initX, initY);
	bot->setSpriteType(randomJumpingBotType);
	bot->setCurrentState(JUMPING);
	bot->setAlpha(255);
	spriteManager->addBot(bot, game->getGSM()->getWorld());
	bot->affixTightAABBBoundingVolume();
}

/*
	initBugsGUI - This method builds a GUI for the Bugs Game application.
	Note that we load all the GUI components from this method, including
	the ScreenGUI with Buttons and Overlays and the Cursor.
	*/
void BugsDataLoader::hardCodedLoadGUIExample(Game *game)
{
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// SETUP THE CURSOR VIA OUR HELPER METHOD
	initCursor(gui, guiTextureManager);
	initSplashScreen(game, gui, guiTextureManager);
	initMainMenu(gui, guiTextureManager);
	initInGameGUI(gui, guiTextureManager);
}

/*
	initCursor - initializes a simple little cursor for the gui.
	*/
void BugsDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	imageID = guiTextureManager->loadTexture(W_GREEN_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	imageID = guiTextureManager->loadTexture(W_RED_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(imageIDs,
		*(imageIDs->begin()),
		0,
		0,
		0,
		255,
		32,
		32);
	gui->setCursor(cursor);
}

/*
	initSplashScreen - initializes the game's splash screen gui.
	*/
void BugsDataLoader::initSplashScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *splashScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_SPLASH_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		0,
		0,
		255,
		game->getGraphics()->getScreenWidth(),
		game->getGraphics()->getScreenHeight(),
		false,
		W_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

/*
	initMainMenu - initializes the game's main menu gui.
	*/
void BugsDataLoader::initMainMenu(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_MAIN_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 1366;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		300,
		273,
		0,
		255,
		200,
		100,
		false,
		W_EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(W_START_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_START_IMAGE_MO_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		100,
		280,
		0,
		255,
		200,
		100,
		false,
		W_START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU, mainMenuGUI);
}

/*
	initInGameGUI - initializes the game's in-game gui.
	*/
void BugsDataLoader::initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *inGameGUI = new ScreenGUI();

	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		0,
		0,
		255,
		200,
		100,
		false,
		W_QUIT_COMMAND);
	inGameGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_IN_PROGRESS, inGameGUI);
}

/*
	initViewport - initializes the game's viewport.
	*/
void BugsDataLoader::initViewport(GameGUI *gui, map<wstring, wstring> *properties)
{
	// AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
	// WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
	// THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
	// THE VIEWPORT IN THE Y AXIS
	Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY, toggleOffsetY, screenWidth, screenHeight;
	wstring viewportOffsetXProp = (*properties)[W_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[W_VIEWPORT_OFFSET_Y];
	wstring toggleOffsetYProp = (*properties)[W_TOGGLE_OFFSET_Y];
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	wstringstream(toggleOffsetYProp) >> toggleOffsetY;
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;
	int viewportWidth = screenWidth - viewportOffsetX;
	int viewportHeight = screenHeight - viewportOffsetY;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(viewportOffsetY);
	viewport->setToggleOffsetY(toggleOffsetY);
}

void BugsDataLoader::makeBot(Game *game, int botType, int x, int y, wstring state)
{
	GameStateManager *gsm = game->getGSM();
	World *world = gsm->getWorld();
	SpriteManager *spriteManager = gsm->getSpriteManager();
	AnimatedSpriteType *meleeAlien = gsm->getSpriteManager()->getSpriteType(botType);

	Physics *physics = gsm->getPhysics();


	RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

	PhysicalProperties *pp = bot->getPhysicalProperties();
	//int x = 11 * 64;
	//int y = 2 * 64;

	bot->setSpriteType(meleeAlien);
	bot->setCurrentState(state);
	bot->setAlpha(255);
	spriteManager->addBot(bot, game->getGSM()->getWorld());
	bot->affixTightAABBBoundingVolume();
	bot->setHealth(3);

	b2BodyDef alienBodyDef;
	alienBodyDef.type = b2_dynamicBody;
	alienBodyDef.position.Set(x, y);


	b2Body *meleeAlien2 = physics->getWorld()->CreateBody(&alienBodyDef);

	b2PolygonShape alienShape;
	alienShape.SetAsBox(.5, .8);

	b2FixtureDef alienFixtureDef;
	alienFixtureDef.shape = &alienShape;
	alienFixtureDef.density = 1;

	if (botType == 4){
		alienFixtureDef.filter.categoryBits = CATEGORY_RALIEN;
		alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_LASERRIFLE | CATEGORY_ALIENBULLET);
	}
	if (botType == 3){

		alienFixtureDef.filter.categoryBits = CATEGORY_MALIEN;
		alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_LASERRIFLE | CATEGORY_ALIENBULLET);

	}

	if (botType == 6){

		alienFixtureDef.filter.categoryBits = CATEGORY_LASERRIFLE;
		alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_HLASER | CATEGORY_VLASER | CATEGORY_LASERRIFLE | CATEGORY_MALIEN | CATEGORY_RALIEN);
	}

	if (botType == 7){

		alienFixtureDef.filter.categoryBits = CATEGORY_HLASER;
		alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_LASERRIFLE);
	}

	if (botType == 8){

		alienFixtureDef.filter.categoryBits = CATEGORY_VLASER;
		alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_ALIENBULLET | CATEGORY_PLAYERBULLET | CATEGORY_LASERRIFLE);
	}






	b2Fixture *meleeAlienF = meleeAlien2->CreateFixture(&alienFixtureDef);

	UserData *u6 = new UserData();
	u6->setBot(bot);
	u6->isbot = true;
	meleeAlienF->SetUserData(u6);



	bot->setBody(meleeAlien2);
	pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64);
}

void BugsDataLoader::makeTurret(Game *game, int botType, int x, int y, wstring state)
{
	GameStateManager *gsm = game->getGSM();
	World *world = gsm->getWorld();
	SpriteManager *spriteManager = gsm->getSpriteManager();
	AnimatedSpriteType *meleeAlien = gsm->getSpriteManager()->getSpriteType(botType);

	Physics *physics = gsm->getPhysics();


	RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);

	PhysicalProperties *pp = bot->getPhysicalProperties();
	//int x = 11 * 64;
	//int y = 2 * 64;

	bot->setSpriteType(meleeAlien);
	bot->setCurrentState(state);
	bot->setAlpha(255);
	spriteManager->addBot(bot, game->getGSM()->getWorld());
	bot->affixTightAABBBoundingVolume();
	bot->setHealth(10);

	b2BodyDef alienBodyDef;
	alienBodyDef.type = b2_staticBody;
	alienBodyDef.position.Set(x, y);


	b2Body *meleeAlien2 = physics->getWorld()->CreateBody(&alienBodyDef);

	b2PolygonShape alienShape;
	alienShape.SetAsBox(.5, .8);

	b2FixtureDef alienFixtureDef;
	alienFixtureDef.shape = &alienShape;
	alienFixtureDef.density = 1;


	alienFixtureDef.filter.categoryBits = CATEGORY_RALIEN;
	alienFixtureDef.filter.maskBits = 0xFFFF & ~(CATEGORY_LASERRIFLE | CATEGORY_ALIENBULLET);


	b2Fixture *meleeAlienF = meleeAlien2->CreateFixture(&alienFixtureDef);

	UserData *u6 = new UserData();
	u6->setBot(bot);
	u6->isbot = true;
	meleeAlienF->SetUserData(u6);



	bot->setBody(meleeAlien2);
	pp->setPosition(bot->getBody()->GetPosition().x * 64, bot->getBody()->GetPosition().y * 64);
}

void BugsDataLoader::makeWall(Game *game, int x, int y){
	b2BodyDef tileBodyDef;
	tileBodyDef.type = b2_staticBody;
	tileBodyDef.position.Set(x, y);

	b2Body* staticBody = game->getGSM()->getPhysics()->getWorld()->CreateBody(&tileBodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(1.0, 1.0);

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 1;
	staticBody->CreateFixture(&boxFixtureDef);
}