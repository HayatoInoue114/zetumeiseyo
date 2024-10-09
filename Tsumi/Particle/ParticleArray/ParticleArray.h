#pragma once

#include "MyMath.h"
#include "IParticle.h"
#include "ParticleRenderer.h"
#include "Camera.h"
#include "TextureManager.h"

/* ParticleArrayクラス */
class ParticleArray {

public:

	// コンストラクタ、デストラクタ
	ParticleArray() {};
	~ParticleArray() {};

	// 更新処理
	void Update();

	// 粒子配列に粒子追加
	void push_back(unique_ptr<IParticle> addParticle);

#pragma region Get

	// 粒子配列の取得
	const auto& GetParticleArray() { return this->particles_; }

	// 描画に使うテクスチャの取得
	uint32_t GetUseTesture() const { return this->useTexture_; }

#pragma endregion 

#pragma region Set

	// 描画に使うテクスチャの設定
	void SetUseTexture(uint32_t setTexture) { this->useTexture_ = setTexture; }

#pragma endregion 


private:

	// 粒子配列
	list<unique_ptr<IParticle>> particles_;

	// 使用するテクスチャ
	uint32_t useTexture_ = 0;
};

