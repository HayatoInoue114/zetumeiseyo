#pragma once
#include "../IEnemy/IEnemy.h"
#include "../MathFunction/MathFunction.h"
#include "../Effect/BulletHitParticle/BulletHitParticle.h"

class Fire : public IEnemy, public OBBCollider
{
public:
	void Initialize(Player* player, Vector3 position, int level);

	void ParameterInitialize();

	void Update();

	void Draw(Camera* camera);

	void Shoot();

	void Chase();

#pragma region Collider 衝突判定

	// 衝突判定
	void OnCollision(uint32_t id) override;

	// コライダーのゲッター
	Vector3 GetOBBWorldPos() override { return worldTransform_.GetWorldPos(); }
	Vector3 GetSize() override { return this->size_; }
	Vector3 GetRotate() override { return this->worldTransform_.rotate; }

	OBBCollider* GetOBBCollider() override { return this; }

	void OnCollisionWithPlayer(Player* player) override;
	void OnCollisionWithPlayerBullet(IPlayerBullet* bullet) override;
#pragma endregion 


private:
	// Colliderのビットのセッティング
	void SettingColliderBit();

	// OBBColliderのセッティング
	void SettingCollider();

	void FeedMove();

	float ToPlayerDis();

	

private:
	//射撃のクールタイム
	const int bulletCoolTime = 120;
	int nowCT = 0;

	Vector3 size_{};

	bool isMove = false;
	bool isFoundOnce = false;
	Vector3 movePoint{};

	float chaseIntensity_{};

	
};

