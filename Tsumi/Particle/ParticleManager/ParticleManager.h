#pragma once

#include "ParticleRenderer.h"
#include "ParticleArray.h"

/* ParticleManagerクラス */
class ParticleManager {

private: // シングルトンパターン

	// コンストラクタ、デストラクタ
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	const ParticleManager& operator=(const ParticleManager&) = delete;

public: // メンバ関数

	// インスタンスの取得
	static ParticleManager* GetInstance() {
		static ParticleManager instance;
		return &instance;
	}

	// 初期化処理、更新処理、描画処理
	void Initialize();
	void Update();
	void Draw(Camera* camera);

	// 粒子の追加
	void AddParticle(string particleName, unique_ptr<IParticle> particle);

	// クリア
	static void Clear();


#pragma region Get

	// 粒子配列の取得
	ParticleArray* GetParticleArray(string particleName) { return particleMap_[particleName].get(); }

#pragma endregion

#pragma region Set

	// テクスチャの設定
	void SetTexture(const string particleName, uint32_t setTexture) { this->particleMap_[particleName]->SetUseTexture(setTexture); }

	// 粒子配列の設定
	//void SetParticleArray(const string particleName){this->particleMap_[particleName] }

#pragma endregion 


private: // メンバ変数

	// 描画クラス
	unique_ptr<ParticleRenderer> renderer_ = nullptr;

	// 粒子配列
	map<string, unique_ptr<ParticleArray>> particleMap_;
};

