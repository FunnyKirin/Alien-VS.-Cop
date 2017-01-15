#pragma once

#include <vector>
#include "sssf\gsm\ai\Bot.h"

class QuadTreeNode
{

public:

	int ix;
	int iy;
	int iwidth;
	int iheight;

	QuadTreeNode(int x, int y, int width, int height);
	QuadTreeNode() {}
	~QuadTreeNode() {}

	std::vector <Bot*> botsVector;

	



};