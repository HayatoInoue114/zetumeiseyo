#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Camera.h"


// Modelの前方宣言
class Model;

/* IModelState モデルのステートパターン */
class IModelState {

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IModelState() {};


	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(Model* pModel) = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(Model* pModel, WorldTransform worldTransform, Camera* camera) = 0;


private: // メンバ変数

};