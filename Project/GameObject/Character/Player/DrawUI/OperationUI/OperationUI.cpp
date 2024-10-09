#include "OperationUI.h"



void OperationUI::Initialize()
{
	vectorSize_ = 2;

	texHD_.resize(vectorSize_);
	sprite_.resize(vectorSize_);
	transform_.resize(vectorSize_);

	texHD_[0] = TextureManager::LoadTexture("GameScene/Player", "Dash_UI.png");
	texHD_[1] = TextureManager::LoadTexture("GameScene/Player", "Attack_UI.png");

	texSize_ = { 108.0f, 86.0f }; 

	for (size_t i = 0; i < vectorSize_; ++i) {

		sprite_[i] = make_unique<Sprite>();
		sprite_[i]->Initialize(texSize_);
		sprite_[i]->SetSpriteOrigin(SpriteOrigin::Center);

		transform_[i].Initialize();
		transform_[i].translate = { 
			1064.0f + (i * texSize_.x), 
			640.0f, 
			0.0f};
	}
}


void OperationUI::Draw2DFront(Camera* camera)
{
	for (size_t i = 0; i < vectorSize_; ++i) {

		transform_[i].UpdateMatrix();

		sprite_[i]->Draw(texHD_[i], transform_[i], camera);
	}
}
