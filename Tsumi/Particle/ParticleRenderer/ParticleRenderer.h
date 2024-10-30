#pragma once
#include "MyMath.h"
#include "Struct.h"
#include "IParticle.h"
#include "ParticleGraphicPipeline.h"
#include "CreateResource.h"
#include "DescriptorManager.h"
#include "Camera.h"

/* ParticleRendererクラス */
class ParticleRenderer {

public: // メンバ関数

	// コンストラクタとデストラクタ
	ParticleRenderer() {};
	~ParticleRenderer() {};

	// 初期化処理、更新処理、描画処理
	void Initialize(uint32_t maxInstance);
	void Draw(const list<unique_ptr<IParticle>>& p,  uint32_t useTexture, Camera* camera);

#pragma region Get 取得

	// テクスチャの取得
	uint32_t GetUseTexture() { return this->useTexture_; }

#pragma endregion 

#pragma region Set 取得

	// テクスチャの設定
	void SetUseTexture(uint32_t setTexture) { this->useTexture_ = setTexture; }

#pragma endregion 


private: // メンバ関数

	// コマンドコール
	void CommandCall();

	// ビルボードの処理
	Matrix4x4 CalcBillBord(Camera* camera, Vector3 rotate);


private: // メンバ変数

	// リソース
	ResourcePeroperty resource_{};

	// 
	uint32_t dsvIndex_ = 0;

	// インスタンスの最大と個数
	const uint32_t kMaxInstanceNum_ = 5000;
	uint32_t instanceNum_;

	Vector4 pos_ = { 0.0f, 0.0f, 0.0f, 1.0f };
	float size_ = 0.5f;

	// 使用するテクスチャ
	uint32_t useTexture_;
};

