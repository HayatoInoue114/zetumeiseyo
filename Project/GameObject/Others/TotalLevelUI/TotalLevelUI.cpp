#include "TotalLevelUI.h"



void TotalLevelUI::Initialize()
{
	totalLabelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "total_Lavel.png");

	labelTexSize_ = { 180.0f, 90.0f };

	totalLabelSprite_ = make_unique<Sprite>();
	totalLabelSprite_->Initialize(labelTexSize_);
	totalLabelSprite_->SetSpriteOrigin(SpriteOrigin::Center);

	totalLabelTransform_.Initialize();
	totalLabelTransform_.translate = {
		108.0f,
		96.0f + (5 * 96.0f),
		0.0f,
	};

	totalNumSprite_.resize(2);
	totalNumTransform_.resize(2);

	for (int i = 0; i < 2; ++i) {

		totalNumSprite_[i] = make_unique<Sprite>();
		totalNumSprite_[i]->Initialize({ 48.0f, 48.0f });
		totalNumSprite_[i]->SetSpriteOrigin(SpriteOrigin::Center);

		totalNumTransform_[i].Initialize();
		totalNumTransform_[i].translate = {
			128.0f + (i * 32.0f) + labelTexSize_.x - 48.0f,
			96.0f + (5 * 96.0f),
			0.0f,
		};
	}

	totalNum_ = SaveValue::GetIntValue("Total_Level");

	eachNum_[0] = (totalNum_ % 100) / 10;
	eachNum_[1] = (totalNum_ % 10) / 1;


	// レベル
	levelTexHD_ = TextureManager::LoadTexture("GameScene/UI", "Number.png");

	levelTexSize_ = { 48.0f, 48.0f };

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
}


void TotalLevelUI::Draw2DFront(Camera* camera)
{
	totalLabelTransform_.UpdateMatrix();
	totalLabelSprite_->Draw(totalLabelTexHD_, totalLabelTransform_, camera);
	for (int i = 0; i < 2; ++i) {
		totalNumTransform_[i].UpdateMatrix();
		totalNumSprite_[i]->SetSrc(src_[eachNum_[i]]);
		totalNumSprite_[i]->Draw(levelTexHD_, totalNumTransform_[i], camera);
	}
}
