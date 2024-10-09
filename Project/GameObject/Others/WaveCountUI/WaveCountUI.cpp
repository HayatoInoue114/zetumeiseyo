#include "WaveCountUI.h"



void WaveCountUI::Initialize(int nowCount, int waveMax)
{
	spriteSize_ = { 40.0f, 40.0f };

	// ウェーブ
	waveNumSprite_.resize(2);
	waveNumTransform_.resize(2);

	for (int i = 0; i < 2; ++i) {

		waveNumSprite_[i] = make_unique<Sprite>();
		waveNumSprite_[i]->Initialize(spriteSize_);
		waveNumSprite_[i]->SetSpriteOrigin(SpriteOrigin::Center);

		waveNumTransform_[i].Initialize();
		waveNumTransform_[i].translate = {
			i == 0 ? WinApp::kHalfWindowWidth - spriteSize_.x : WinApp::kHalfWindowWidth + spriteSize_.x,
			104.0f,
			0.0f,
		};
	}
	eachNum_[0] = (nowCount % 10) / 1;
	eachNum_[1] = (waveMax % 10) / 1;


	// スラッシュ
	slashTexHD_ = TextureManager::LoadTexture("GameScene/UI", "slash.png");
	slashSprite_ = make_unique<Sprite>();
	slashSprite_->Initialize(spriteSize_);
	slashSprite_->SetSpriteOrigin(SpriteOrigin::Center);
	slashTransform_.Initialize();
	slashTransform_.translate = {
		WinApp::kHalfWindowWidth,
		104.0f,
		0.0f,
	};


	// ナンバー
	numberTexHD_ = TextureManager::LoadTexture("GameScene/UI", "Number.png");

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


void WaveCountUI::Draw2DFront(int nowCount, Camera* camera)
{
	// ウェーブ
	for (int i = 0; i < 2; ++i) {
		waveNumTransform_[i].UpdateMatrix();
	}
	waveNumSprite_[0]->SetSrc(src_[eachNum_[0] = nowCount]);
	waveNumSprite_[0]->Draw(numberTexHD_, waveNumTransform_[0], camera);
	waveNumSprite_[1]->SetSrc(src_[eachNum_[1]]);
	waveNumSprite_[1]->Draw(numberTexHD_, waveNumTransform_[1], camera);

	// スラッシュ
	slashTransform_.UpdateMatrix();
	slashSprite_->Draw(slashTexHD_, slashTransform_, camera);
}
