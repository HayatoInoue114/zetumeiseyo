#include "DrawCost.h"



void DrawCost::Initialize(uint32_t cost)
{
	// ラベル
	lavelTexHD_ = TextureManager::LoadTexture("GameScene/Player", "Cost_Label.png");

	lavelSize_ = { 180.0f, 90.0f };

	lavelSprite_ = make_unique<Sprite>();
	lavelSprite_->Initialize(lavelSize_);

	lavelTransform_.Initialize();
	tTranslate_ = {
		580.0f,
		80.0f,
		0.0f,
	};

	eachTimes_[0] = (cost % 100) / 10;
	eachTimes_[1] = (cost % 10) / 1;


	// 数字
	numberTexHD_ = TextureManager::LoadTexture("GameScene/UI", "Number.png");

	numberSize_ = { 32.0f, 32.0f };

	numberSprite_.resize(2);
	for (size_t i = 0; i < numberSprite_.size(); ++i) {
		numberSprite_[i] = make_unique<Sprite>();
		numberSprite_[i]->Initialize(numberSize_);
	}

	numberTransform_.resize(2);
	tNumberTransform_.resize(2);
	for (size_t i = 0; i < numberTransform_.size(); ++i) {
		numberTransform_[i].Initialize();
		tNumberTransform_[i].Initialize();
		tNumberTransform_[i].translate = {
			tTranslate_.x + lavelSize_.x + (i * 56.0f),
			tTranslate_.y + (lavelSize_.y / 2.0f) - (numberSize_.y / 2.0f),
			0.0f,
		};
	}

	src_.resize(10);
	int column = 0;
	int line = 0;
	for (size_t i = 0; i < src_.size(); ++i) {

		src_[i].LeftUp = { 0.0f + (0.2f * column), 0.0f + (0.5f * line) };
		src_[i].LeftDown = { 0.0f + (0.2f * column), 0.5f + (0.5f * line) };
		src_[i].RightUp = { 0.2f + (0.2f * column), 0.0f + (0.5f * line) };
		src_[i].RightDown = { 0.2f + (0.2f * column), 0.5f + (0.5f * line) };

		column++;

		if (i == 4) {
			column = 0;
			line++;
		}
	}
}


void DrawCost::Draw(Camera* camera)
{
	lavelTransform_.translate = tTranslate_ + translate_;
	// ラベル
	lavelTransform_.UpdateMatrix();
	lavelSprite_->Draw(lavelTexHD_, lavelTransform_, camera);

	// 数字
	SetSrc();
	for (size_t i = 0; i < numberSprite_.size(); i++) {
		numberTransform_[i].translate = tNumberTransform_[i].translate + translate_;
		numberTransform_[i].UpdateMatrix();
		numberSprite_[i]->Draw(numberTexHD_, numberTransform_[i], camera);
	}
}



void DrawCost::SetSrc()
{
	numberSprite_[0]->SetSrc(src_[eachTimes_[0]]);
	numberSprite_[1]->SetSrc(src_[eachTimes_[1]]);
}
