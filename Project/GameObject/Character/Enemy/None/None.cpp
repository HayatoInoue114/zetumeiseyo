#include "None.h"
#include "GameScene/GameScene.h"

void None::Initialize(Player* player, Vector3 position, int level) {
	level;
	player_ = player;
	model_.CreateFromObj("none", {}, worldTransform_);
	isAlive_ = true;
	isFeed_ = false;

	worldTransform_.Initialize();
	worldTransform_.translate = position;

	param.searchDistance = 15;

	ParameterInitialize();
}

void None::ParameterInitialize() {
	//パラメーター
	param.speed = 0.5f + level_ / 15.0f;
	param.hp = 15 + level_ * 2;
	param.power = level_ + 1;
	param.searchDistance = 15.0f + (float)level_ * 3;

	worldTransform_.scale = { (float)level_ / 2 + 1,(float)level_ / 2 + 1,(float)level_ / 2 + 1 };
}

void None::Update() {
	worldTransform_.UpdateMatrix();

	if (param.hp <= 0 && !isFeed_) {
		worldTransform_.scale *= 1.3f;
		model_.SetColor({ 0.0f,0.0f,255.0f,255.0f });
		isFeed_ = true;
	}
}

void None::Draw(Camera* camera) {
	if (isAlive_) {
		model_.Draw(worldTransform_, camera);
	}
}

void None::OnCollision(uint32_t id)
{
	if (id == ObjectId::PlayerBullet && !isFeed_) {
		Log("Hit!!\n");
		//param.hp -= 10;
		//param.hp -= player_->GetAttackLevel();
	}
	if (id == ObjectId::Player && isFeed_) {
		player_->AddEnergy(100);
		isAlive_ = false;
	}
}

void None::OnCollisionWithPlayer(Player* player)
{
	if (isFeed_) {
		player->AddEnergy(100);
		isAlive_ = false;
	}
}

void None::OnCollisionWithPlayerBullet(IPlayerBullet* bullet)
{
	if (!isFeed_) {
		Log("Hit!!\n");
		param.hp -= bullet->GetDamage();
		param.speed -= 0.02f;

		/*float color = 0;

		color += 30.0f;

		model_.SetColor({ 0.0f,0.0f,color,255.0f });*/

		//param.hp -= player_->GetAttackLevel();
	}
}

void None::SettingColliderBit()
{
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Enemy);
	OBBCollider::SetCollisionMask(ObjectMask::Enemy);
	OBBCollider::SetID(ObjectId::Enemy);
}

void None::SettingCollider()
{
	// OBBのサイズの設定
	this->size_ = 2.0f * worldTransform_.scale;
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(worldTransform_.rotate);
}