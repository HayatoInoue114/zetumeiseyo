#include "NormalParticle.h"

void NormalParticle::Initialize()
{
	name_ = "unti";
	
	wt_.Initialize();
	uv_.Initialize();

	wt_.translate = Vector3::zero;

	perSecond_ = 1;

	direction_ = Vector3::zero;

	velocity_ = Vector3::zero;

	acceleration_ = Vector3::zero;

	minLifetime_ = 3.0f;
	maxLifetime_ = 60.0f;
	lifeTime_ = 0;

	color_ = Vector4::one;

	isActive_ = true;
}

void NormalParticle::Update()
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

void NormalParticle::AddParticle(uint32_t texture, Vector3 pos)
{
	pos;
	std::unique_ptr<NormalParticle> newParticle = IParticle::Create<NormalParticle>();
	ParticleManager::GetInstance()->AddParticle(name_, std::move(newParticle));
	ParticleManager::GetInstance()->SetTexture(name_, texture);
}


