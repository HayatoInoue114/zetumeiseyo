#pragma once

#include <list>
#include <memory>

#include "../GameObject.h"
#include "Input.h"

// 前方宣言
class Player;


/* フォローカメラクラス */
class Indicator {

public: // メンバ関数
	Indicator();
	~Indicator();

	// 初期化処理
	void Init();
	// 更新処理
	void Update(const Vector3& enemyPosition);
	//描画処理
	void Draw3D(Camera* camera);



#pragma region Accessor アクセッサ

	// Player
	void SetPlayer(Player* player) { this->player_ = player; }

	// ForwardVec
	Vector3& GetForwardVec() { return this->forwardVec_; }

	// RightVec
	Vector3& GetRightVec() { return this->rightVec_; }

	void SetIsAlive(bool isAlive) { this->isAlive_ = isAlive; }

	bool IsAlive() { return this->isAlive_; }

#pragma endregion 


private:

	void FuncFollow(const Vector3& directionToEnemy);

	// 前方ベクトルを求める
	void CalcForwardVec();

	// 右方ベクトルを求める
	void CalcRightVec();

	// ImGuiの描画
	void DrawImGui();

	void ColorChange();

	float CalculateEuclideanDistance(const Vector3& point1, const Vector3& point2);

private:

	// プレイヤー
	Player* player_ = nullptr;

	Vector3 enemyPos_{};
	//本体
	WorldTransform worldTransform_{};
	unique_ptr<Model> model_ = nullptr;

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
	Vector3 constOffset_{};
	// プレイヤーからのオフセット
	Vector3 playerOffset_{};

	// 前方ベクトル
	Vector3 forwardVec_{};
	// 右方ベクトル
	Vector3 rightVec_{};

	//色を点滅させるフラグ
	bool isColorChange_{};
	//フレームカウント
	float count_;
	//色のフラグ
	bool isColor_;
	//色の点滅範囲
	float colorBlinkRange_{};
	//色
	Vector4 color_{};
	//モデルを消すためのフラグ
	bool isAlive_{};
};

