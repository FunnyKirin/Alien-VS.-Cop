/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	BugsKeyEventHandler.h

	This is a custom key event handler. Each game can choose
	to use different keyboard keys and respond differently to when
	they are pressed. This class will handle key presses for
	this particular game following the KeyEventHandler pattern.
*/

#pragma once
#include "bugs_VS\stdafx.h"
#include "sssf\input\KeyEventHandler.h"
#include <Box2D/Box2D.h>

class Game;

class BugsKeyEventHandler : public KeyEventHandler
{
public:
	BugsKeyEventHandler()		{}
	~BugsKeyEventHandler()		{}
	void handleKeyEvents(Game *game);
	void Shoot(Game *game, double x, double y, double dX, double dY);

	const short CATEGORY_PLAYER = 0x0001;  // 0000000000000001 in binary
	const short CATEGORY_MALIEN = 0x0002; // 0000000000000010 in binary
	const short CATEGORY_RALIEN = 0x0004;
	const short CATEGORY_PLAYERBULLET = 0x0008;
	const short CATEGORY_ALIENBULLET = 0x0010;
	const short CATEGORY_HLASER = 0x0020;
	const short CATEGORY_VLASER = 0x0040;
	const short CATEGORY_LASERRIFLE = 0x0080;

};