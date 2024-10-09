#pragma once

#include "MyMath.h"
#include "Struct.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "DirectXCommon.h"
#include "LightGraphicPipeline.h"
#include "CreateResource.h"
#include "IModelState.h"
#include "DescriptorManager.h"



/* ModelSphereクラス */
class ModelSphereState : public IModelState {

public: // メンバ関数

	ModelSphereState() {};
	~ModelSphereState() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* pModel) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(Model* pModel, WorldTransform worldTransform, Camera* camera) override;


private: // メンバ関数

	/// <summary>
	/// コマンドコール処理
	/// </summary>
	void CommandCall(uint32_t texture, WorldTransform worldTransform, Camera* camera);


private: // メンバ関数

	// リソース
	ResourcePeroperty resource_{};

	// 分割数
	int32_t subdivision_ = 16;
};