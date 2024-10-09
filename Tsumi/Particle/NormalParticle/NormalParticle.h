#pragma once
#include "IParticle.h"
#include "ParticleManager.h"


/* NormalParticleクラス */
class NormalParticle : public IParticle {

public: // メンバ関数

	// コンストラクタ、デストラクタ
	NormalParticle() {};
	~NormalParticle() {};

	// 初期化処理、更新処理
	void Initialize() override;
	void Update() override;

	void AddParticle(uint32_t texture, Vector3 pos) override;
	
private:

};

