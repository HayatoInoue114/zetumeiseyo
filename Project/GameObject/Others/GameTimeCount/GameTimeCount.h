#pragma once

#include "GameObject.h"

/* GameTimeCountクラス */
class GameTimeCount {

public: // メンバ関数

	// コンストラクタ、デストラクタ
	GameTimeCount() {};
	~GameTimeCount() {};

	// 初期化処理、更新処理、描画処理
	void Initialize();
	void Update();
	void DrawFront2D(Camera* camera);

	// タイマーを減らす
	void SubTimer();

	// 時間の追加処理
	void AddTimer(uint32_t addTime);

	// 制限時間をリセットする
	void ReSetTimer();

#pragma region Get

	// 制限時間の取得
	uint32_t GetNowGameTime() const { return this->nowGameTimer_; }

	// 時間切れのフラグ
	bool IsTimeUp() const { return this->isTimeUp_; }

#pragma endregion 


private: // メンバ関数

	// 秒針を回転させる
	void AdvanceClockHand();

	// タイムバーを縮小させる
	void ReductTimeBarSizeX();

	// フレームを秒に変換する
	uint32_t ConvertToSecond();


private: // メンバ変数

	// ゲームの制限時間
	uint32_t nowGameTimer_ = 0;
	bool isTimeUp_ = false;
	uint32_t gameFrame_ = 0;
	uint32_t gameTime_ = 0;


	/* ----- Clock 時計 ----- */
	uint32_t clockTex_ = 0;
	unique_ptr<Sprite> clockSp_;
	WorldTransform clockWt_{};


	/* ----- ClockHand 秒針 ----- */
	uint32_t clockHandTex_ = 0;
	unique_ptr<Sprite> clockHandSp_;
	WorldTransform clockHandWt_{};

	// 秒針の終了回転量
	float endClockHandRotateZ_ = 4.75f;

	// 秒針の等速前進のフレーム
	float advanceNowFrame_ = 0;
	float advanceEndFrame_ = 0;


	/* ----- TimeBar タイムバー ----- */
	uint32_t timeBarTex_ = 0;
	unique_ptr<Sprite> timeBarSp_;
	WorldTransform timeBarWt_{};
	Vector2 timeBarSize_{};
	Vector2 timeBarInitSize_{};

	// タイムバーの終了サイズ
	float endTimeBarSizeX_ = 88.0f;

	// タイムバーの縮小フレーム
	float reductNowFrame_ = 0;
	float reductEndFrame_ = 0;


	/* ----- Back バック ----- */
	uint32_t backTex_ = 0;
	unique_ptr<Sprite> backSp_;
	WorldTransform backWt_{};
};

