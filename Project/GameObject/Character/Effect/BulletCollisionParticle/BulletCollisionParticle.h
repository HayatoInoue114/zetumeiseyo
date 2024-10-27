#pragma once
#include "IParticle.h"
#include "ParticleManager.h"

/* BulletCollisionParticleクラス */
class BulletCollisionParticle : public IParticle {

public:

	// コンストラクタ、デストラクタ
	BulletCollisionParticle() {};
	~BulletCollisionParticle() {};

	// 初期化処理、更新処理
	void Initialize() override;
	void Update() override;

	void AddParticle(uint32_t texture, Vector3 pos) override;

private:

};

