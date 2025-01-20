#include "LevelUp.h"

void LevelUp::LevelUpFunc(Player *player, GameTimeCount* gameTimeCount)
{
	if (player->GetEnergy() >= levelUpEnergy_) {
		gameTimeCount->SetIsTimeUp(true);
	}
}
