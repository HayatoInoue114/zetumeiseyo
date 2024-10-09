#include "LevelManager.h"

void LevelManager::Initialize(EnemyManager* enemyManager, Player* player,int waveCount)
{
	player;
	enemyManager->SetLevel(waveCount);
}

void LevelManager::Update(EnemyManager* enemyManager, Player* player, int waveCount)
{
	if (player->GetTotalLevel() <= 3) {
		enemyManager->SetLevel(waveCount);
	}
	else if (player->GetTotalLevel() <= 6) {
		enemyManager->SetLevel(waveCount + 1);
	}
	else if (player->GetTotalLevel() <= 12) {
		enemyManager->SetLevel(waveCount + 2);
	}
	else if (player->GetTotalLevel() <= 20) {
		enemyManager->SetLevel(waveCount + 3);
	}
	else if (player->GetTotalLevel() <= 30) {
		enemyManager->SetLevel(waveCount + 4);
	}
}

void LevelManager::DefaultLevel()
{
}
