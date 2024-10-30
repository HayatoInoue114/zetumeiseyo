#include "Player.h"
//#include "GameScene/GameScene.h"
#include "Enemy/IEnemy/IEnemy.h"
#include "GameManager.h"


// 初期化処理
void Player::Initialize()
{
	// Bodyの初期化処理
	bodyModel_ = make_unique<Model>();
	bodyModel_->CreateFromObj("PlayerBody", "Player");
	bodyWt_.Initialize();


	// 移動速度
	velocity_ = Vector3::zero;
	moveVector_ = 0.3f;


	// Bulletの初期化処理
	bulletModel_ = make_unique<Model>();
	bulletModel_->CreateFromObj("PlayerBullet", "Player");
	bulletVelocity_ = 1.5f;
	AttackInterval_ = 20;
	AttackTimer_ = 1;
	bulletType_ = TypeNormal;
	pelletNum_ = 5;


	// Reticleの初期化処理
	reticle_ = make_unique<PlayerReticle>();
	reticle_->Initialize();
	reticle_->SetPlayer(this);


	// エネルギーの初期設定
	energy_ = 100;


	// 死亡フラグ
	isDead_ = false;


	// 初期レベル
	totalLevel_ = 0;


	// ダッシュしているかのフラグ
	isDash_ = false;

	// ダッシュにかかるフレーム
	ASDashFrame_ = 20;


	// LevelUpPrope
	levelUpPrope_ = make_unique<PlayerLevelUpPrope>();
	levelUpPrope_->Initialize();
	levelUpPrope_->SetPlayer(this);

	// OperationUI
	operationUI_ = make_unique<OperationUI>();
	operationUI_->Initialize();

	// HpBar
	hpBar_ = make_unique<HpBar>();
	hpBar_->Initialize();
	hpBar_->SetPlayer(this);

	// 死亡時に使うパーティクル
	deadFuncParticle_ = make_unique<DeadFuncParticle>();
	deadFuncTex_ = TextureManager::LoadTexture("EffectMtl", "splash.png");

	// ダッシュパーティクル
	//dashParticle_ = make_unique<PlayerDashParticle>();
	dashParticleTexHD_ = TextureManager::LoadTexture("EffectMtl", "cloud.png");

	// PlayerSound
	sound_ = make_unique<PlayerSound>();
	sound_->Initialize();

	// レベルチェック
	LevelCheck();

	// Colliderのビットのセッティング
	SettingColliderBit();

	isShooting_ = false;
}


// 更新処理
void Player::Update(Camera* camera)
{
	// ワールドトランスフォームの初期化
	bodyWt_.UpdateMatrix();

	// Reticleの更新処理
	reticle_->Update(camera);

	if (!isDead_) {

		// 射撃処理
		IsAttack();

		// 移動処理
		Move();

		// 姿勢の処理
		//CalcBodyRotate();

		// ダッシュの処理
		DashFunc();

		// OBBColliderのセッティング
		SettingCollider();

		// HPチェック
		HPCheck();

		// 無敵時間処理
		InvincibleTime();

		if (isShooting_) {
			// 射撃中はカメラの進行方向に姿勢を合わせる
			FaceCameraDirection();
		}
		else {
			// 移動方向からY軸の姿勢を合わせる
			CalcBodyOrienation(iLStick_, stickMoveDirection_);
			CalcBodyOrienation(iKeys_, keyMoveDirection_);
		}
	}
	else {

		// 死亡演出
		DeadFunc();
	}


	// 色をセットしておく
	bodyModel_->SetColor(color_);

#ifdef _DEBUG

	if (ImGui::TreeNode("Player")) {

		ImGui::Text("WorldTransform");
		ImGui::DragFloat3("Scale", &bodyWt_.scale.x, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &bodyWt_.rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &bodyWt_.translate.x, 0.01f);
		ImGui::ColorEdit4("color", &color_.x);
		ImGui::Checkbox("isInvicible", &isInvincible_);


		ImGui::Text("");
		ImGui::Text("Energy = %d", energy_);

		ImGui::Text("");
		ImGui::Text("HP = %d", hp_);
		ImGui::Text("HPMax = %d", hpMax_);
		ImGui::Text("Val = %.2f", moveVector_);
		ImGui::Text("attack = %d", attack_);
		ImGui::Text("Interval = %d", AttackInterval_);
		ImGui::Text("Dash = %.1f", toDash_);

		ImGui::TreePop();
	}

#endif // _DEBUG
}


