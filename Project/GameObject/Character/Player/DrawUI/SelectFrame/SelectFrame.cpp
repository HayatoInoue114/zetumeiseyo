#include "SelectFrame.h"



void SelectFrame::Initialize()
{
	texHD_ = TextureManager::LoadTexture("GameScene/Player", "selectFrame.png");

	sprite_ = make_unique<Sprite>();
	sprite_->Initialize({ 140.0f, 96.0f });
	sprite_->SetSpriteOrigin(SpriteOrigin::Center);
	sprite_->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });

	transform_.Initialize();


	AtexHD_ = TextureManager::LoadTexture("GameScene/Player", "frameA.png");

	Asprite_ = make_unique<Sprite>();
	Asprite_->Initialize({ 140.0f, 96.0f });
	Asprite_->SetSpriteOrigin(SpriteOrigin::Center);
	Asprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	Atransform_.Initialize();
}


void SelectFrame::Draw2DFront(uint32_t page, uint32_t num, Camera* camera)
{
	if (page == 0) {
		
		tTranslate_ = {
			levelVectors_[num].x,
			levelVectors_[num].y,
			0.0f,
		};
		tTranslate_ = {
			levelVectors_[num].x,
			levelVectors_[num].y,
			0.0f,
		};
		transform_.translate = tTranslate_ + translate_;
		Atransform_.translate = tTranslate_ + translate_;
	}
	else if (page == 1) {
		tTranslate_ = {
			bulletVectors_[num].x,
			bulletVectors_[num].y,
			0.0f,
		};
		tTranslate_ = {
			bulletVectors_[num].x,
			bulletVectors_[num].y,
			0.0f,
		};
		transform_.translate = transform_.translate + translate_;
		Atransform_.translate = Atransform_.translate + translate_;
	}
	
	transform_.UpdateMatrix();
	sprite_->Draw(texHD_, transform_, camera);
	Atransform_.UpdateMatrix();
	Asprite_->Draw(AtexHD_, Atransform_, camera);
}
