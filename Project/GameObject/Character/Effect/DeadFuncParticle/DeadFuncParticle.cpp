#include "DeadFuncParticle.h"

void DeadFuncParticle::Initialize()
{
	name_ = "DeadFuncParticle";


	wt_.Initialize();
	uv_.Initialize();

	Scope scope = { 0.5, 2.0f };
	wt_.scale = {
		RandomGenerator::getRandom(scope),
		RandomGenerator::getRandom(scope),
		0.0f,
	};

	scope = { 1.0f, 5.0f };
	wt_.rotate = {
		RandomGenerator::getRandom(scope),
		RandomGenerator::getRandom(scope),
		0.0f,
	};

	wt_.translate = Vector3::zero;

	perSecond_ = 1;

	direction_ = Vector3::zero;

	scope = { -0.07f, 0.07f };
	velocity_ = { 
		RandomGenerator::getRandom(scope),
		0.5f, 
		RandomGenerator::getRandom(scope),
	};

	acceleration_ = { 0.0f, 0.01f, 0.0f };

	minLifetime_ = 3.0f;
	maxLifetime_ = 120.0f;
	lifeTime_ = 0;

	color_ = { 1.0f, 0.0f, 0.0f, 1.0f };

	isActive_ = true;
}

void DeadFuncParticle::Update()
{
	wt_.UpdateMatrix();
	uv_.UpdateMatrix();

	lifeTime_++;

	if (lifeTime_ >= maxLifetime_) {
		isActive_ = false;
	}

	wt_.translate += velocity_;
	velocity_ -= acceleration_;

	color_.w =
		1.0f + (0.0f - 1.0f) * Ease::InCirc(lifeTime_ / maxLifetime_);

	if (color_.w <= 0.0f) {
		isActive_ = false;
	}
}

void DeadFuncParticle::AddParticle(uint32_t texture, Vector3 pos)
{
	std::unique_ptr<DeadFuncParticle> newParticle = IParticle::Create<DeadFuncParticle>();
	newParticle->SetTranslate(pos);
	ParticleManager::GetInstance()->AddParticle(name_, std::move(newParticle));
	ParticleManager::GetInstance()->SetTexture(name_, texture);
}

