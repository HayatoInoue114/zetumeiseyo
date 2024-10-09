#include "PlayerReticle.h"
#include "Player/Player.h"



// 初期化処理
void PlayerReticle::Initialize()
{
	reticleModel_ = make_unique<Model>();
	reticleModel_->CreateFromObj("PlayerReticle", "Player");
	offset_ = { 0.0f, 0.0f, 10.0f };
	reticleWt_.Initialize();
	reticleWt_.translate = offset_;
	theta1_ = { 0.0f, 0.0f };
	theta1_ = { 0.0f, 1.0f };
	velocity_ = Vector2::zero;
	thetaMove_ = 0.01f;
}


// 更新処理
void PlayerReticle::Update(Camera* camera)
{
	// ワールドトランスフォームの初期化
	reticleWt_.UpdateMatrix();

	// 移動処理
	Move();

	// カメラに向ける
	CalcRotateToCamera(camera);


	if (KeysInput::PressKeys(DIK_LEFT)) {
		reticleWt_.translate.x -= 0.2f;
	}
	if (KeysInput::PressKeys(DIK_RIGHT)) {
		reticleWt_.translate.x += 0.2f;
	}

	Vector2 cont = GamePadInput::GetRStick();

#ifdef _DEBUG


#endif // _DEBUG
}


// 描画処理
void PlayerReticle::Draw3D(Camera* camera)
{
	reticleModel_->Draw(reticleWt_, camera);
}


// 移動処理
void PlayerReticle::Move()
{
	// stickの入力を取得
	Vector2 rSthickInput = GamePadInput::GetRStick();

	// stick入力が一定範囲を超えている場合、角度を更新
	if (std::abs(rSthickInput.x) > 0.2f || std::abs(rSthickInput.y) > 0.2f) {

		// 入力に基づいて角度を更新
		rethickeAngle_ = std::atan2(rSthickInput.y, rSthickInput.x);
	}

	// レティクルの位置を計算
	Vector3 reticlePos = {
		kRetickeRad_ * std::cosf(rethickeAngle_),
		0.0f,
		kRetickeRad_ * std::sinf(rethickeAngle_),
	};

	// プレイヤーの位置を取得
	Vector3 playerPos = player_->GetWorldPos();

	// レティクルの位置を設定
	reticleWt_.translate = playerPos + reticlePos;
}


// カメラに向く処理
void PlayerReticle::CalcRotateToCamera(Camera* camera)
{
	// カメラとの差分
	Vector3 diffRotate = Normalize(reticleWt_.GetWorldPos() - camera->GetWorldPos());

	// 姿勢をカメラに向ける
	reticleWt_.rotate.y = atan2(diffRotate.x, diffRotate.z);
	float vecZ = sqrt((diffRotate.x * diffRotate.x) + (diffRotate.z * diffRotate.z));
	float height = -diffRotate.y;
	reticleWt_.rotate.x = atan2(height, vecZ);
}

