#include "ResultUI.h"



void ResultUI::Initialize(int label, int level, Vector2 pos)
{
	// ラベル
	if (label == LevelPrope::HP) {
		labelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "HP_Lavel.png");
	}
	else if (label == LevelPrope::Vel) {
		labelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "Vel_Lavel.png");
	}
	else if (label == LevelPrope::Damage) {
		labelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "Damage_Lavel.png");
	}
	else if (label == LevelPrope::Rate) {
		labelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "Rate_Lavel.png");
	}
	else if (label == LevelPrope::Dash) {
		labelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "Dash_Lavel.png");
	}

	labelTexSize_ = { 180.0f, 90.0f };

	labelSprite_ = make_unique<Sprite>();
	labelSprite_->Initialize(labelTexSize_);
	labelSprite_->SetSpriteOrigin(SpriteOrigin::Center);

	labelTransform_.Initialize();
	labelTransform_.translate = { pos.x, pos.y,0.0f };


	// レベル
	levelTexHD_ = TextureManager::LoadTexture("GameScene/UI", "Number.png");

	levelTexSize_ = { 48.0f, 48.0f };

	levelSprite_ = make_unique<Sprite>();
	levelSprite_->Initialize(levelTexSize_);
	levelSprite_->SetSpriteOrigin(SpriteOrigin::Center);

	levelTransform_.Initialize();
	levelTransform_.translate = {
		labelTransform_.translate.x + labelTexSize_.x - levelTexSize_.x,
		labelTransform_.translate.y,
		0.0f,
	};

	src_.resize(10);
	int column = 0;
	int line = 0;
	for (int Index = 0; Index < src_.size(); Index++) {

		// 上下左右の値を求める
		src_[Index].LeftUp = { 0.0f + (0.2f * column), 0.0f + (0.5f * line) };
		src_[Index].LeftDown = { 0.0f + (0.2f * column), 0.5f + (0.5f * line) };
		src_[Index].RightUp = { 0.2f + (0.2f * column), 0.0f + (0.5f * line) };
		src_[Index].RightDown = { 0.2f + (0.2f * column), 0.5f + (0.5f * line) };

		// 列を足す
		column++;

		// ５回ループしたら
		if (Index == 4) {
			// 列を０にして行に１足す
			column = 0;
			line++;
		}
	}

	levelNum_ = level;
}


void ResultUI::Draw2DFront(Camera* camera)
{
	// ラベル
	labelTransform_.UpdateMatrix();
	labelSprite_->Draw(labelTexHD_, labelTransform_, camera);

	// レベル
	levelTransform_.UpdateMatrix();
	levelSprite_->SetSrc(src_[levelNum_]);
	levelSprite_->Draw(levelTexHD_, levelTransform_, camera);
}

