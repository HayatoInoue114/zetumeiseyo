#include "BackScreen.h"



void BackScreen::Initialize()
{
	texHD_ = TextureManager::LoadTexture("GameScene/Player", "LevelUpBack.png");

	sprite_ = make_unique<Sprite>();
	sprite_->Initialize({ 960.0f, 540.0f });
	sprite_->SetSpriteOrigin(SpriteOrigin::Center);

	transform_.Initialize();
	transform_.translate = { WinApp::kHalfWindowWidth, WinApp::kHalfWindowHeight, 0.0f };
}


void BackScreen::Draw2DFront(Camera* camera)
{
	transform_.UpdateMatrix();

	sprite_->Draw(texHD_, transform_, camera);
}