// 描画処理
void Player::Draw3D(Camera* camera)
{
	// Bodyの描画
	bodyModel_->Draw(bodyWt_, camera);

	// Reticleの初期化
	reticle_->Draw3D(camera);
}
void Player::Draw2DBack(Camera* camera)
{
	// LevelUpPrope
	levelUpPrope_->Draw2DBack(camera);

}
void Player::Draw2DFront(Camera* camera)
{
	// OperationUI
	operationUI_->Draw2DFront(camera);

	// HpBar
	hpBar_->Draw2DFront(camera);

	// LevelUpPrope
	levelUpPrope_->Draw2DFront(camera);
}


// 衝突判定
void Player::OnCollision(uint32_t id)
{
	if (id == ObjectId::EnemyBullet) {
		hp_ -= 1;
	}
}


// エネミーとの個別衝突判定
void Player::OnCollisionWithEnemy(IEnemy* enemy)
{
	if (!isDead_) {
		if(!enemy->IsFeed())
		hp_ -= 2;
	}
}

void Player::OnCollisionWithEnemyBullet()
{
	if (!isDead_) {

		// 無敵時間のフラグが立っていない時に通る
		if (!isInvincible_) {

			// 無敵時間のフラグを立てる
			isInvincible_ = true;

			// 体力を減らす
			hp_ -= 2;

			// alpha値を下げて無敵時間を分かりやすくする
			color_.w = 0.1f;

			// SE再生
			sound_->Play("damage", 1.0f);
		}
	}
}


// Colliderのビットのセッティング
void Player::SettingColliderBit()
{
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Player);
	OBBCollider::SetCollisionMask(ObjectMask::Player);
	OBBCollider::SetID(ObjectId::Player);
}


// OBBColliderのセッティング
void Player::SettingCollider()
{
	// OBBのサイズの設定
	this->size_ = 2.0f * bodyWt_.scale;
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(bodyWt_.rotate);
}


// 移動処理
void Player::Move() {

	//rotate
	LookAt(bodyWt_, reticle_->GetWorldPos());

	// velocityは0で毎フレーム初期化
	velocity_ = Vector3::zero;

	// stickの入力を取得
	iLStick_ = GamePadInput::GetLStick();

	// キーの処理
	if (KeysInput::PressKeys(DIK_W))
	{
		velocity_.z = moveVector_;
	};
	if (KeysInput::PressKeys(DIK_A))
	{
		velocity_.x = -moveVector_;
	};
	if (KeysInput::PressKeys(DIK_S))
	{
		velocity_.z = -moveVector_;
	};
	if (KeysInput::PressKeys(DIK_D))
	{
		velocity_.x = moveVector_;
	};

	// パッドの処理
	if (GamePadInput::GetLStick().x <= -0.3f)
	{
		velocity_.x = -1.0f;
	}
	if (GamePadInput::GetLStick().x >= 0.3f)
	{
		velocity_.x = 1.0f;
	}
	if (GamePadInput::GetLStick().y <= -0.3f)
	{
		velocity_.z = -1.0f;
	}
	if (GamePadInput::GetLStick().y >= 0.3f)
	{
		velocity_.z = 1.0f;
	}

	// 正規化
	if (velocity_.x != 0.0f || velocity_.z != 0.0f) {
		float length = Length({ velocity_.x, velocity_.z });
		velocity_.x /= length;
		velocity_.z /= length;
	}

	// velocityに速度を掛けて座標に加算
	bodyWt_.translate += (velocity_ * moveVector_);


	// 移動限界
	const float kMoveMit = 100.0f;
	bodyWt_.translate.x = max(bodyWt_.translate.x, -kMoveMit);
	bodyWt_.translate.x = min(bodyWt_.translate.x, +kMoveMit);
	bodyWt_.translate.z = max(bodyWt_.translate.z, -kMoveMit);
	bodyWt_.translate.z = min(bodyWt_.translate.z, +kMoveMit);
}


