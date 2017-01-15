#pragma once

#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\world\World.h"
#include <vector>
#include <array>
#include "sssf\gsm\sprite\QuadTreeNode.h"
#include <list>

class QuadTree
{

public:
	QuadTree(World *w);
	~QuadTree() {}

	std::array<QuadTreeNode, 21> botList;
	std::list<Bot*> allBotList;

	Game *game;

	int botCount;
	int width;
	int height;
	void				addBotToTree(Bot *botToAdd, World *w);
	list<Bot*>			removeFromTree(list<Bot*>  theBotList, World *w);
	list<Bot*>			updateTree(list<Bot*>  theBotList, World *w);




};