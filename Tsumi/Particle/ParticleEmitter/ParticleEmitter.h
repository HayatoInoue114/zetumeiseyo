#pragma once

#include "MyMath.h"

/* ParticleEmitterクラス */
class ParticleEmitter {

public: // メンバ関数

	// コンストラクタ、デストラクタ
	ParticleEmitter() {};
	~ParticleEmitter() {};

	// パーティクルの追加
	void AddParticle();

#pragma region Get 取得

	// 発生座標の取得
	Vector3 GetPosition() { return this->position_; }

	// 発生率の取得
	float GetRate() { return this->perSecond_; }

	// 発生方向の取得
	Vector3 GetDirection() { return this->direction_; }

	// 初期速度の取得
	Vector3 GetVelocity() { return this->velocity_; }

	// 寿命の取得
	float GetMinLifeTime() { return this->minLifetime_;}
	float GetMaxLifeTime() { return this->maxLifetime_; }

	// 色の取得
	Vector4 GetColor() { return this->color_; }

#pragma endregion 

#pragma region Set 設定

	// 発生座標の設定
	void SetPosition(Vector3 setPos) { this->position_ = setPos; }

	// 発生率の設定
	void SetRate(float setPs) { this->perSecond_ = setPs; }

	// 発生方向の設定
	void SetDirection(Vector3 setDirection) { this->direction_ = setDirection; }

	// 初期速度の設定
	void SetVelocity(Vector3 setVel) { this->velocity_ = setVel; }

	// 寿命の設定
	void SetLifeTime(float setMin, float setMax) { this->minLifetime_ = setMin; this->maxLifetime_ = setMax; }
	void SetMinLifeTime(float setMin) { this->minLifetime_ = setMin; }
	void SetMaxLifeTime(float setMax) { this->maxLifetime_ = setMax; }

	// 色の設定
	void SetColor(Vector4 setColor) { this->color_ = setColor; }

#pragma endregion 


private: // メンバ関数


private: // メンバ変数

	// 座標
	Vector3 position_{};

	// 発生率
	float perSecond_;

	// 発生方向
	Vector3 direction_{};

	// 初期速度
	Vector3 velocity_{};

	// 寿命
	float minLifetime_;
	float maxLifetime_;

	// 色
	Vector4 color_{};
};

