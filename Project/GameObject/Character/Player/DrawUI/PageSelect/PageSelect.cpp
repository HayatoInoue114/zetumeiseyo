#include "PageSelect.h"



void PageSelect::Initialize()
{
	size_ = 2;

	sprite_.resize(size_);
	transform_.resize(size_);

	texHD_ = TextureManager::LoadTexture("GameScene/Player", "pageSelect.png");

	for (size_t i = 0; i < size_; ++i) {

		sprite_[i] = make_unique<Sprite>();
		sprite_[i]->Initialize({ 28.0f, 28.0f });
		sprite_[i]->SetSpriteOrigin(SpriteOrigin::Center);

		transform_[i].Initialize();
		transform_[i].translate = {
			612.0f + (i * 56.0f),
			656.0f - 48.0f,
			0.0f,
		};
	}
}


void PageSelect::Draw2DFront(int nowPage, Camera* camera)
{
	if (nowPage == 0) {
		sprite_[0]->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
		sprite_[1]->SetColor(Vector4::one);
	}
	else if (nowPage == 1) {
		sprite_[0]->SetColor(Vector4::one);
		sprite_[1]->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	}
	for (size_t i = 0; i < size_; ++i) {

		transform_[i].UpdateMatrix();
		sprite_[i]->Draw(texHD_, transform_[i], camera);
	}
}
