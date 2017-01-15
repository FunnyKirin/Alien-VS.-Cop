/*
Author: Richard McKenna
Stony Brook University
Computer Science Department

BugsTextGenerator.cpp

See BugsTextGenerator.h for a class description.
*/

#include "bugs_VS\stdafx.h"
#include "bugs\BugsGame.h"
#include "bugs\BugsTextGenerator.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\sprite\TopDownSprite.h"
#include "sssf\text\GameText.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"
#include "sssf\gsm\sprite\QuadTree.h"

static const wstring	W_MOUSE_COORDS_TEXT = L"Mouse: (";
static const int		W_TEXT_X = 200;
static const int		W_TEXT_Y = 10;
static const int		W_PATHFINDING_TEXT_X = 800;
static const int		W_PATHFINDING_TEXT_Y = 10;
static const wstring	W_ROTATION_IN_RADIANS = L"Ant Rotation in Radians: ";
static const wstring	W_ROTATION_IN_DEGREES = L"Ant Rotation in Degrees: ";
static const wstring	W_RADIANS = L" radians";
static const wstring	W_DEGREES = L" degrees";
static const wstring	SIN_THETA = L"sin(theta): ";
static const wstring	COS_THETA = L"cos(theta): ";
static const wstring	W_ANT_CENTER_TEXT = L"Ant Center: (";
static const wstring	W_ANT_GRID_TEXT = L"Ant Center Col,Row: (";
static const wstring	W_ANT_LEFT_TOP_TEXT = L"Ant Left, Top: (";
static const wstring	W_ANT_RIGHT_BOTTOM_TEXT = L"Ant Right, Bottom: (";
static const wstring	W_GUIDE_TEXT = L"Shoot: space key";

static const wstring	W_NODE1 = L"Node 1: ";
static const wstring	W_NODE2 = L"Node 2: ";
static const wstring	W_NODE3 = L"Node 3: ";
static const wstring	W_NODE4 = L"Node 4: ";
static const wstring	W_NODE5 = L"Node 5: ";
static const wstring	W_NODE6 = L"Node 6: ";
static const wstring	W_NODE7 = L"Node 7: ";
static const wstring	W_NODE8 = L"Node 8: ";
static const wstring	W_NODE9 = L"Node 9: ";
static const wstring	W_NODE10 = L"Node 10: ";
static const wstring	W_NODE11 = L"Node 11: ";
static const wstring	W_NODE12 = L"Node 12: ";
static const wstring	W_NODE13 = L"Node 13: ";
static const wstring	W_NODE14 = L"Node 14: ";
static const wstring	W_NODE15 = L"Node 15: ";
static const wstring	W_NODE16 = L"Node 16: ";
static const wstring	W_NODE17 = L"Node 17: ";
static const wstring	W_NODE18 = L"Node 18: ";
static const wstring	W_NODE19 = L"Node 19: ";
static const wstring	W_NODE20 = L"Node 20: ";
static const wstring	W_NODE21 = L"Node 21: ";
static const wstring	W_TOTAL = L"TOTAL: ";

void BugsTextGenerator::appendPathfindingText(Game *game)
{
	wstringstream wss;
	if (game->getGSM()->isGameInProgress())
	{
		GridPathfinder *pathfinder = game->getGSM()->getSpriteManager()->getPathfinder();
		TopDownSprite *ant = game->getGSM()->getSpriteManager()->getPlayer();
		pathfindingText.append(W_ANT_CENTER_TEXT);
		wss << ant->getBoundingVolume()->getCenterX();
		wss << L", ";
		wss << ant->getBoundingVolume()->getCenterY();
		wss << L")\n";
		wss << W_ANT_GRID_TEXT;
		int col = (int)(ant->getBoundingVolume()->getCenterX() / pathfinder->getGridWidth());
		wss << col;
		wss << L", ";
		int row = ant->getBoundingVolume()->getCenterY() / pathfinder->getGridHeight();
		wss << row;
		wss << L")\n";
		wss << W_ANT_LEFT_TOP_TEXT;
		wss << ant->getPhysicalProperties()->getX();
		wss << ", ";
		wss << ant->getPhysicalProperties()->getY();
		wss << ")\n";
		wss << W_ANT_RIGHT_BOTTOM_TEXT;
		wss << ant->getPhysicalProperties()->getX() + ant->getSpriteType()->getTextureWidth();
		wss << ", ";
		wss << ant->getPhysicalProperties()->getY() + ant->getSpriteType()->getTextureHeight();
		wss << ")\n";
		pathfindingText.append(wss.str());
	}
}

void BugsTextGenerator::appendMouseCoords(Game *game)
{
	wstringstream wss;
	WindowsInput *input = (WindowsInput*)game->getInput();
	textToGenerate.append(W_MOUSE_COORDS_TEXT);
	wss << input->getMousePoint()->x;
	wss << L", ";
	wss << input->getMousePoint()->y;
	wss << L")";
	wss << L"\n";
	textToGenerate.append(wss.str());
}


