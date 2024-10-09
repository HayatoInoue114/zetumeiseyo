#pragma once

#include "GameObject.h"


/* EngineManualクラス */
class EngineManual {

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EngineManual() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EngineManual();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// 背景スプライトの描画処理
	/// </summary>
	void BackSpriteDraw(Camera* camera);

	/// <summary>
	/// ３Dオブジェクトの描画処理
	/// </summary>
	void ModelDraw(Camera* camera);

	/// <summary>
	/// 前景スプライトの描画処理
	/// </summary>
	void FrontSpriteDraw(Camera* camera);

private: // メンバ変数

	// 3Dモデル
	unique_ptr<Model> demoModel_ = nullptr; // <! 基本的にunique_ptrを使うこと
	WorldTransform demoModelWt_{}; // <! Wt = WorldTransform

	// 前景スプライト
	unique_ptr<Sprite> FSprite_ = nullptr;
	WorldTransform FSpriteWt_{};
	uint32_t FSpriteTexHD_;

	// 背景スプライト
	unique_ptr<Sprite> BSprite_ = nullptr;
	WorldTransform BSpriteWt_{};
	uint32_t BSpriteTexHD_;

	// オーディオ
	uint32_t kakkoiiAHD_; // <! AHD = Audio HanDle;
	uint32_t meriAHD_;
	uint32_t mokugyoAHD_;

};
