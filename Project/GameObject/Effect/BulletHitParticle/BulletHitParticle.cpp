#include "BulletHitParticle.h"

void BulletHitParticle::Initialize()
{
	name_ = "BulletHitParticle";

	wt_.Initialize();
	uv_.Initialize();

	wt_.translate = Vector3::zero;

	perSecond_ = 1;

	direction_ = Vector3::zero;

	Scope scope = { -0.2f, 0.2f };
	velocity_ = {
		RandomGenerator::getRandom(scope),
		RandomGenerator::getRandom(scope),
		RandomGenerator::getRandom(scope),
	};

	acceleration_ = { 0.0f, 0.03f, 0.0f };

	minLifetime_ = 3.0f;
	maxLifetime_ = 30.0f;
	lifeTime_ = 0;

	color_ = { 1.0f, 0.0f, 0.0f, 1.0f };

	isActive_ = true;
}

void BulletHitParticle::Update()
{
	wt_.UpdateMatrix();
	uv_.UpdateMatrix();

	lifeTime_++;

	if (lifeTime_ >= maxLifetime_) {
		isActive_ = false;
	}

	wt_.translate += velocity_;

	color_.w =
		1.0f + (0.0f - 1.0f) * Ease::InCirc(lifeTime_ / maxLifetime_);

	if (color_.w <= 0.0f) {
		isActive_ = false;
	}
}

void BulletHitParticle::AddParticle(uint32_t texture, Vector3 pos)
{
	std::unique_ptr<BulletHitParticle> newParticle = IParticle::Create<BulletHitParticle>();
	newParticle->SetTranslate(pos);
	newParticle->color_ = { 1,0,0,1 };
	ParticleManager::GetInstance()->AddParticle(name_, std::move(newParticle));
	ParticleManager::GetInstance()->SetTexture(name_, texture);
}

