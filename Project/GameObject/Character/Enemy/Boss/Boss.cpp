#include "Boss.h"
#include "GameScene/GameScene.h"

void Boss::Initialize(Player* player, Vector3 position, int level) {
	level;
	player_ = player;
	model_.CreateFromObj("boss", {}, worldTransform_);
	isAlive_ = true;
	isFeed_ = false;

	worldTransform_.Initialize();
	worldTransform_.translate = position;

	param.searchDistance = 15;

	ParameterInitialize();

	findFrame = 0;
	isFind = false;
}

void Boss::ParameterInitialize() {
	//パラメーター
	param.speed = 8.0f;
	param.hp = 550;
	param.power = 10;
	param.searchDistance = 45.0f;

	worldTransform_.scale = { 6,6,6 };
}

void Boss::Update() {
	if (!isFeed_) {
		Chase();
		Shoot();

		if (isFind) {
			findFrame++;

			if (findFrame >= maxFindFrame) {
				isFind = false;
			}
		}

		Vector3 a = player_->GetWorldPos() - worldTransform_.translate;

		// Y軸周り角度(0y)
		worldTransform_.rotate.y = std::atan2(a.x, a.z) + 3.14f;

		worldTransform_.UpdateMatrix();

		if (param.hp <= 0) {
			worldTransform_.scale *= 1.3f;
			model_.SetColor({ 0.0f,0.0f,255.0f,255.0f });
			isFeed_ = true;
		}
	}
	else {
		worldTransform_.UpdateMatrix();
	}

	for (EnemyBullet* bullet : bullet_) {
		if (!bullet->isMortor) {
			bullet->Update();
		}
		else {
			bullet->UpdateMortar();
		}
	}

	bullet_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
}

void Boss::Draw(Camera* camera) {

	model_.Draw(worldTransform_, camera);

	for (EnemyBullet* bullet : bullet_) {
		bullet->Draw(camera);
	}
}

