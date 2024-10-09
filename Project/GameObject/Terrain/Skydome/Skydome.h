#pragma once

#include "GameObject.h"


/* Skydomeクラス */
class Skydome {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Skydome() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Skydome() {};

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static Skydome* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(Camera* camera);


private:

	// モデル
	std::unique_ptr<Model> model_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_{};

	// カラー
	Vector4 color_{};

	// ライト
	DirectionalLight light_{};

	// ノーマルマップ
	uint32_t normalMapTexHD_;

};

