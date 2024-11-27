#pragma once

#include <list>
#include <memory>

#include "../../GameObject.h"
#include "Input.h"

// 前方宣言
class Player;


/* フォローカメラクラス */
class FollowCamera {

public: // メンバ関数

	// コンストラクタ、デストラクタ
	FollowCamera() = default;
	~FollowCamera() = default;

	// 初期化処理　更新処理　描画処理
	void Init();
	void Update();
	void Draw3D();
	void Draw2DFront();
	void Draw2DBack();
	
	void Shake();

#pragma region Accessor アクセッサ

	// Player
	void SetPlayer(Player* player) { this->player_ = player; }

	// ForwardVec
	Vector3& GetForwardVec() { return this->forwardVec_; }

	// RightVec
	Vector3& GetRightVec() { return this->rightVec_; }

#pragma endregion 


private:

	// 回転処理
	void FuncOrientation();

	// フォロー処理
	void FuncFollow();

	// 前方ベクトルを求める
	void CalcForwardVec();

	// 右方ベクトルを求める
	void CalcRightVec();

	// ImGuiの描画
	void DrawImGui();
private:

	// プレイヤー
	Player* player_ = nullptr;

	// カメラマネージャー
	//CameraManager* cameraManager_ = nullptr;
	// カメラ本体
	Camera camera_{};

	// 入力
	GamePadInput* input_ = nullptr;
	// LStickの入力
	Vector2 iRStick_{};
	// デッドゾーン
	const float DZone_ = 0.2f;

	// 回転速度
	float addOrientationSpeed_ = 0.3f;
	// 姿勢計算の補間速度
	float orientationLerpSpeed_ = 0.5f;

	// 相対位置
	Vector3 offset_{};
	// プレイヤーからのオフセット
	Vector3 playerOffset_{};

	// 前方ベクトル
	Vector3 forwardVec_{};
	// 右方ベクトル
	Vector3 rightVec_{};

	//カメラシェイクのための変数
	float scopeRange_;

	int count_;
private:


};

