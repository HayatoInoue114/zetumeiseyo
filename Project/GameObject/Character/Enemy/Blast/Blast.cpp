#include "Blast.h"
#include "GameObject.h"
#include <algorithm>

void Blast::Initialize(Player* player, Vector3 position, int level) {
	player_ = player;
	worldTransform_.Initialize();
	worldTransform_.translate = position;
	velocity_ = { 0,0,0 };
	//model_.CreateFromObj("PlayerBody","Player");
	model_.CreateFromObj("Blast");
	itemModel_.CreateFromObj("box");
	isAlive_ = true;
	isFeed_ = false;
	velocityXZ_ = 0;
	cntAhead_ = kTurnInterval_;
	rad_ = 0;
	t_ = 0;
	flame_ = 0;
	tmpPos_ = {};
	level_ = level;
	color_ = { 0,0,0,1 };
	colorW_ = 0.8f;
	colorBlinkRange_ = 20;
	count_ = -colorBlinkRange_;
	isBlast_ = false;
	isDiscover_ = false;
	originalPos_ = {};
	hitColor_ = { 0,0,0,1 };

	blastParticle_ = make_unique<BlastParticle>();

	particletex_ = TextureManager::LoadTexture("", "black2x2.png");

	ParameterInitialize();

	SettingColliderBit();
}

void Blast::Initialize(Vector3 position, int level) {
	worldTransform_.Initialize();
	worldTransform_.translate = position;
	velocity_ = { 0,0,0 };
	//model_.CreateFromObj("PlayerBody","Player");
	model_.CreateFromObj("Blast");
	isAlive_ = true;
	isFeed_ = false;
	velocityXZ_ = 0;
	cntAhead_ = kTurnInterval_;
	rad_ = 0;
	t_ = 0;
	flame_ = 0;
	tmpPos_ = {};
	level_ = level;
	color_ = { 0,0,0,1 };
	colorW_ = 0.8f;
	colorBlinkRange_ = 20;
	count_ = -colorBlinkRange_;
	isBlast_ = false;
	isDiscover_ = false;
	originalPos_ = {};
	hitColor_ = { 0,0,0,1 };

	blastParticle_ = make_unique<BlastParticle>();

	particletex_ = TextureManager::LoadTexture("", "black2x2.png");

	ParameterInitialize();

	SettingColliderBit();
}

void Blast::ParameterInitialize() {
	//パラメーター
	param.speed = 0.7f + level_ / 25.0f;
	param.hp = 15 + level_ * 2;
	param.power = level_ * 3;
	param.searchDistance = 20.0f + (float)level_ * 3;
	//worldTransform_.scale = { (float)level_ / 2 + 1,(float)level_ / 2 + 1,(float)level_ / 2 + 1 };

	initParam_ = param;
}

void Blast::Update() {
	flame_++;
	if (!isFeed_) {
		//Wander();

		CalculateDetectionRange();

		DiscoverPlayer();

		Chace();

		SettingCollider();

		ColorChange();

		BlastOnNearby();

		//param.speed = std::clamp(param.speed, 0.0f, 1.0f);

		worldTransform_.UpdateMatrix();

		if (param.hp <= 0) {
			worldTransform_.scale *= 1.3f;
			worldTransform_.translate.y = 0.0f;
			model_.SetColor({ 0.0f,0.0f,1.0f,1.0f });
			isFeed_ = true;
		}
	}
	else {
		FeedMove();

		worldTransform_.UpdateMatrix();
	}
}

void Blast::Draw(Camera* camera) {
	if (!isFeedAnimetionEnd_) {
		model_.Draw(worldTransform_, camera);
	}
	else {
		itemModel_.Draw(worldTransform_, camera);
	}
	
}

std::unique_ptr<Blast> Blast::Spawn(Player* player, Vector3 position, int level) {
	std::unique_ptr<Blast> newEnemy = std::make_unique<Blast>();
	newEnemy->Initialize(player, position, level);

	return newEnemy;
}

void Blast::OnCollisionWithPlayer(Player* player)
{
	if (isFeed_) {
		player->AddEnergy(10);
		isAlive_ = false;
	}

}

void Blast::OnCollisionWithPlayerBullet(IPlayerBullet* bullet)
{
	if (!isFeed_) {
		Log("Hit!!\n");
		param.hp -= bullet->GetDamage();
		param.speed -= 0.02f;

		hitColor_.x = (float)param.hp / (float)initParam_.hp;
		hitColor_.y = (float)param.hp / (float)initParam_.hp;

		model_.SetColor(hitColor_);

		

		//param.hp -= player_->GetAttackLevel();
	}
}

