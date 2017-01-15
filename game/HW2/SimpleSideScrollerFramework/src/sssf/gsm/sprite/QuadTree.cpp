#include <vector>
#include "sssf\gsm\sprite\QuadTree.h"
#include "sssf\gsm\sprite\QuadTreeNode.h"
#include "sssf\gsm\world\World.h"
#include <array>
#include "sssf\game\Game.h"

QuadTree::QuadTree(World *w)
{
	w->setWorldWidth(3200);
	w->setWorldHeight(3200);
	botList[0] = QuadTreeNode(0, 0, w->getWorldWidth(), w->getWorldHeight());
	botList[1] = QuadTreeNode(0, 0, w->getWorldWidth() / 2, w->getWorldHeight() / 2);
	botList[2] = QuadTreeNode(w->getWorldWidth() / 2, 0, w->getWorldWidth() / 2, w->getWorldHeight() / 2);
	botList[3] = QuadTreeNode(0, w->getWorldHeight() / 2, w->getWorldWidth() / 2, w->getWorldHeight() / 2);
	botList[4] = QuadTreeNode(0, 0, w->getWorldWidth() / 2, w->getWorldHeight() / 2);
	botList[5] = QuadTreeNode(0, 0, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[6] = QuadTreeNode(w->getWorldWidth() / 4, 0, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[7] = QuadTreeNode(0, w->getWorldHeight() / 4, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[8] = QuadTreeNode(w->getWorldWidth() / 4, w->getWorldHeight() / 4, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[9] = QuadTreeNode(w->getWorldWidth() / 2, 0, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[10] = QuadTreeNode((w->getWorldWidth() * 3) / 4, 0, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[11] = QuadTreeNode(w->getWorldWidth() / 2, w->getWorldHeight() / 4, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[12] = QuadTreeNode((w->getWorldWidth() * 3) / 4, w->getWorldHeight() / 4, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[13] = QuadTreeNode(0, w->getWorldHeight() / 2, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[14] = QuadTreeNode(w->getWorldWidth() / 4, w->getWorldHeight() / 2, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[15] = QuadTreeNode(0, (w->getWorldHeight() * 3) / 4, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[16] = QuadTreeNode(w->getWorldWidth() / 4, (w->getWorldHeight() * 3) / 4, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[17] = QuadTreeNode(w->getWorldWidth() / 2, w->getWorldHeight() / 2, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[18] = QuadTreeNode((w->getWorldWidth() * 3) / 4, w->getWorldHeight() / 2, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[19] = QuadTreeNode(w->getWorldWidth() / 2, (w->getWorldHeight() * 3) / 4, w->getWorldWidth() / 4, w->getWorldHeight() / 4);
	botList[20] = QuadTreeNode((w->getWorldWidth() * 3) / 4, (w->getWorldHeight() * 3) / 4, w->getWorldWidth() / 4, w->getWorldHeight() / 4);

}

void QuadTree::addBotToTree(Bot *botToAdd, World *w)
{
	int worldWidth = w->getWorldWidth();
	int worldHeight = w->getWorldHeight();

	int botXpos = botToAdd->getPhysicalProperties()->getX();
	int botYpos = botToAdd->getPhysicalProperties()->getY();
	int xWidth = 128;
	int yHeight = 128;

	/*for (int i = 1; i <= 4; i++)
	{
		// which parent is it in
		if ((botXpos + xWidth) >= botList[i].ix && (botYpos + yHeight) >= botList[i].iy)
		{
			if ((botXpos + xWidth) <= (botList[i].ix + botList[i].iwidth) && (botYpos + yHeight) <= (botList[i].iy + botList[i].iheight))
			{
				for (int j = 1; j <= 4; j++)
				{
					//which  leaf?
					if ((botXpos + xWidth) >= (botList[4 * i + j].ix) && (botYpos + yHeight) >= (botList[4 * i + j].iy))
					{
						if ((botXpos + xWidth) <= (botList[4 * i + j].ix + botList[4 * i + j].iwidth) && (botYpos + yHeight) <= (botList[4 * i + j].iy + botList[4 * i + j].iheight))
						{
							botList[4 * i + j].botsVector.push_back(botToAdd);
							allBotList.push_back(botToAdd);
						}
					}

				}
			}
		}
	}*/

	for (int i = 20; i >= 0; i--)
	{
		if (botXpos >= botList[i].ix && botYpos >= botList[i].iy && (botXpos + xWidth) < (botList[i].ix + botList[i].iwidth) && (botYpos + yHeight) < (botList[i].iy + botList[i].iheight))
		{
			botList[i].botsVector.push_back(botToAdd);
			allBotList.push_back(botToAdd);
			break;
		}

	}




}

list<Bot*> QuadTree::removeFromTree(list<Bot*>  theBotList, World *w)
{
	int count = 0;
	list<Bot*>::iterator botIterator;
	botIterator = theBotList.begin();
	while (botIterator != theBotList.end())
	{
		Bot *bot = (*botIterator);
		theBotList.pop_back();
		botIterator++;
		count++;
		if (count == 9)
		{
			break;
		}
	}

	return updateTree(theBotList, w);
}

list<Bot*> QuadTree::updateTree(list<Bot*>  theBotList, World *w)
{
	allBotList.clear();
	for (int i = 0; i < botList.size(); i++)
	{
		botList[i].botsVector.clear();
	}

	list<Bot*>::iterator botIterator;
	botIterator = theBotList.begin();
	while (botIterator != theBotList.end())
	{
		Bot *bot = (*botIterator);
		addBotToTree(bot, w);
		botIterator++;
	}

	
	return allBotList;
}

