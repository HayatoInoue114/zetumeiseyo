#include "BlastParticle.h"

void BlastParticle::AddParticle(uint32_t useTex, Vector3 pos)
{
	std::unique_ptr<BlastParticle> newParticle = IParticle::Create<BlastParticle>();
	newParticle->SetTranslate(pos);
	ParticleManager::GetInstance()->AddParticle(name_, std::move(newParticle));
	ParticleManager::GetInstance()->SetTexture(name_, useTex);
}

void BlastParticle::Initialize()
{
	name_ = "BlastParticle";

	wt_.Initialize();
	uv_.Initialize();

	wt_.translate = Vector3::zero;

	perSecond_ = 1;

	direction_ = Vector3::zero;

	Scope scope = { -0.4f, 0.4f };
	velocity_ = {
		RandomGenerator::getRandom(scope),
		RandomGenerator::getRandom(scope),
		RandomGenerator::getRandom(scope),
	};

	acceleration_ = { 0.0f, 0.03f, 0.0f };

	minLifetime_ = 3.0f;
	maxLifetime_ = 30.0f;
	lifeTime_ = 0;

	color_ = Vector4::one;

	isActive_ = true;
}

void BlastParticle::Update()
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
