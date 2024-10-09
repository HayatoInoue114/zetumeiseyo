#pragma once

#include "GameObject.h"

class Player;

/* PlayerReticleクラス */
class PlayerReticle {

public: // プレイヤーのレティクル : メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerReticle() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerReticle() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw3D(Camera* camera);

#pragma region Get 取得

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	Vector3 GetWorldPos() { return this->reticleWt_.GetWorldPos(); }

#pragma endregion

#pragma region Set 設定

	/// <summary>
	/// Playerの設定
	/// </summary>
	void SetPlayer(Player* setPlayer) { this->player_ = setPlayer; }

#pragma endregion 

private: 

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// カメラに向く処理
	/// </summary>
	void CalcRotateToCamera(Camera* camera);

private:

	Player* player_ = nullptr;

	unique_ptr<Model> reticleModel_ = nullptr;
	WorldTransform reticleWt_{};

	// theta θ
	float rethickeAngle_ = 0.0f;
	const float kRetickeRad_ = 5.0f;
	const float kAngleSpeed_ = 1.0f;

	Vector2 theta1_{};
	Vector2 theta2_{};
	Vector2 velocity_{};
	float thetaMove_;

	// プレイヤーからのオフセット
	Vector3 offset_;

	Vector2 hr{};
};

