#include "BackArrow.h"



void BackArrow::Initialize()
{
	size_ = 2;

	texHD_.resize(size_);
	sprite_.resize(size_);
	transform_.resize(size_);
	tTransform_.resize(size_);

	texHD_[0] = TextureManager::LoadTexture("GameScene/Player", "backArrwo_toRight.png");
	texHD_[1] = TextureManager::LoadTexture("GameScene/Player", "backArrwo_toLeft.png");
	
	for (size_t i = 0; i < size_; ++i) {

		sprite_[i] = make_unique<Sprite>();
		sprite_[i]->Initialize({ 32.0f, 32.0f });

		transform_[i].Initialize();
		tTransform_[i].Initialize();
		tTransform_[i].translate = {
			1056.0f - (i * 868.0f),
			630.0f - 48.0f,
			0.0f,
		};
	}


	labelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "NextWave_Label.png");

	labelSprite_ = make_unique<Sprite>();
	labelSprite_->Initialize({ 240.0f, 120.0f });

	labelWt_.Initialize();
	tTranslate_ = {
		860.0f,
		520.0f,
		0.0f,
	};
	
}


void BackArrow::Draw2DFront(int nowPage, Camera* camera)
{
	transform_[nowPage].translate = tTransform_[nowPage].translate + translate_;
	sprite_[nowPage]->Draw(texHD_[nowPage], transform_[nowPage], camera);
	transform_[nowPage].UpdateMatrix();

	if (nowPage == 1) {
		labelWt_.UpdateMatrix();
		labelWt_.translate = tTranslate_ + translate_;
		labelSprite_->Draw(labelTexHD_, labelWt_, camera);
	}
}
