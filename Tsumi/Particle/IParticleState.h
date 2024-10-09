#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"


// Particle
struct ParticleProperties {
	WorldTransform worldTransform;
	WorldTransform uvTransform;
	Vector4 color;
	Vector3 velocity;
	uint32_t lifeTime;
	uint32_t currentTime;
	bool isActive;
};


// Particleの前方宣言
class Particle;

/* IParticleState パーティクルのステートパターン */
class IParticleState {

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IParticleState() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(Particle* pParticle) = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(uint32_t texHD, Particle* pParticle, list<ParticleProperties> prope, ViewProjection view) = 0;


private: // メンバ変数

};