void Blast::CalculateDetectionRange() {
	//範囲内か計算
	distanceToPlayer_ = CalculateEuclideanDistance(player_->GetWorldPos(), worldTransform_.translate);

	//もし索敵範囲内なら追いかける
	if (distanceToPlayer_ <= param.searchDistance) {
		if (!isTrace_) {
			isDiscover_ = true;
		}
		//isTrace_ = true;
	}
	else {
		//isDiscover_ = false;
		isTrace_ = false;
	}
}

void Blast::DiscoverPlayer() {
	if (isDiscover_) {
		// ジャンプの高さ
		float jumpHeight = 1.5f;

		// ジャンプの時間
		float jumpDuration = 1.0f;

		// 現在の時間
		float currentTime = 1.0f / 60.0f;

		animationTime_ += currentTime;

		if (animationTime_ == currentTime) {
			originalPos_ = worldTransform_.translate;
			originalScale_ = worldTransform_.scale;
		}

		worldTransform_.translate = Jump(originalPos_, jumpHeight, jumpDuration, animationTime_);
		worldTransform_.scale = JumpScale(originalScale_,jumpDuration, animationTime_);
		Shake(jumpDuration, animationTime_);
	}
	else {
		animationTime_ = 0;
		worldTransform_.scale = { (float)level_ / 2 + 1,(float)level_ / 2 + 1,(float)level_ / 2 + 1 };
	}
}

void Blast::Chace() {
	if (isTrace_ && !isBlast_) {
		worldTransform_.rotate = {};

		count_++;
		blastCount_++;
		// 座標を移動させる（1フレーム分の移動量を足しこむ)
		Vector3 toPlayer = player_->GetWorldPos() - worldTransform_.translate;

		//ベクトルを正規化する
		toPlayer = Normalize(toPlayer);

		velocity_ = SLerp(velocity_, toPlayer, chaseIntensity_) * param.speed;

		worldTransform_.translate = worldTransform_.translate + velocity_;

		// Y軸周り角度(0y)
		worldTransform_.rotate.y = std::atan2(velocity_.x, velocity_.z);

		velocityXZ_ = std::sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
		// X軸周り角度(0x)
		worldTransform_.rotate.x = std::atan2(-velocity_.y, velocityXZ_);

		t_ = 0;

		colorBlinkRange_ = ToPlayerDis();
	}
}


void Blast::Wander() {
	if (!isTrace_ && !isBlast_) {
		blastCount_ = 0;
		isDiscover_ = false;
		
		if (t_ == 0) {
			float rad = 10;

			Scope scope = { rad / 2,rad };
			rad_ = RandomGenerator::getRandom(scope);
			tmpPos_ = CalculateNextPosition(worldTransform_.translate, rad_);

			Scope vel = { -0.15f,0.15f };

			Scope zero = {};

			ScopeVec3 vel3 = { vel,zero,vel };

			vel3_ = RandomGenerator::getRandom(vel3);
		}

		tmpPos_ += vel3_;

		Scope scope = { 120.0f,180.0f };

		float currentTime = 1.0f / RandomGenerator::getRandom(scope);

		if (t_ <= 1.0f) {
			t_ += currentTime;
		}if (t_ > 1.0f) {
			t_ = 0;
		}

		float easedValue = (float)easeInOutBounce(t_);

		worldTransform_.translate = MoveWithEasing(worldTransform_.translate, tmpPos_, easedValue);

		// Y軸周り角度(0y)
		worldTransform_.rotate.y = CalculateAngle(worldTransform_.translate, tmpPos_);

		velocityXZ_ = std::sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	}
}

void Blast::ColorChange() {
	if (isTrace_) {
		//色を何秒で変化させるか
		if (count_ < 0) {
			isColor_ = true;
		}
		if (count_ >= 0) {
			isColor_ = false;
		}

		if (count_ >= colorBlinkRange_) {
			count_ = -colorBlinkRange_;
		}

		if (isColor_) {
			color_ = { 1,0,0,1 };
		}
		else {
			color_ = { 1,1,1,1 };
		}
	}
	else {
		color_ = { 1,1,1,1 };
	}

	model_.SetColor(color_);
}

void Blast::TitleColorBlink()
{
	count_++;
	colorBlinkRange_ -= 0.5f;

	//色を何秒で変化させるか
	if (count_ < 0) {
		isColor_ = true;
	}
	if (count_ >= 0) {
		isColor_ = false;
	}

	if (count_ >= colorBlinkRange_) {
		count_ = -colorBlinkRange_;
	}

	if (isColor_) {
		color_ = { 1,0,0,1 };
	}
	else {
		color_ = { 1,1,1,1 };
	}

	model_.SetColor(color_);
}

void Blast::FeedMove()
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

