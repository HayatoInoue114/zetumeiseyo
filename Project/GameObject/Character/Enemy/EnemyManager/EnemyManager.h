#pragma once
#include <sstream>
#include "Player/Player.h"
#include "GameObject.h"
#include "Enemy/Trace/Trace.h"
#include "Enemy/Blast/Blast.h"
#include "Enemy/Fire/Fire.h"
#include "Enemy/Mortar/Mortar.h"
#include "Enemy/Boss/Boss.h"
#include "Enemy/IEnemy/IEnemy.h"

class GameScene;
class IScene;

#define MAX_ENEMY 3

class EnemyManager
{
public:
	EnemyManager();

	~EnemyManager();

	void EnemySpawn(Player* player);

	void TitleSpawn(Camera* camera);

	void Initialize();

	void Update();

	void TitleUpdate();

	void SelectUpdate();

	void Draw(Camera* camera);

	std::list<IEnemy*>* GetEnemy() { return &enemies_; }

	void SetGameScene(IScene* scene) { this->gameScene = scene; };

	void SetLevel(int level) { level_ = level; }

private:
	Vector3 CalculateNextPosition(const Vector3& position, float radius);
private:
	// 敵発生コマンド
	std::stringstream popCommands;

	int flame_;

	int traceMax_;

	int blastMax_;

	float rad_[4];

	Vector3 tmpPos_[4];

	Vector3 tmpRotate_[4];

	std::list<IEnemy*> enemies_;

	IScene* gameScene = nullptr;

	int level_;
	int waveCount_;

	//IEnemy* enemyArr_[4];

	//std::vector<std::unique_ptr<Trace>> trace_;
	//std::vector<std::unique_ptr<Blast>> blast_;
};