// プレイヤー本体の姿勢処理
void Player::CalcBodyRotate()
{
	Vector3 addRotate{};

	if (GamePadInput::PressButton(PadData::RIGHT_SHOULDER)) {

		addRotate = Normalize(reticle_->GetWorldPos() - bodyWt_.GetWorldPos());
	}
	else {

		if (GamePadInput::GetLStick().x <= -0.1f ||
			GamePadInput::GetLStick().x >= 0.1f) {
			addRotate.x = GamePadInput::GetLStick().x;
		}
		if (GamePadInput::GetLStick().y <= -0.1f ||
			GamePadInput::GetLStick().y >= 0.1f) {
			addRotate.z = GamePadInput::GetLStick().y;
		}
	}

	// Y軸周り角度(θy)
	bodyWt_.rotate.y = std::atan2(addRotate.x, addRotate.z);
}


// ダッシュの処理
void Player::DashFunc()
{
	// ダッシュ先のベクトル取得
	if (GamePadInput::PressButton(PadData::LEFT_SHOULDER) || KeysInput::TriggerKey(DIK_LSHIFT)) {

		// ボタン押下時にフラグが立っていなかったら
		if (!isDash_) {

			// フラグを立てる
			isDash_ = true;

			// 移動先の座標の計算
			toDashPosition_ = CalcToDashPos() + bodyWt_.GetWorldPos();

			// ダッシュ処理開始時の座標
			toInitDashPosition_ = bodyWt_.GetWorldPos();

			// ダッシュのフレームを０に設定
			nowDashFrame_ = 0;

			// SE再生
			sound_->Play("dash", 0.1f);
		}
	}

	// ダッシュのフラグがたったら
	if (isDash_) {

		// ダッシュのフレームをインクリメン
		nowDashFrame_++;

		// 今のフレームを終了フレームで割る
		float dashTime = float(nowDashFrame_) / float(ASDashFrame_);

		// イージングで座標に加算
		bodyWt_.translate.x = toDashPosition_.x + (toDashPosition_.x - toInitDashPosition_.x) * Ease::OutExpo(dashTime);
		bodyWt_.translate.z = toDashPosition_.z + (toDashPosition_.z - toInitDashPosition_.z) * Ease::OutExpo(dashTime);

		// 移動限界
		const float kMoveMit = 100.0f;
		bodyWt_.translate.x = max(bodyWt_.translate.x, -kMoveMit);
		bodyWt_.translate.x = min(bodyWt_.translate.x, +kMoveMit);
		bodyWt_.translate.z = max(bodyWt_.translate.z, -kMoveMit);
		bodyWt_.translate.z = min(bodyWt_.translate.z, +kMoveMit);

		// パーティクルを出しておく
		//dashParticle_->AddParticle(dashParticleTexHD_, bodyWt_.translate);

		// 終了したらフラグを折る
		if (nowDashFrame_ == ASDashFrame_) {

			isDash_ = false;
			nowDashFrame_ = 0;
		}
	}
}


// ダッシュ先の座標の計算
Vector3 Player::CalcToDashPos()
{
	Vector2 LStick{};
	if (GamePadInput::GetLStick().x <= 0.05f && GamePadInput::GetLStick().x >= -0.05f &&
		GamePadInput::GetLStick().y <= 0.05f && GamePadInput::GetLStick().y >= -0.05f) {
		LStick = { 0.0f, 1.0f };
	}
	else {
		LStick = GamePadInput::GetLStick();
	}
	Vector3 toDash = {
		bodyWt_.GetWorldPos().x + LStick.x,
		bodyWt_.GetWorldPos().y,
		bodyWt_.GetWorldPos().z + LStick.y,
	};
	
	Vector3 result = Normalize(toDash - bodyWt_.GetWorldPos());
	return result * toDash_;
}