float Blast::CalculateEuclideanDistance(const Vector3& point1, const Vector3& point2) {
	float deltaX = point2.x - point1.x;
	float deltaY = point2.y - point1.y;
	float deltaZ = point2.z - point1.z;

	return std::sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

Vector3 Blast::GetFrontVector() {
	Matrix4x4 wM = MakeRotateXYZMatrix(worldTransform_.rotate);
	Vector3 dir = { 0.0f,0.0f,1.0f };
	return Normalize(Multiply(dir, wM));
}

Vector3 Blast::Multiply(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{};
	result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
	return result;
}

Vector3 Blast::CalculateNextPosition(const Vector3& position, float radius) {
	// 乱数生成器の準備
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

	// ランダムな方向ベクトルを生成し、長さradiusにスケーリングする
	float dx = dis(gen) * radius;
	float dy = dis(gen) * radius;

	// 予測移動先座標を計算する
	float nextX = position.x + dx;
	float nextZ = position.z + dy;

	return { nextX, 0,nextZ };
}

Vector3 Blast::MoveWithEasing(const Vector3& currentPosition, const Vector3& targetPosition, float t) {
	// 現在位置から目標位置までの距離と方向を計算する
	float deltaX = targetPosition.x - currentPosition.x;
	float deltaY = targetPosition.z - currentPosition.z;

	// イージングを適用した移動量を計算する
	float newX = currentPosition.x + deltaX * t;
	float newZ = currentPosition.z + deltaY * t;

	// Y軸周り角度(0y)
	worldTransform_.rotate.y = std::atan2(newX, newZ);

	velocityXZ_ = std::sqrt(newX * newX + newZ * newZ);
	// X軸周り角度(0x)
	worldTransform_.rotate.x = std::atan2(-velocity_.y, velocityXZ_);

	return { newX, 0,newZ };
}

void Blast::BlastOnNearby()
{
	if (ToPlayerDis() <= 1.0f && !isBlast_) {
		isBlast_ = true;
		isBlastParticle_ = true;
		player_->SetDamage(param.power);
		for (int i = 0; i < 25; i++) {
			//blastParticle_->AddParticle(particletex_, this->GetWorldTransform().translate);
		}
	}

	if (isBlast_) {
		if (t_ <= 0.01f) {
			//blastParticle_->AddParticle(particletex_, this->GetWorldTransform().translate);
		}
	}

	if (isBlast_) {
		color_ = { 1,0,0,colorW_ };
		model_.SetColor(color_);


		float currentTime = 1.0f / 30.0f;

		if (t_ <= 1.0f) {
			t_ += currentTime;
		}

		float easedValue = (float)easeOutQuint(t_);
		float easedValueColor = (float)easeInCirc(t_);

		colorW_ = 1.0f - easedValueColor;
	
		blastRange_ = {1 + easedValue + easedValue, 1 + easedValue + easedValue, 1 + easedValue + easedValue };
		
		worldTransform_.scale = blastRange_;
	}

	if (t_ >= 1.0f) {
		isAlive_ = false;
	}
}

float Blast::easeInOutQuad(float startValue, float movement, float currentTime) {
	float duration = 1.0;  // イージングにかかる時間（1秒と仮定）

	currentTime /= duration / 2.0f;
	if (currentTime < 1.0f) {
		return movement / 2.0f * currentTime * currentTime + startValue;
	}

	currentTime--;
	return -movement / 2.0f * (currentTime * (currentTime - 2.0f) - 1.0f) + startValue;
}

Vector3 Blast::easeInOutQuad(Vector3 startValue, Vector3 movement, float currentTime) {
	float duration = 1.0;  // イージングにかかる時間（1秒と仮定）

	currentTime /= duration / 2.0f;
	if (currentTime < 1.0f) {
		return movement / 2.0f * currentTime * currentTime + startValue;
	}

	currentTime--;
	return -movement / 2.0f * (currentTime * (currentTime - 2.0f) - 1.0f) + startValue;
}

float Blast::easeInOutQuad(float x) {
	return x < 0.5f ? 2.0f * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 2.0f) / 2.0f;
}

float Blast::ToPlayerDis() {
	return (float)sqrt(pow(player_->GetWorldPos().x - worldTransform_.translate.x, 2) + pow(player_->GetWorldPos().z - worldTransform_.translate.z, 2));
}

double Blast::easeOutBounce(double x) {
	const double n1 = 7.5625;
	const double d1 = 2.75;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		x -= 1.5 / d1;
		return n1 * x * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		x -= 2.25 / d1;
		return n1 * x * x + 0.9375;
	}
	else {
		x -= 2.625 / d1;
		return n1 * x * x + 0.984375;
	}
}

double Blast::easeInBounce(double x) {
	return 1 - easeOutBounce(1 - x);
}

double Blast::easeInOutBounce(double x) {
	return x < 0.5
		? (1 - easeOutBounce(1 - 2 * x)) / 2
		: (1 + easeOutBounce(2 * x - 1)) / 2;
}

double Blast::easeOutQuint(double x) {
	return 1 - std::pow(1 - x, 5);
}

