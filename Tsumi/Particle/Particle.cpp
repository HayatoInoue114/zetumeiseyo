#include "Particle.h"



// 初期化処理
void Particle::Initialize(IParticleState* state, uint32_t instanceNum, uint32_t useTex) {

	// テクスチャの初期設定
	this->useTexture_ = useTex;

	// インスタンス数
	instanceNum_ = instanceNum;

	// ステートパターンの初期化処理
	this->state_ = state;
	state_->Initialize(this);
}


// 描画処理
void Particle::Draw(uint32_t texHD, ViewProjection view) {

	/*ImGui::Begin("Particle");
	ImGui::Text("ParticleSize = %d", particlePropes_.size());
	ImGui::End();*/

	state_->Draw(texHD, this, particlePropes_, view);
}


// 先頭から情報を取り出す
list<ParticleProperties> Particle::RetrieveFront()
{
	for (list<ParticleProperties>::iterator itr = particlePropes_.begin(); itr != particlePropes_.end();) {

		if ((*itr).isActive) {
			++itr;
		}
		else {
			particlePropes_.erase(itr);
			continue;
		}
	}

	list<ParticleProperties> propes = particlePropes_;
	particlePropes_.clear();
	return propes;
}


// リストの登録
void Particle::PushBackList(ParticleProperties prope) {

	particlePropes_.push_back(prope);
}


// パーティクルの生成器
ParticleProperties Particle::ParticleGenerators(Scope lifeTimeScope, ScopeVec3 posScope, ScopeVec3 velScope, ScopeVec4 colorScope)
{
	ParticleProperties particlePrope{};
	particlePrope.worldTransform.Initialize();
	particlePrope.worldTransform.scale = Vector3::one;
	particlePrope.worldTransform.rotate = Vector3::zero;
	particlePrope.worldTransform.translate = RandomGenerator::getRandom(posScope);
	particlePrope.color = RandomGenerator::getColorRandom(colorScope);
	particlePrope.velocity = RandomGenerator::getRandom(velScope) / 100.0f;
	particlePrope.uvTransform.scale = Vector3::one;
	particlePrope.uvTransform.rotate = Vector3::zero;
	particlePrope.uvTransform.translate = Vector3::zero;
	particlePrope.lifeTime = uint32_t(RandomGenerator::getRandom(lifeTimeScope) * 60.0f);
	particlePrope.currentTime = 0;
	particlePrope.isActive = true;

	return particlePrope;
}


// Emitterにによるパーティクル生成
void Particle::Emit(const Emitter& emitter, Scope lifeTimeScope, ScopeVec3 posScope, ScopeVec3 velScope, ScopeVec4 colorScope)
{
	// エミッターのカウント分パーティクルを生成する
	for (int count = 0; count < int(emitter.count); ++count) {
		ParticleProperties prope{};
		prope = ParticleGenerators(lifeTimeScope, posScope, velScope, colorScope);
		prope.worldTransform.translate += emitter.worldTransform.translate;
		PushBackList(prope);
	}
}

