#pragma once
#include "Enemy/EnemyManager/EnemyManager.h"
#include "Player/Player.h"

class LevelManager
{
public:
	void Initialize(EnemyManager* enemyManager, Player* player, int waveCount);

	void Update(EnemyManager* enemyManager, Player* player, int waveCount);
	
	/*void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; }

	void SetPlayer(Player* player) { player_ = player; }*/
private:
	void DefaultLevel();
private:
	/*EnemyManager* enemyManager_;
	Player* player_;

	int enemyLevel_;
	int playerLevel_;*/
};