float Blast::CalculateAngle(const Vector3& current, const Vector3& target) {
	// 目標位置と現在位置の差を計算
	float deltaX = target.x - current.x;
	float deltaZ = target.z - current.z;

	// atan2関数を使用して角度を計算
	return std::atan2(deltaX, deltaZ);
}

double Blast::easeInCirc(double x) {
	return 1 - std::sqrt(1 - std::pow(x, 2));
}

Vector3 Blast::Jump(const Vector3& initialPosition, float jumpHeight, float jumpDuration, float currentTime) {
	// 現在のジャンプ時間を[0, 1]の範囲に正規化する
	float normalizedTime = (float)min(currentTime / jumpDuration, 1.0f);

	// ジャンプの進行度をイージング関数で計算する
	float jumpProgress;
	if (normalizedTime < 0.5f) {
		jumpProgress = jumpHeight * (float)easeOutQuint(normalizedTime * 2.0f);
	}
	else {
		jumpProgress = jumpHeight * (1.0f - (float)easeOutQuint((normalizedTime - 0.5f) * 2.0f));
	}

	// 初期位置からの垂直方向の距離を計算し、それを元の位置に加える
	Vector3 finalPosition = initialPosition;
	finalPosition.y += jumpProgress;

	//プレイヤーに体を向ける
	worldTransform_.rotate.y = CalculateAngle(worldTransform_.translate, player_->GetWorldPos());

	// ジャンプが終了したら元の位置に戻る
	if (currentTime >= jumpDuration) {
		isTrace_ = true;
		isDiscover_ = false;
		finalPosition.y = initialPosition.y;
	}

	return finalPosition;
}

void Blast::Shake(float jumpDuration, float currentTime) {
	// 現在のジャンプ時間を[0, 1]の範囲に正規化する
	float normalizedTime = (float)min(currentTime / jumpDuration, 1.0f);

	Scope scope = { -0.2f,0.2f };
	ScopeVec3 vec3 = { scope,scope,scope };

	// ジャンプの進行度をイージング関数で計算する
	if (normalizedTime < 0.5f && flame_ % 5 == 0) {
		worldTransform_.translate += RandomGenerator::getRandom(vec3);
	}
}

Vector3 Blast::JumpScale(const Vector3& initialScale, float jumpDuration, float currentTime) {

	// ジャンプ後のスケールを計算
	Vector3 finalScale = initialScale;
	if (currentTime < jumpDuration) {
		finalScale += calculateJumpScale(currentTime, jumpDuration);
	}
	else {
		finalScale += calculateLandingScale(currentTime, jumpDuration,initialScale);
	}

	// ジャンプが終了したら元の位置に戻る
	if (currentTime >= jumpDuration) {
		isTrace_ = true;
		isDiscover_ = false;
		finalScale = initialScale;
	}

	return finalScale;
}

// ジャンプ中のスケールを計算する関数
Vector3 Blast::calculateJumpScale(float currentTime, float jumpDuration) {
	// ジャンプ中の時間を[0, 1]の範囲に正規化する
	float normalizedTime = min(currentTime / jumpDuration, 1.0f);

	// スケールの計算
	float scaleMultiplier = 0.5f * (float)std::sin(M_PI * min(normalizedTime * 2.0f, 1.0f));
	return { -scaleMultiplier * 0.5f, scaleMultiplier, -scaleMultiplier* 0.5f };
}

// 着地時のスケールを計算する関数
Vector3 Blast::calculateLandingScale(float currentTime, float jumpDuration, const Vector3& initialScale) {
	// 着地時の時間を[0, 1]の範囲に正規化する
	float normalizedTime = min(currentTime / jumpDuration, 1.0f);
	// イージング関数を使用してスケールの変化を計算
	float scaleMultiplier = 1.0f - (float)easeOutQuint(normalizedTime);

	// xとz方向のスケールを増やし、y方向のスケールを減らす
	Vector3 landingScale = {
		initialScale.x + scaleMultiplier * 0.5f, // x方向のスケールを増やす
		initialScale.y * (1.0f - scaleMultiplier), // y方向のスケールを減らす
		initialScale.z + scaleMultiplier * 0.5f // z方向のスケールを増やす
	};

	return landingScale;
}

//使わん
void Blast::SettingColliderBit()
{
	OBBCollider::SetCollosionAttribute(ObjectAttribute::Enemy);
	OBBCollider::SetCollisionMask(ObjectMask::Enemy);
	OBBCollider::SetID(ObjectId::Enemy);
}
void Blast::SettingCollider()
{
	// OBBのサイズの設定
	this->size_ = 2.0f * worldTransform_.scale;
	OBBCollider::SetSize(this->size_);
	OBBCollider::SetRotate(worldTransform_.rotate);
}

void Blast::OnCollision(uint32_t id)
{
	id;
}