void BugsTextGenerator::appendAntRotationInfo(Game *game)
{
	wstringstream wss;
	TopDownSprite *ant = game->getGSM()->getSpriteManager()->getPlayer();
	//	float rad = ant->getRotationInRadians();

	wss << W_ROTATION_IN_RADIANS;
	//wss << rad;
	wss << W_RADIANS + L"\n";
	wss << W_ROTATION_IN_DEGREES;
	//wss << (rad * 180)/PI;
	wss << W_DEGREES + L"\n";
	wss << SIN_THETA;
	//wss << sin(rad);
	wss << L"\n";
	wss << COS_THETA;
	//wss << cos(rad);
	wss << L"\n";
	textToGenerate.append(wss.str());
}

void BugsTextGenerator::appendGuide(Game *game)
{
	wstringstream wss;
	WindowsInput *input = (WindowsInput*)game->getInput();
	textToGenerate.append(wss.str());
}

/*
initText - Provides an example of how to render text to our screen every frame,
allowing for the text to continually change. This method adds a text object
to the text manager (GameText), which allows it to be updated each frame. This
method should be called only once for each object.
*/
void BugsTextGenerator::initText(Game *game)
{

	// FIRST UPDATE THE TEXT THIS GENERATOR IS USING
	//appendMouseCoords(game);
	//appendAntRotationInfo(game);
	treeText(game);
	// AND THEN HAND IT TO THE TEXT MANAGER, SPECIFYING WHERE IT SHOULD BE RENDERED
	GameText *text = game->getText();
	text->addText(&textToGenerate, W_TEXT_X, W_TEXT_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	// LET'S ALSO DISPLAY SOME STUFF FOR PATHFINDING
	appendPathfindingText(game);
	text->addText(&pathfindingText, W_PATHFINDING_TEXT_X, W_PATHFINDING_TEXT_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());

	appendGuide(game);
}

/*
updateText - updates the text. Note that the text must already have been added
to the text manager via the initText method. Also not that there should be a
separate TextGenerator for each line of text we wish to render.
*/
void BugsTextGenerator::updateText(Game *game)
{
	// THE TEXT MANAGER (GameText) HAS A POINTER TO OUR TEXT, SO WE CAN
	// SIMPLY CLEAR IT OUT OF OLD CHARACTER DATA AND FILL IT IN WITH
	// WHAT WE WANT.
	textToGenerate.clear();
	//appendMouseCoords(game);
	//appendAntRotationInfo(game);
	//treeText(game);
	//pathfindingText.clear();
	//appendPathfindingText(game);
	appendGuide(game);
}

void BugsTextGenerator::treeText(Game *game)
{
	wstringstream wss;
	QuadTree& blah = game->getGSM()->getSpriteManager()->getTree();

	wss << W_TOTAL << blah.allBotList.size() << L"\n";
	wss << W_NODE1 << blah.botList[0].botsVector.size() << L"\n";
	wss << W_NODE2 << blah.botList[1].botsVector.size() << L"\n";
	wss << W_NODE3 << blah.botList[2].botsVector.size() << L"\n";
	wss << W_NODE4 << blah.botList[3].botsVector.size() << L"\n";
	wss << W_NODE5 << blah.botList[4].botsVector.size() << L"\n";
	wss << W_NODE6 << blah.botList[5].botsVector.size() << L"\n";
	wss << W_NODE7 << blah.botList[6].botsVector.size() << L"\n";
	wss << W_NODE8 << blah.botList[7].botsVector.size() << L"\n";
	wss << W_NODE9 << blah.botList[8].botsVector.size() << L"\n";
	wss << W_NODE10 << blah.botList[9].botsVector.size() << L"\n";
	wss << W_NODE11 << blah.botList[10].botsVector.size() << L"\n";
	wss << W_NODE12 << blah.botList[11].botsVector.size() << L"\n";
	wss << W_NODE13 << blah.botList[12].botsVector.size() << L"\n";
	wss << W_NODE14 << blah.botList[13].botsVector.size() << L"\n";
	wss << W_NODE15 << blah.botList[14].botsVector.size() << L"\n";
	wss << W_NODE16 << blah.botList[15].botsVector.size() << L"\n";
	wss << W_NODE17 << blah.botList[16].botsVector.size() << L"\n";
	wss << W_NODE18 << blah.botList[17].botsVector.size() << L"\n";
	wss << W_NODE19 << blah.botList[18].botsVector.size() << L"\n";
	wss << W_NODE20 << blah.botList[19].botsVector.size() << L"\n";
	wss << W_NODE21 << blah.botList[20].botsVector.size() << L"\n";

	textToGenerate.append(wss.str());

}