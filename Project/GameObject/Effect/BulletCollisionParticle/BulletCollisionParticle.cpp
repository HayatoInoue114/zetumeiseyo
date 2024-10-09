#include "BulletCollisionParticle.h"

void BulletCollisionParticle::Initialize()
{
	name_ = "BulletCollisionParticle";

	wt_.Initialize();
	uv_.Initialize();

	wt_.scale = { 2.0f, 2.0f, 2.0f };
	wt_.translate = Vector3::zero;

	perSecond_ = 1;

	direction_ = Vector3::zero;

	Scope scope = { -0.5f, 0.5f };
	velocity_ = {
		RandomGenerator::getRandom(scope),
		RandomGenerator::getRandom(scope),
		0.0f,
	};

	acceleration_ = { 0.005f, 0.005f, 0.0f };

	minLifetime_ = 3.0f;
	maxLifetime_ = 20.0f;
	lifeTime_ = 0;

	color_ = { 1.0f, 1.0f, 0.0f, 1.0f };

	isActive_ = true;
}

void BulletCollisionParticle::Update()
{
	wt_.UpdateMatrix();
	uv_.UpdateMatrix();

	lifeTime_++;

	if (lifeTime_ >= maxLifetime_) {
		isActive_ = false;
	}

	wt_.translate += velocity_;
	velocity_ -= acceleration_;

	wt_.rotate.z += 0.2f;

	color_.w =
		1.0f + (0.0f - 1.0f) * Ease::InCirc(lifeTime_ / maxLifetime_);

	if (color_.w <= 0.0f) {
		isActive_ = false;
	}
}

void BulletCollisionParticle::AddParticle(uint32_t texture, Vector3 pos)
{
	std::unique_ptr<BulletCollisionParticle> newParticle = IParticle::Create<BulletCollisionParticle>();
	newParticle->SetTranslate(pos);
	ParticleManager::GetInstance()->AddParticle(name_, std::move(newParticle));
	ParticleManager::GetInstance()->SetTexture(name_, texture);
}
