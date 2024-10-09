#pragma once

#include "MyMath.h"

/* Timerクラス */
class Timer final {

public: // メンバ関数

	// スタート時に呼び出す処理
	void Start(float setStart, float setEnd);

	// タイマーの更新処理
	void Update();

	// リセットみたいなやつ
	void Clear();

#pragma region Get 取得

	float GetNowFrame() { return this->nowFrame_; }
	float GetEndFrame() { return this->endFrame_; }

	bool GetIsActive() { return this->isActive_; }
	bool GetIsFinish() { return this->isFinish_; }

#pragma endregion 

#pragma region Set 設定

	void SetNowFrame(float setFrame) { this->nowFrame_ = setFrame; }
	void SetEndFrame(float setFrame) { this->endFrame_ = setFrame; }

	void SetIsActive(bool setFlag) { this->isActive_ = setFlag; }
	void SetIsFinish(bool setFlag) { this->isFinish_ = setFlag; }

#pragma endregion 


private: // メンバ変数

	float nowFrame_;
	float endFrame_;
	bool isActive_;
	bool isFinish_;
	
};

