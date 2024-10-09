#pragma once
#include "MyMath.h"
#include "Struct.h"
#include "IParticleState.h"
#include "ParticleGraphicPipeline.h"
#include "CreateResource.h"
#include "DescriptorManager.h"
#define INSTANCE_MAX 1000;


/* ParticlePlaneクラス */
class ParticlePlane : public IParticleState {

public: // メンバ関数

	ParticlePlane() {};
	~ParticlePlane() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Particle* pParticle) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(uint32_t texHD, Particle* pParticle, list<ParticleProperties> prope , ViewProjection view) override;


private: // メンバ関数

	/// <summary>
	/// コマンドコール
	/// </summary>
	void CommandCall(uint32_t texHandle);

	/// <summary>
	/// ビルボードの処理
	/// </summary>
	Matrix4x4 CalcBillBord(ViewProjection view);


private: // メンバ変数

	// リソース
	ResourcePeroperty resource_{};

	// ModelData
	ModelData modelData_{};

	Vector4 pos_ = { 0.0f, 0.0f, 0.0f, 1.0f };
	float size_ = 0.5f;

	const uint32_t kMaxInstanceNum_ = INSTANCE_MAX;
	uint32_t instanceNum_;

	uint32_t itrNum_ = 0;

	uint32_t dsvIndex_ = 0;
};

