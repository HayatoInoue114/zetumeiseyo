#pragma once

#include "GameObject.h"


/* Fieldクラス */
class Field {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Field() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Field() {};

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static Field* GetInstance();

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
};

