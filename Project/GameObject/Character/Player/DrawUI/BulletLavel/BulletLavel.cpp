#include "BulletLavel.h"



void BulletLavel::Initialize(int num, Vector2 pos)
{
	if (num == 0) {
		texHD_ = TextureManager::LoadTexture("GameScene/Player", "Lavel_Normal.png");
	}
	else if (num == 1) {
		texHD_ = TextureManager::LoadTexture("GameScene/Player", "Lavel_Penetration.png");
	}
	else if (num == 2) {
		texHD_ = TextureManager::LoadTexture("GameScene/Player", "Shotgun.png");
	}

	size_ = { 152.0f, 104.0f };

	sprite_ = make_unique<Sprite>();
	sprite_->Initialize(size_);
	sprite_->SetSpriteOrigin(SpriteOrigin::Center);

	transform_.Initialize();
	transform_.translate = { pos.x, pos.y, 0.0f };
}


void BulletLavel::Draw2DFront(int nowPage, Camera* camera)
{
	if (nowPage == 1) {
		transform_.UpdateMatrix();
		sprite_->Draw(texHD_, transform_, camera);
	}
}