// レベルチェック
void Player::LevelCheck()
{
	// HP
	hp_ = levelUpPrope_->SetHP();
	hpMax_ = levelUpPrope_->SetHP();

	//Vel
	moveVector_ = levelUpPrope_->SetVelocity();

	// Attack
	attack_ = levelUpPrope_->SetAttack();

	// AttackInterval
	AttackInterval_ = levelUpPrope_->SetAttackInterval();

	// Dash
	toDash_ = levelUpPrope_->SetDash();

	// BulletType
	bulletType_ = levelUpPrope_->SetBulletType();

	levelUpPrope_->CalcTotalLevel();
}


// HPチェック
void Player::HPCheck()
{
	// HPが0いかになったら死亡フラグをたてる
	if (hp_ <= 0) {
		isDead_ = true;
		hp_ = 0;

		// 死んだ座標を記録しておく
		deadPos_ = bodyWt_.translate;
	}
}


// 死亡演出
void Player::DeadFunc()
{
	// 震える処理
	TrembleFunc();

	// 出血処理
	BleedingFunc();
}


// 震える処理
void Player::TrembleFunc()
{
	if (!trembleFuncFinish_) {

		trembleNowFrame_++;

		// 乱数で震える処理を入れる
		Scope scope = { -2.0f, 2.0f };
		bodyWt_.translate.x = deadPos_.x + RandomGenerator::getRandom(scope);
		bodyWt_.translate.z = deadPos_.z + RandomGenerator::getRandom(scope);

		if (trembleNowFrame_ == trembleEndFrame_) {
			trembleNowFrame_ = 0;
			trembleFuncFinish_ = true;
			deadFuncFinish_ = true;
		}
	}
}


// 出血処理
void Player::BleedingFunc()
{
	if (trembleFuncFinish_) {

		//deadFuncParticle_->AddParticle(deadFuncTex_, bodyWt_.translate);
	}
}


// 衝突時無敵時間処理
void Player::InvincibleTime()
{
	// 無敵時間のフラグが立っていたら
	if (isInvincible_) {

		// フレームをデクリメント
		invincibleNowFrame_++;

		if (invincibleNowFrame_ == invincibleEndFrame_) {

			// フラグを折る
			isInvincible_ = false;

			// カラーのalpha値を元に戻す
			color_.w = 1.0f;

			// フレームは0に再設定
			invincibleNowFrame_ = 0;
		}
	}
}


// レベルの変更処理
void Player::ChangeLevelPrope()
{
	levelUpPrope_->Update();
}


// 射撃処理
void Player::IsAttack()
{
	// 射撃タイマーを減らす
	AttackTimer_--;

	// キー押下でタイマーを減らし射撃
	if (KeysInput::PressKeys(DIK_SPACE) && AttackTimer_ <= 0) {
		// 設定し直す
		AttackTimer_ = AttackInterval_;
		// バレットの設定をして射撃
		SettingNewBullet();
	}
	else if (KeysInput::ReleaseKeys(DIK_SPACE)) {

		// キーを離したら次の瞬間押してすぐ射撃できるように
		// １フレームを入れておく
		AttackTimer_ = 1;
	}

	// ボタン押下でタイマーを減らし射撃
	if (GamePadInput::PressButton(PadData::RIGHT_SHOULDER) && AttackTimer_ <= 0) {
		// 設定し直す
		AttackTimer_ = AttackInterval_;
		// バレットの設定をして射撃
		SettingNewBullet();
	}
	else if (GamePadInput::ReleaseButton(PadData::RIGHT_SHOULDER)) {

		// キーを離したら次の瞬間押してすぐ射撃できるように
		// １フレームを入れておく
		AttackTimer_ = 1;
	}
}


