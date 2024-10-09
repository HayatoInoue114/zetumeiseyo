#include "Fire.h"
#include "GameScene/GameScene.h"

void Fire::Initialize(Player* player, Vector3 position, int level) {
	level;
	player_ = player;
	model_.CreateFromObj("fire", {}, worldTransform_);
	itemModel_.CreateFromObj("box");
	isAlive_ = true;
	isFeed_ = false;

	worldTransform_.Initialize();
	worldTransform_.translate = position;

	

	param.searchDistance = 15;

	ParameterInitialize();

}

void Fire::ParameterInitialize() {
	//パラメーター
	param.speed = 0.5f + level_ / 15.0f;
	param.hp = 15 + level_ * 2;
	param.power = level_ + 1;
	param.searchDistance = 15.0f + (float)level_ * 3;

	worldTransform_.scale = { (float)level_ / 2 + 1,(float)level_ / 2 + 1,(float)level_ / 2 + 1 };
}

void Fire::Update() {
	if (!isFeed_) {
		Chase();
		Shoot();

		Vector3 a = player_->GetWorldPos() - worldTransform_.translate;

		// Y軸周り角度(0y)
		worldTransform_.rotate.y = std::atan2(a.x, a.z) + 3.14f;

		worldTransform_.UpdateMatrix();

		if (param.hp <= 0) {
			worldTransform_.scale *= 1.3f;
			model_.SetColor({ 0.0f,0.0f,255.0f,255.0f });
			isFeed_ = true;
		}
		else {
			model_.SetColor(Vector4(1, 0, 0, 1));
		}
	}
	else {
		FeedMove();

		worldTransform_.UpdateMatrix();
	}

	for (EnemyBullet* bullet : bullet_) {
		bullet->Update();
	}

	bullet_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
}

void Fire::Draw(Camera* camera) {
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

void Fire::Shoot() {
	//CT減算
	nowCT--;

	//プレイヤーとの距離を計算
	Vector3 distance = player_->GetWorldPos() - worldTransform_.translate;
	distanceToPlayer_ = Length(distance);

	//射程範囲内かつCTが過ぎてる時
	if (distanceToPlayer_ <= param.searchDistance && nowCT <= 0) {
		isFoundOnce = true;
		nowCT = bulletCoolTime;

		Vector3 bulletVelocity = Normalize(distance) / 3.0f;
		EnemyBullet* newBullet;
		newBullet = new EnemyBullet();
		newBullet->Initialize(worldTransform_.translate, bulletVelocity, param.power);
		bullet_.push_back(newBullet);
		//gameScene->AddEnemyBullet(newBullet);
	}
}


void Fire::Chase() {
	//プレイヤーとの距離を計算
	Vector3 distance = player_->GetWorldPos() - worldTransform_.translate;
	distanceToPlayer_ = Length(distance);

	//射程範囲外かつ移動中でないとき
	if (distanceToPlayer_ >= param.searchDistance && !isMove) {
		//まだ一度もプレイヤーを発見していないとき
		if (!isFoundOnce) {
			//x,z ともに一定範囲内を探索
			Scope scope = { -20, 20 };
			movePoint.x = RandomGenerator::getRandom(scope);
			movePoint.z = RandomGenerator::getRandom(scope);

			velocity_ = Normalize(movePoint - worldTransform_.translate) / 9.0f;
			velocity_.y = 0;

			isMove = true;
		}
		else {
			//一度でもプレイヤーを発見したとき
			float random = 5.0f;
			//プレイやーの一定範囲内を目標に移動
			Scope scopeX = { player_->GetWorldPos().x - random, player_->GetWorldPos().x + random };
			Scope scopeZ = { player_->GetWorldPos().z - random, player_->GetWorldPos().z + random };
			movePoint.x = RandomGenerator::getRandom(scopeX);
			movePoint.z = RandomGenerator::getRandom(scopeZ);

			velocity_ = Normalize(movePoint - worldTransform_.translate) / 9.0f;
			velocity_.y = 0;

			isMove = true;
		}
	}
	if (isMove) {//移動中
		worldTransform_.translate += velocity_;

		//目標地点にたどり着いたとき
		if (Length(movePoint - worldTransform_.translate) <= 1.0f) {
			isMove = false;
		}
		//移動中索敵範囲から外れた際、移動位置の再抽選
		//if (distanceToPlayer_ <= param.searchDistance) {
		//	isMove = false;
		//}
	}
}

void Fire::OnCollision(uint32_t id)
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

void Fire::OnCollisionWithPlayer(Player* player)
{
	if (isFeed_) {
		player->AddEnergy(10);
		isAlive_ = false;
	}
}

void Fire::OnCollisionWithPlayerBullet(IPlayerBullet* bullet)
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

void Fire::SettingColliderBit()
{
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Enemy);
	OBBCollider::SetCollisionMask(ObjectMask::Enemy);
	OBBCollider::SetID(ObjectId::Enemy);
}

void Fire::SettingCollider()
{
	// OBBのサイズの設定
	this->size_ = 2.0f * worldTransform_.scale;
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(worldTransform_.rotate);
}

void Fire::FeedMove()
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

float Fire::ToPlayerDis() {
	return (float)sqrt(pow(player_->GetWorldPos().x - worldTransform_.translate.x, 2) + pow(player_->GetWorldPos().z - worldTransform_.translate.z, 2));
}