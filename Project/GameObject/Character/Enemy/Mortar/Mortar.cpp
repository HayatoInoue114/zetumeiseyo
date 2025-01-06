#include "Mortar.h"
#include "GameScene/GameScene.h"

void Mortar::Initialize(Player* player, Vector3 position, int level) {
	level;
	player_ = player;
	model_.CreateFromObj("mortar", {}, worldTransform_);
	itemModel_.CreateFromObj("box");
	worldTransform_.Initialize();
	worldTransform_.translate = position;
	worldTransform_.scale *= 2;

	nowCT = bulletCoolTime;
	param.searchDistance = 15.0f;

	texture_ = TextureManager::LoadTexture("", "black2x2.png");

	//hitParticle_ = std::make_unique<BulletHitParticle>();

	ParameterInitialize();

	isAlive_ = true;
	isFeed_ = false;
}

void Mortar::ParameterInitialize() {
	//パラメーター
	param.speed = 0.5f + level_ / 15.0f;
	param.hp = 15 + level_ * 2;
	param.power = level_ + 1;
	param.searchDistance = 15.0f + (float)level_ * 3;

	worldTransform_.scale = { (float)level_ / 2 + 1,(float)level_ / 2 + 1,(float)level_ / 2 + 1 };
}

void Mortar::Update() {
	if (!isFeed_) {
		
		Shoot();
		worldTransform_.UpdateMatrix();
		model_.SetColor(Vector4(0, 1, 0, 1));
		
		HitReaction();

		if (param.hp <= 0) {
			worldTransform_.scale *= 1.3f;
			model_.SetColor({ 0.0f,0.0f,255.0f,255.0f });
		    isFeed_ = true;
		}
	}
	else {
		FeedMove();
		z += 1.0f;
	//	model_.SetColor({ 0.0f,0.0f, z,255.0f });
		worldTransform_.UpdateMatrix();
	}

	for (EnemyBullet* bullet : bullet_) {
		bullet->UpdateMortar();
	}

	bullet_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
}

void Mortar::Draw(Camera* camera) {
	if (!isFeedAnimetionEnd_) {
		model_.Draw(worldTransform_, camera);
	}
	else {
		itemModel_.Draw(worldTransform_, camera);
	}
	for (EnemyBullet* bullet : bullet_) {
		bullet->Draw(camera);
	}
}

void Mortar::Shoot() {
	//CT減算
	nowCT--;

	//プレイヤーとの距離を計算
	distance = player_->GetWorldPos() - worldTransform_.translate;
	distanceToPlayer_ = Length(distance);

	if (param.searchDistance >= distanceToPlayer_) {
		RotateMove();
	}

	//射程範囲内かつCTが過ぎてる時
	if (param.searchDistance >= distanceToPlayer_ && nowCT <= 0) {
		nowCT = bulletCoolTime;

		EnemyBullet* newBullet;
		newBullet = new EnemyBullet();
		newBullet->InitializeMortar(worldTransform_.translate, player_->GetWorldPos());
		bullet_.push_back(newBullet);
		//gameScene->AddEnemyBulletM(newBullet);
	}
}

void Mortar::OnCollision(uint32_t id)
{
	if (id == ObjectId::PlayerBullet && !isFeed_) {
		Log("Hit!!\n");
		//param.hp -= 10;
		//param.hp -= player_->GetAttackLevel();
	}
	if (id == ObjectId::Player && isFeed_) {
		player_->AddEnergy(10);
		isAlive_ = false;
	}
}

void Mortar::OnCollisionWithPlayer(Player* player)
{
	if (isFeed_) {
		player->AddEnergy(10);
		isAlive_ = false;
	}
}

void Mortar::OnCollisionWithPlayerBullet(IPlayerBullet* bullet)
{
	if (!isFeed_) {
		Log("Hit!!\n");
		param.hp -= bullet->GetDamage();
		param.speed -= 0.02f;

		isHit = true;

		/*float color = 0;

		color += 30.0f;

		model_.SetColor({ 0.0f,0.0f,color,255.0f });*/

		//param.hp -= player_->GetAttackLevel();
	}
}

void Mortar::SettingColliderBit()
{
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Enemy);
	OBBCollider::SetCollisionMask(ObjectMask::Enemy);
	OBBCollider::SetID(ObjectId::Enemy);
}

void Mortar::SettingCollider()
{
	// OBBのサイズの設定
	this->size_ = 2.0f * worldTransform_.scale;
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(worldTransform_.rotate);
}

void Mortar::FeedMove()
{
	FeedAnimetion();
	const float kSerchDis = 10.0f;

	float feedTime = flame_ * 0.05f;

	if (isFeedAnimetionEnd_) {
		worldTransform_.scale = { 1.0f,1.0f,1.0f };
		worldTransform_.translate.y = FloatingAnimation(feedTime, 1.0f, 2.0f).y;
	}
	if (ToPlayerDis() <= kSerchDis) {
		chaseIntensity_ = 0.1f;
		param.searchDistance = 5.0f;
		param.speed = 0.8f;

		worldTransform_.rotate = {};

		// 座標を移動させる（1フレーム分の移動量を足しこむ)
		Vector3 toPlayer = player_->GetWorldPos() - worldTransform_.translate;

		//ベクトルを正規化する
		toPlayer = Normalize(toPlayer);
		//velocity_ = Normalize(velocity_);

		velocity_ = SLerp(velocity_, toPlayer, chaseIntensity_) * param.speed;

		worldTransform_.translate = worldTransform_.translate + velocity_;
	}
}

float Mortar::ToPlayerDis() {
	return (float)sqrt(pow(player_->GetWorldPos().x - worldTransform_.translate.x, 2) + pow(player_->GetWorldPos().z - worldTransform_.translate.z, 2));
}

void Mortar::RotateMove()
{
	LookAt(worldTransform_,player_->GetWorldPos());
}


// ベクトルから回転を計算する関数
Vector3 Mortar::CalculateRotation(const Vector3& direction) {
	Vector3 rotation{};
	rotation.y = std::atan2(direction.x, direction.z);
	return rotation;
}

// 点Aから点Bの方向に体を向ける関数
void Mortar::LookAt(WorldTransform& enemyTransform, const Vector3& playerPosition) {
	Vector3 direction = Normalize(playerPosition - enemyTransform.translate);
	enemyTransform.rotate = CalculateRotation(direction);
}

void Mortar::ShotAnimation()
{
	
}
