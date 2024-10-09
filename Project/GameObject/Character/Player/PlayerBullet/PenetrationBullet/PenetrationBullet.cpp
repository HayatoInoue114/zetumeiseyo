#include "PenetrationBullet.h"
#include "Player/Player.h"



// 初期化処理
void PenetrationBullet::Initialize(Player* player, Model& modeHD, Vector3 initPos, Vector3 vel, int damage)
{
	// Playerの設定
	player_ = player;

	// モデルの設定
	bulletModel_ = make_unique<Model>();
	(*bulletModel_) = modeHD;

	// ワールドトランスフォームの初期化 & 座標の設定
	bulletWt_.Initialize();
	bulletWt_.translate = initPos;

	// 速度の設定
	velocity_ = vel;

	// 姿勢の計算
	bulletWt_.rotate.y = std::atan2(vel.x, vel.z);
	float velZ = std::sqrt((vel.x * vel.x) + (vel.z * vel.z));
	float height = -vel.y;
	bulletWt_.rotate.x = std::atan2(height, velZ);

	// 死亡フラグは折っておく
	isDead_ = false;

	// 寿命の時間の設定 2秒に設定
	lifeTimer_ = 3 * 60;

	// ダメージの設定
	damage_ = damage;
	hp_ = damage;

	// 衝突カウント
	hitCount_ = 0;
	deadCount_ = 3;

	// Colliderのビットのセッティング
	SettingColliderBit();
}


// 更新処理
void PenetrationBullet::Update()
{
	// ワールドトランスフォームの初期化
	bulletWt_.UpdateMatrix();

	// 移動処理
	Move();

	// OBBColliderのセッティング
	SettingCollider();

	// 寿命の処理
	RemoveAfterLifetime();
}


// 描画処理
void PenetrationBullet::Draw3D(Camera* camera)
{
	bulletModel_->Draw(bulletWt_, camera);
}


// 衝突判定
void PenetrationBullet::OnCollision(uint32_t id)
{
	if (id == ObjectId::Enemy) {

		// 衝突フラグを立てる
		isHit_ = true;

		// 衝突カウントをインクリメント
		hitCount_++;

		//// エネルギーの加算
		//player_->AddEnergy(10);

		// 無敵時間の処理に入る
		if (InitiateShieldFunc()) {

			// 既定値を超えたら死亡フラグを立てる
			if (hitCount_ >= deadCount_) {

				// エネミーと衝突したら死亡フラグを立てる
				isDead_ = true;
			}
		}
	}
}


// Colliderのビットのセッティング
void PenetrationBullet::SettingColliderBit()
{
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Player);
	OBBCollider::SetCollisionMask(ObjectMask::Player);
	OBBCollider::SetID(ObjectId::PlayerBullet);
}


// OBBColliderのセッティング
void PenetrationBullet::SettingCollider()
{
	// OBBのサイズの設定
	this->size_ = {
		2.0f * bulletWt_.scale.x,
		1.5f * bulletWt_.scale.y,
		2.0f * bulletWt_.scale.z,
	};
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(bulletWt_.rotate);
}


// 移動処理
void PenetrationBullet::Move()
{
	bulletWt_.translate += velocity_;
}


// 無敵時間の処理
bool PenetrationBullet::InitiateShieldFunc()
{
	// 衝突していなかったら早期リターン
	if (!isHit_) {
		return false;
	}

	// 無敵時間のタイマーをデクリメント
	shieldTimer_++;

	// タイマーが既定値でフラグとタイマーを再セット
	if (shieldTimer_ >= 3) {

		isHit_ = false;
		shieldTimer_ = 0;

		return true;
	}

	return false;
}
