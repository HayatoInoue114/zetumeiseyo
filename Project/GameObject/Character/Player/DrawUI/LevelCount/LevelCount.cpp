#include "LevelCount.h"



void LevelCount::Initialize(int prope, Vector2 pos)
{
	// ラベル
	if (prope == LevelPrope::HP) {
		lavelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "HP_Lavel.png");
	}
	else if (prope == LevelPrope::Vel) {
		lavelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "Vel_Lavel.png");
	}
	else if (prope == LevelPrope::Damage) {
		lavelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "Damage_Lavel.png");
	}
	else if (prope == LevelPrope::Rate) {
		lavelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "Rate_Lavel.png");
	}
	else if (prope == LevelPrope::Dash) {
		lavelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "Dash_Lavel.png");
	}

	lavelSpriteSize_ = { 152.0f, 104.0f };

	lavelSprite_ = make_unique<Sprite>();
	lavelSprite_->Initialize(lavelSpriteSize_);
	lavelSprite_->SetSpriteOrigin(SpriteOrigin::Center);

	lavelTransform_.Initialize();
	lavelTransform_.translate = { pos.x, pos.y,	0.0f, };


	// レベルカウント
	size_ = 5;

	countTexHD_ = TextureManager::LoadTexture("GameScene/Player", "LevelCount.png");

	countSprite_.resize(size_);
	countTransform_.resize(size_);

	for (size_t i = 0; i < size_; ++i) {
		countSprite_[i] = make_unique<Sprite>();
		countSprite_[i]->Initialize({ 36.0f, 36.0f });
		countSprite_[i]->SetSpriteOrigin(SpriteOrigin::Center);

		countTransform_[i].Initialize();
		countTransform_[i].translate = {
			lavelTransform_.translate.x + lavelSpriteSize_.x + (i * 72.0f),
			lavelTransform_.translate.y,
			0.0f,
		};
	}
}


void LevelCount::Draw2DFront(uint32_t num, Camera* camera)
{
	// ラベル
	lavelTransform_.UpdateMatrix();
	lavelSprite_->Draw(lavelTexHD_, lavelTransform_, camera);
	
	// レベルカウント
	for (size_t i = 0; i < num; ++i) {
		countSprite_[i]->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	}
	for (size_t i = 0; i < size_; ++i) {

		countTransform_[i].UpdateMatrix();

		if (i >= 5) { return; }
		countSprite_[i]->Draw(countTexHD_, countTransform_[i], camera);
	}
}
