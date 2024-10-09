#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"
#include "ImGuiManager.h"

/* DebugCameraクラス */
class DebugCamera {

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	WorldTransform& GetWorldTransform() { return worldTransform_; };

	/// <summary>
	/// ビュープロジェクションの取得関数
	/// </summary>
	ViewProjection& GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// カメラのアクティブフラグの取得
	/// </summary>
	bool& GetIsActive() { return IsActive_; }

	/// <summary>
	/// 有効フラグの設定
	/// </summary>
	void SetIsActive(bool state) { IsActive_ = state; }

private:

	// ワールドトランスフォーム
	WorldTransform worldTransform_{};

	// ビュープロジェクション
	ViewProjection viewProjection_{};

	/// <summary>
	/// デバッグカメラが有効か
	/// </summary>
	bool IsActive_ = false;
};