#pragma once


#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\ai\Bot.h"



class UserData
{





public:

	AnimatedSprite *sprite = nullptr;
	Bot *bot = nullptr;
	bool isbot = false;

	UserData(){}
	~UserData(){}








	AnimatedSprite*			getSprite()			{ return sprite; }

	void setSprite(AnimatedSprite *initSprite)
	{
		sprite = initSprite;
	}


	Bot*					getBot()			{ return bot; }

	void setBot(Bot *initBot)
	{
		bot = initBot;
	}









};