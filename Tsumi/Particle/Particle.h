#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "IParticleState.h"
#include "ParticlePlane.h"
#include "MyMath.h"


// Emitter
struct Emitter {
	WorldTransform worldTransform; // !< エミッタのワールドトランスフォーム
	uint32_t count; // !< 発生数
	float frequency; // !< 発生頻度
	float frequencyTime; // !< 頻度用時刻
};

/* Particleクラス */
class Particle {

public: // メンバ関数

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static Particle* GetInstance() {
		static Particle instance;
		return &instance;
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Particle() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Particle() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="state"></param>
	void Initialize(IParticleState* state, uint32_t instanceNum, uint32_t useTex = 1);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(uint32_t useTex, ViewProjection view);

	/// <summary>
	/// 先頭から情報を取り出す
	/// </summary>
	list<ParticleProperties> RetrieveFront();

	/// <summary>
	/// リストの登録
	/// </summary>
	void PushBackList(ParticleProperties prope);

	/// <summary>
	/// パーティクルの生成器
	/// </summary>
	static ParticleProperties ParticleGenerators(
		Scope lifeTimeScope,
		ScopeVec3 posScope,
		ScopeVec3 velScope = { .X {0.0f, 0.0f}, .Y {0.0f, 0.0f}, .Z {0.0f, 0.0f} },
		ScopeVec4 colorScope = { .X {256.0f, 256.0f}, .Y {256.0f, 256.0f}, .Z {256.0f, 256.0f}, .W {256.0f, 256.0f} });

	/// <summary>
	/// Emitterにによるパーティクル生成
	/// </summary>
	void Emit(
		const Emitter& emitter, 
		Scope lifeTimeScope,
		ScopeVec3 posScope,
		ScopeVec3 velScope = { .X {0.0f, 0.0f}, .Y {0.0f, 0.0f}, .Z {0.0f, 0.0f} },
		ScopeVec4 colorScope = { .X {256.0f, 256.0f}, .Y {256.0f, 256.0f}, .Z {256.0f, 256.0f}, .W {256.0f, 256.0f} });

#pragma region Get 取得

	/// <summary>
	/// インスタンス数の取得
	/// </summary>
	uint32_t GetInstanceNum() { return instanceNum_; }

	/// <summary>
	/// 使用するテクスチャハンドルの取得
	/// </summary>
	uint32_t GetUseTexture() { return this->useTexture_; }

	/// <summary>
	/// 何かのサイズの取得
	/// </summary>
	float GetSize() { return this->size_; }

#pragma endregion

#pragma region Set 設定

	/// <summary>
	/// テクスチャハンドルの設定
	/// </summary>
	void SetTexHandle(uint32_t texHD) { this->useTexture_ = texHD; }

#pragma endregion 



private: // メンバ変数

	// ステートパターン
	IParticleState* state_ = nullptr;

	// パーティクルリスト
	std::list<ParticleProperties> particlePropes_{};

	// インスタンス数
	uint32_t instanceNum_;

	// テクスチャ
	uint32_t useTexture_;

	// サイズ
	float size_;

	// Δタイム
	const float kDeltaTimer = 1.0f / 60.0f;

};

