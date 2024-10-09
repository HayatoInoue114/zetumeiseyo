#include "ParticleManager.h"



// 初期化処理
void ParticleManager::Initialize()
{
	// 描画クラスの初期化
	renderer_ = make_unique<ParticleRenderer>();
	renderer_->Initialize(2048u);
}


// 更新処理
void ParticleManager::Update()
{
	for (auto& [name, particleArray] : particleMap_) {
		particleArray->Update();
	}
}


// 描画処理
void ParticleManager::Draw(Camera* camera)
{
	for (auto& [name, particleArray] : particleMap_) {

		// パーティクルの配列の取得
		const auto& particles = particleArray->GetParticleArray();

		// 使うテクスチャの取得
		uint32_t useTexture = particleArray->GetUseTesture();

		// サイズが0なら飛ばす
		if (particles.size() == 0u) { continue; }

		// 描画処理
		renderer_->Draw(particles, useTexture, camera);
	}
}


// パーティクルの追加
void ParticleManager::AddParticle(string particleName, unique_ptr<IParticle> particle)
{
	// マップがなかったら作成する
	decltype(particleMap_)::iterator particleItr = particleMap_.find(particleName);
	
	if (particleItr == particleMap_.end()) {
		particleMap_[particleName] = make_unique<ParticleArray>();
	}

	// パーティクルを追加
	particleMap_[particleName]->push_back(move(particle));
}


// クリア
void ParticleManager::Clear()
{
	ParticleManager::GetInstance()->particleMap_.clear();
}