void Boss::Shoot() {
	//CT減算
	nowCT--;
	nowMCT--;

	//プレイヤーとの距離を計算
	Vector3 distance = player_->GetWorldPos() - worldTransform_.translate;
	distanceToPlayer_ = Length(distance);

	//射程範囲内かつCTが過ぎてる時
	if ((distanceToPlayer_ <= param.searchDistance || isFind) && nowCT <= 0) {
		isFoundOnce = true;
		nowCT = bulletCoolTime;

		//Player目がけた弾
		Vector3 bulletVelocity = Normalize(distance) / 3.0f;
		EnemyBullet* newBullet;
		newBullet = new EnemyBullet();
		newBullet->Initialize(worldTransform_.translate, bulletVelocity, param.power);
		bullet_.push_back(newBullet);

		//拡散する弾
		if (bulletVelocity.x >= 0) {//
			if (bulletVelocity.z >= 0) { //右上
				for (int i = 1; i <= howManyExtraBullet; i++) {
					if (i % 2 == 0) { //rightSide
						EnemyBullet* newBullet;
						newBullet = new EnemyBullet();

						Vector3 vel = { bulletVelocity.x + i * diffusionRate, bulletVelocity.y, bulletVelocity.z - i * diffusionRate };
						newBullet->Initialize(worldTransform_.translate, vel, param.power);
						bullet_.push_back(newBullet);
					}
					else { //leftSide
						EnemyBullet* newBullet;
						newBullet = new EnemyBullet();

						Vector3 vel = { bulletVelocity.x - i * diffusionRate,bulletVelocity.y,bulletVelocity.z + i * diffusionRate };
						newBullet->Initialize(worldTransform_.translate, vel, param.power);
						bullet_.push_back(newBullet);
					}
				}
			}
			else { //右下
				for (int i = 1; i <= howManyExtraBullet; i++) {
					if (i % 2 == 0) { //rightSide
						EnemyBullet* newBullet;
						newBullet = new EnemyBullet();

						Vector3 vel = { bulletVelocity.x + i * diffusionRate, bulletVelocity.y, bulletVelocity.z + i * diffusionRate };
						newBullet->Initialize(worldTransform_.translate, vel, param.power);
						bullet_.push_back(newBullet);
					}
					else { //leftSide
						EnemyBullet* newBullet;
						newBullet = new EnemyBullet();

						Vector3 vel = { bulletVelocity.x - i * diffusionRate,bulletVelocity.y,bulletVelocity.z - i * diffusionRate };
						newBullet->Initialize(worldTransform_.translate, vel, param.power);
						bullet_.push_back(newBullet);
					}
				}
			}
		}
		else {
			if (bulletVelocity.z >= 0) { //左上
				for (int i = 1; i <= howManyExtraBullet; i++) {
					if (i % 2 == 0) { //rightSide
						EnemyBullet* newBullet;
						newBullet = new EnemyBullet();

						Vector3 vel = { bulletVelocity.x + i * diffusionRate, bulletVelocity.y, bulletVelocity.z + i * diffusionRate };
						newBullet->Initialize(worldTransform_.translate, vel, param.power);
						bullet_.push_back(newBullet);
					}
					else { //leftSide
						EnemyBullet* newBullet;
						newBullet = new EnemyBullet();

						Vector3 vel = { bulletVelocity.x - i * diffusionRate,bulletVelocity.y,bulletVelocity.z - i * diffusionRate };
						newBullet->Initialize(worldTransform_.translate, vel, param.power);
						bullet_.push_back(newBullet);
					}
				}
			}
			else { //左下
				for (int i = 1; i <= howManyExtraBullet; i++) {
					if (i % 2 == 0) { //rightSide
						EnemyBullet* newBullet;
						newBullet = new EnemyBullet();

						Vector3 vel = { bulletVelocity.x + i * diffusionRate, bulletVelocity.y, bulletVelocity.z - i * diffusionRate };
						newBullet->Initialize(worldTransform_.translate, vel, param.power);
						bullet_.push_back(newBullet);
					}
					else { //leftSide
						EnemyBullet* newBullet;
						newBullet = new EnemyBullet();

						Vector3 vel = { bulletVelocity.x - i * diffusionRate,bulletVelocity.y,bulletVelocity.z + i * diffusionRate };
						newBullet->Initialize(worldTransform_.translate, vel, param.power);
						bullet_.push_back(newBullet);
					}
				}
			}
		}
	}
	//MortorBullet
	if ((distanceToPlayer_ <= param.searchDistance || isFind) && nowMCT <= 0) {
		nowMCT = mBulletCoolTime;

		for (int i = 0; i < howManyMortorBullet; i++) {
			Vector3 toPosition = player_->GetWorldPos();

			Scope scopeX = { toPosition.x - 10.0f, toPosition.x + 10.0f };
			Scope scopeZ = { toPosition.z - 10.0f, toPosition.z + 10.0f };
			toPosition.x = RandomGenerator::getRandom(scopeX);
			toPosition.z = RandomGenerator::getRandom(scopeZ);

			EnemyBullet* newBullet;
			newBullet = new EnemyBullet();
			newBullet->InitializeMortar(worldTransform_.translate, toPosition);
			bullet_.push_back(newBullet);
		}
	}
}


void Boss::Chase() {
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

			velocity_ = Normalize(movePoint - worldTransform_.translate) * param.speed / 50;
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

			velocity_ = Normalize(movePoint - worldTransform_.translate) * param.speed / 50;
			velocity_.y = 0;

			isMove = true;
		}
	}
	if (isMove) {//移動中
		worldTransform_.translate += velocity_;
		velocity_.y = 0;

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

void Boss::OnCollision(uint32_t id)
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

void Boss::OnCollisionWithPlayer(Player* player)
{
	if (isFeed_) {
		player->AddEnergy(10);
		isAlive_ = false;
	}
}

void Boss::OnCollisionWithPlayerBullet(IPlayerBullet* bullet)
{
	if (!isFeed_) {
		Log("Hit!!\n");
		param.hp -= bullet->GetDamage();
		param.speed -= 0.02f;

		isFind = true;
		findFrame = 0;
		/*float color = 0;

		color += 30.0f;

		model_.SetColor({ 0.0f,0.0f,color,255.0f });*/

		//param.hp -= player_->GetAttackLevel();
	}
}

void Boss::SettingColliderBit()
{
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Enemy);
	OBBCollider::SetCollisionMask(ObjectMask::Enemy);
	OBBCollider::SetID(ObjectId::Enemy);
}

void Boss::SettingCollider()
{
	// OBBのサイズの設定
	this->size_ = 2.0f * worldTransform_.scale;
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(worldTransform_.rotate);
}