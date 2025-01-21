#pragma once
#include "Player/Player.h"
#include "GameTimeCount/GameTimeCount.h"

class LevelUp
{
public:
	void LevelUpFunc(Player *player,GameTimeCount *GameTimeCount);
private:
	//レベルアップ画面に移行する必要エネルギー数(5)
	int levelUpEnergy_ = 5;
};

