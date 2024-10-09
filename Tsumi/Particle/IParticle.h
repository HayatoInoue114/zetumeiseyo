#pragma once

#include "MyMath.h"


/* IParticleクラス */
class IParticle {

public: // メンバ関数
	 
	// コンストラクタ、デストラクタ
	IParticle() {};
	virtual ~IParticle() {};

	// 初期化処理、更新処理
	virtual void Initialize() = 0;
	virtual void Update() = 0;

	// パーティクルの追加
	virtual void AddParticle(uint32_t texture, Vector3 pos) = 0;

	// 新しいインスタンスを作成
	template<typename T>
	static std::unique_ptr<T> Create() {
		std::unique_ptr<T> newParticle = std::make_unique<T>();
		newParticle->Initialize();
		return newParticle;
	}

#pragma region Get 取得

	// 名前の取得
	std::string const GetName() { return this->name_; }

	// トランスフォームの取得
	WorldTransform GetTransform() { return this->wt_; }

	// UVTransformの取得
	WorldTransform GetUvTransform() { return this->uv_; }

	// 発生率の取得
	float GetRate() const { return this->perSecond_; }

	// 発生方向の取得
	Vector3 GetDirection() const { return this->direction_; }

	// 初期速度の取得
	Vector3 GetVelocity() const { return this->velocity_; }

	// 加速度の設定
	Vector3 GetAcceleration() const { return this->acceleration_; }

	// 寿命の取得
	float GetLifeTime() const { return this->lifeTime_; }
	float GetMinLifeTime() const { return this->minLifetime_; }
	float GetMaxLifeTime() const { return this->maxLifetime_; }

	// 色の取得
	Vector4 GetColor() const { return this->color_; }

	// アクティブフラグ
	bool GetIsActive() const { return this->isActive_; }

#pragma endregion 

#pragma region Set 設定

	// トランスフォームの取得
	void SetTransform(WorldTransform setWt) { this->wt_ = setWt; }

	// SRTの設定　
	void SetScale(Vector3 setS) { this->wt_.scale = setS; }
	void SetRotate(Vector3 setR) { this->wt_.rotate = setR; }
	void SetTranslate(Vector3 setT) { this->wt_.translate = setT; }

	// UVTransformの取得
	void SetUvTransform(WorldTransform setUv) { this->uv_ = setUv; }

	// 発生率の設定
	void SetRate(float setPs) { this->perSecond_ = setPs; }

	// 発生方向の設定
	void SetDirection(Vector3 setDirection) { this->direction_ = setDirection; }

	// 初期速度の設定
	void SetVelocity(Vector3 setVel) { this->velocity_ = setVel; }

	// 加速度の設定
	void SetAcceleration(Vector3 setAcc) { this->acceleration_ = setAcc; }

	// 寿命の設定
	void SetLifeTime(float setTime) { this->lifeTime_ = setTime; }
	void SetMinMaxLifeTime(float setMin, float setMax) { this->minLifetime_ = setMin; this->maxLifetime_ = setMax; }
	void SetMinLifeTime(float setMin) { this->minLifetime_ = setMin; }
	void SetMaxLifeTime(float setMax) { this->maxLifetime_ = setMax; }

	// 色の設定
	void SetColor(Vector4 setColor) { this->color_ = setColor; }

	// アクティブフラグ
	void SetIsActive(bool state) { this->isActive_ = state; }

#pragma endregion 

protected: // メンバ変数

	// パーティクルの名前
	std::string name_{};

	// トランスフォーム
	WorldTransform wt_{};

	// uv
	WorldTransform uv_{};

	// 発生率
	float perSecond_;

	// 発生方向
	Vector3 direction_{};

	// 初期速度
	Vector3 velocity_{};

	// 加速度
	Vector3 acceleration_{};

	// 寿命
	float lifeTime_;
	float minLifetime_;
	float maxLifetime_;

	// 色
	Vector4 color_{};

	// アクティブフラグ
	bool isActive_;
};

