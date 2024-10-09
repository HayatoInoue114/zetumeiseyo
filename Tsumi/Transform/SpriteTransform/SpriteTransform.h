#pragma once

#include "MyMath.h"
#include "CreateResource.h"
#include "Struct.h"

#include <vector>


// 前方宣言
struct TransformationMatrix;

/* Sprite用のワールド変換データ */
struct SpriteTransform {

	// ローカルスケール
	Vector2 scale = Vector2::one;
	// ローカル回転軸
	float rotate = 0.0f;
	// ローカル座標
	Vector2 translate = Vector2::zero;

	// ローカル -> ワールド変換行列
	Matrix4x4 matWorld{};

	// 定数バッファー
	ComPtr<ID3D12Resource> constBuffer = nullptr;

	// マッピング済みアドレス
	TransformationMatrix* constMap;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 行列の更新処理
	/// </summary>
	void UpdateMatrix();


#pragma region Get

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	Vector3 GetWorldPos();

#pragma endregion

#pragma region Set

	/// <summary>
	/// 親子関係を結ぶ
	/// </summary>
	void SetParent(const WorldTransform* parentTransform);

#pragma endregion


	/// <summary>
	/// 定数バッファの生成
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();

	/// <summary>
	/// マッピング終了
	/// </summary>
	void UnMap();

	/// <summary>
	/// 行列の計算・転送
	/// </summary>
	void TransferMatrix();
};

