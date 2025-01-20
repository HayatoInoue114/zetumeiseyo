#pragma once
#include "Player/Player.h"
#include "GameTimeCount/GameTimeCount.h"

class LevelUp
{
public:
	void LevelUpFunc(Player *player,GameTimeCount *GameTimeCount);
private:
	int levelUpEnergy_ = 5;
};

