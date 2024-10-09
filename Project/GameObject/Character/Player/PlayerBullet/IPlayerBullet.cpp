#include "IPlayerBullet.h"



// エネミーとの衝突判定
void IPlayerBullet::onCollisionWithEnemy(IEnemy* enemy)
{
	enemy;
	isDead_ = true;

	for (int i = 0; i < 3; ++i) {
		//collisionParticle_->AddParticle(collisionParticleTexHD1_, bulletWt_.translate);
		//collisionParticle_->AddParticle(collisionParticleTexHD2_, bulletWt_.translate);
	}
}

void IPlayerBullet::onCollisionWithEnemyBullet(EnemyBullet* enemy)
{
	int a = enemy->GetHP();
	int b = GetHP();
	SetHP(GetHP() - enemy->GetHP());

	if (GetHP() <= 0) {
		isDead_ = true;
		for (int i = 0; i < 5; ++i) {
		}
	}
	enemy;
	//isDead_ = true;
}

// 寿命の処理
void IPlayerBullet::RemoveAfterLifetime()
{
	// 寿命の時間をデクリメント
	lifeTimer_--;

	// 0以下で死亡フラグを立てる
	if (lifeTimer_ <= 0) {
		isDead_ = true;
	}
}
