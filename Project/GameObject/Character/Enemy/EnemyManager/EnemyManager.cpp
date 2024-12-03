#include "EnemyManager.h"
#include <cassert>
#include <fstream>
#include "GameScene/GameScene.h"

EnemyManager::EnemyManager() {
	waveCount_ = 0;
}

EnemyManager::~EnemyManager() {
	for (IEnemy* enemy : enemies_) {
		delete enemy;
	}
}

void EnemyManager::EnemySpawn(Player* player) {
	flame_++;
	waveCount_++;

	//for (int i = 0; i < MAX_ENEMY; i++) {
	//	float rad = 60;

	//	Scope scope = { rad / 2,rad };

	//	for (int j = 0; j < 4; j++) {
	//		rad_[j] = RandomGenerator::getRandom(scope);
	//		tmpPos_[j] = CalculateNextPosition(player->GetWorldPos(), rad_[j]);
	//		tmpPos_[j].y = -30.0f;
	//		// 位置の限界
	//		const float kMit = 100.0f;
	//		tmpPos_[j].x = max(tmpPos_[j].x, -kMit);
	//		tmpPos_[j].x = min(tmpPos_[j].x, +kMit);
	//		tmpPos_[j].z = max(tmpPos_[j].z, -kMit);
	//		tmpPos_[j].z = min(tmpPos_[j].z, +kMit);
	//	}
	//	////////////////////////////////////////////////  * Blast *
	//	Blast* newBlast = new Blast();
	//	newBlast->SetGameScene(gameScene);
	//	newBlast->Initialize(player, tmpPos_[3], level_);
	//	enemies_.push_back(newBlast);

	//	///////////////////////////////////////////////   * Fire *
	//	Fire* newFire = new Fire();
	//	newFire->SetGameScene(gameScene);
	//	newFire->Initialize(player, tmpPos_[0], level_);
	//	enemies_.push_back(newFire);


	//	////////////////////////////////////////////////  * Trace * 
	//	/*Trace* newTrace = new Trace();
	//	newTrace->SetGameScene(gameScene);
	//	newTrace->Initialize(player, tmpPos_[1], level_);
	//	enemies_.push_back(newTrace);*/


	//	////////////////////////////////////////////////  * Mortar *
	//	Mortar* newMortar = new Mortar();
	//	newMortar->SetGameScene(gameScene);
	//	newMortar->Initialize(player, tmpPos_[2], level_);
	//	enemies_.push_back(newMortar);

	//}
		////////////////////////////////////////////////  * Boss * 一体だけ
	if (waveCount_ == 6) {
		
	}
	Boss* newBoss = new Boss();
	newBoss->SetGameScene(gameScene);
	newBoss->Initialize(player, tmpPos_[3], level_);
	enemies_.push_back(newBoss);
		
	for (int i = 0; i < MAX_ENEMY; i++) {
		
	}
	
	//Load("EnemyManager\n");
}

void EnemyManager::TitleSpawn(Camera* camera)
{
	camera;
	////////////////////////////////////////////////  * Blast *
	Blast* newBlast = new Blast();
	newBlast->SetGameScene(gameScene);
	newBlast->Initialize({18.5f,0,0}, level_);
	enemies_.push_back(newBlast);


	//Scope max = { 0,3 };
	//int maxint = (int)RandomGenerator::getRandom(max);
	//for (int i = 0; i < maxint; i++) {
	//	float rad = 30;

	//	Scope scope = { rad / 2,rad };

	//	for (int i = 0; i < 4; i++) {
	//		rad_[i] = RandomGenerator::getRandom(scope);
	//		tmpPos_[i] = CalculateNextPosition(camera->GetWorldPos(), rad_[i]);
	//		tmpPos_[i].z += 100.0f;
	//		tmpPos_[i].y = -30.0f;
	//	}
	//	////////////////////////////////////////////////  * Blast *
	//	Blast* newBlast = new Blast();
	//	newBlast->SetGameScene(gameScene);
	//	newBlast->Initialize(tmpPos_[3], level_);
	//	enemies_.push_back(newBlast);
	//}
}

void EnemyManager::Initialize()
{

}

Vector3 EnemyManager::CalculateNextPosition(const Vector3& position, float radius) {
	// 乱数生成器の準備
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

	// ランダムな方向ベクトルを生成し、長さをスケーリングする
	float dx = dis(gen) * radius;
	float dy = dis(gen) * radius;

	// 予測移動先座標を計算する
	float nextX = position.x + dx;
	float nextZ = position.z + dy;

	return { nextX, 0,nextZ };
}

void EnemyManager::Update() {
	for (IEnemy* enemy : enemies_) {
		enemy->SpawnMove();
		if (enemy->isSpawn == true) {
			enemy->Update();
		}
	}

	enemies_.remove_if([](IEnemy* enemy) {
		if (!enemy->IsAlive()) {
			delete enemy;
			return true;
		}
		return false;
	});
}

void EnemyManager::TitleUpdate() {
	for (IEnemy* enemy : enemies_) {
		enemy->SpawnMove();
		if (enemy->isSpawn == true) {
			enemy->TitleMove();
		}
	}

	enemies_.remove_if([](IEnemy* enemy) {
		if (!enemy->IsAlive()) {
			delete enemy;
			return true;
		}
		return false;
		});
}

void EnemyManager::SelectUpdate() {
	for (IEnemy* enemy : enemies_) {
		enemy->SpawnMove();
		if (enemy->isSpawn == true) {
			enemy->SelectMove();
		}
	}

	enemies_.remove_if([](IEnemy* enemy) {
		if (!enemy->IsAlive()) {
			delete enemy;
			return true;
		}
		return false;
		});
}

void EnemyManager::Draw(Camera* camera) {
	for (IEnemy* enemy : enemies_) {
		enemy->Draw(camera);
	}
}