// バレットの設定
void Player::SettingNewBullet()
{
	// バレットタイプで処理分岐
	if (bulletType_ == TypeNormal || bulletType_ == TypePenetration) {

		// バレットのインスタンス作成
		shared_ptr<IPlayerBullet> newbullet = make_unique<PenetrationBullet>();

		// 初期座標
		Vector3 initPos = bodyWt_.GetWorldPos();
		// 移動速度
		Vector3 vel = CalcBulletDirection();

		// Bulletの初期化処理
		newbullet->Initialize(this, (*bulletModel_), initPos, vel, attack_);

		// SE再生
		sound_->Play("shooting", 0.05f);

		// シーンに登録
		registerScene_->AddPlayerBullets(newbullet);
	}
	else if (bulletType_ == TypeShotgun) {

		for (int i = 0; i < pelletNum_; i++) {

			// バレットのインスタンス作成
			shared_ptr<IPlayerBullet> newbullet = make_unique<Shotgun>();

			// 初期座標
			Vector3 initPos = bodyWt_.GetWorldPos();
			// 移動速度
			Vector3 vel = CalcBulletDirection();

			// Bulletの初期化処理
			newbullet->Initialize(this, (*bulletModel_), initPos, vel, attack_);

			// シーンに登録
			registerScene_->AddPlayerBullets(newbullet);
		}

		// SE再生
		sound_->Play("shooting", 0.05f);
	}
}


// バレットの進行方向の計算
Vector3 Player::CalcBulletDirection()
{
	Vector3 ReticlePos{};
	if (bulletType_ == TypeNormal || bulletType_ == TypePenetration) {

		ReticlePos = reticle_->GetWorldPos();
	}
	else if (bulletType_ == TypeShotgun) {

		if (GamePadInput::GetRStick().y <= 0.3f && GamePadInput::GetRStick().y >= -0.3f) {
			ReticlePos = reticle_->GetWorldPos() + RandomGenerator::getRandom({ { -0.05f, 0.05f, }, { 0.0f, 0.0f }, { 0.0f, 0.0f } });
		}
		else {
			ReticlePos = reticle_->GetWorldPos() + RandomGenerator::getRandom({ { -2.0f, 2.0f, }, { 0.0f, 0.0f }, { -2.0f, 2.0f } });
		}

	}
	Vector3 PlaToRet = Normalize(ReticlePos - bodyWt_.GetWorldPos());
	return PlaToRet * bulletVelocity_;
}

// ベクトルから回転を計算する関数
Vector3 Player::CalculateRotation(const Vector3& direction) {
	Vector3 rotation{};
	rotation.y = std::atan2(direction.x, direction.z);
	return rotation;
}

// 点Aから点Bの方向に体を向ける関数
void Player::LookAt(WorldTransform& playerTransform, const Vector3& position) {
	Vector3 direction = Normalize(position - playerTransform.translate);
	playerTransform.rotate = CalculateRotation(direction);
}

// カメラの方向に体の向きを合わせる
void Player::FaceCameraDirection()
{
	// カメラの前方ベクトルを取得
	Vector3 cameraForward = followCamera_->GetForwardVec();

	// カメラのY成分を無視して水平面上の方向を計算
	cameraForward.y = 0.0f;
	cameraForward = Normalize(cameraForward);  // 正規化して方向ベクトルにする

	// 目標の回転角度を求める（Y軸の回転）
	float targetAngle = std::atan2(cameraForward.x, cameraForward.z);

	// 現在の角度と目標角度から最短を求める
	float shortestAngle = ShortestAngle(bodyWt_.rotate.y, targetAngle);

	// 現在の角度を目標角度の間を補間
	bodyWt_.rotate.y =
		Lerp(bodyWt_.rotate.y, bodyWt_.rotate.y + shortestAngle, orientationLerpSpeed_);
}


// 移動方向からY軸の姿勢を合わせる
void Player::CalcBodyOrienation(Vector2 input, Vector3 direction)
{
	if (std::abs(input.x) > DZone_ || std::abs(input.y) > DZone_)
	{
		// 正規化した移動方向
		Vector3 normalizeDirection = Normalize(direction);

		// 目標回転角度
		float targetAngle = std::atan2(normalizeDirection.x, normalizeDirection.z);

		// 現在の角度と目標角度から最短を求める
		float shortestAngle = ShortestAngle(bodyWt_.rotate.y, targetAngle);

		// 現在の角度を目標角度の間を補間
		bodyWt_.rotate.y =
			Lerp(bodyWt_.rotate.y, bodyWt_.rotate.y + shortestAngle, orientationLerpSpeed_);
	}
}