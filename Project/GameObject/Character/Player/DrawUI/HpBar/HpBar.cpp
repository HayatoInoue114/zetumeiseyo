#include "HpBar.h"
#include "Player/Player.h"


void HpBar::Initialize()
{
	// HpBar
	hpBarTexHD_ = TextureManager::LoadTexture("GameScene/Player", "HpBar.png");

	hpBarInitTexSize_ = { 480.0f, 32.0f };
	hpBarTexSize_ = hpBarInitTexSize_;

	hpBarSprite_ = make_unique<Sprite>();
	hpBarSprite_->Initialize(hpBarTexSize_);

	hpBarTransform_.Initialize();
	hpBarTransform_.translate = { 12.0f, 640.0f, 0.0f };


	// HpBarFrame
	frameTexHD_ = TextureManager::LoadTexture("GameScene/Player", "hpBarFrame.png");
	frameInitTexSize_ = { 480.0f, 32.0f };
	frameSprite_ = make_unique<Sprite>();
	frameSprite_->Initialize(frameInitTexSize_);
	frameTransform_.Initialize();
	frameTransform_.translate = { 12.0f, 640.0f, 0.0f };
}


void HpBar::Draw2DFront(Camera* camera)
{
	AdjustmentHpBarSize();
	hpBarTransform_.UpdateMatrix();
	frameTransform_.UpdateMatrix();
	hpBarSprite_->SetSize(hpBarTexSize_);

	hpBarSprite_->Draw(hpBarTexHD_, hpBarTransform_, camera);
	frameSprite_->Draw(frameTexHD_, frameTransform_, camera);
}


void HpBar::AdjustmentHpBarSize()
{
	// 補間でHpBarのサイズを調整する
	float t = float(player_->GetHp()) / float(player_->GetHpMax());
	hpBarTexSize_.x =
		0.0f + (hpBarInitTexSize_.x - 0.0f) * t;
}
