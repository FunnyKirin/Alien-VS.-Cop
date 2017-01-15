#pragma once

#include <Box2D\Box2D.h>
#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameState.h"


class AVCContactListener : public b2ContactListener
{
private:



	Game *game;

public:
	AVCContactListener(
		Game *game)		{
		this->game = game;

	}
	~AVCContactListener()		{}



	void		BeginContact(b2Contact* contact);





};
