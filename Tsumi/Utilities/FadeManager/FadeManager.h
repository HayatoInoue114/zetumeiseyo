#pragma once

#include "GameObject.h"


struct FadeNowState {
	bool isFadeIn;
	bool isFadeOut;
};

enum FunctionFade {
	func_FadeIn,
	func_FadeOut,
};


/* フェードの処理 */
class FadeManager {

public:
	
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static FadeManager* GetInstance() {
		static FadeManager instance;
		return &instance;
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FadeManager() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FadeManager() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize(FunctionFade func);

	/// <summary>
	/// 描画処理
	/// </summary>
	static void Draw(Camera* camera);

	/// <summary>
	///フェードイン処理
	/// </summary>
	static bool IsFadeIn();

	/// <summary>
	/// フェードアウト処理
	/// </summary>
	static bool IsFadeOut();


private:

	/// <summary>
	/// フレームの初期化処理
	/// </summary>
	static void FrameInit();

	/// <summary>
	/// フレームの更新処理
	/// </summary>
	static void FrameUpdate(FunctionFade func);

private:

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// テクスチャハンドル
	uint32_t texHD_;

	// スプライト用トランスフォーム
	WorldTransform wt_{};

	// カラー
	Vector4 color_{};

	// 今の状態
	FadeNowState state_;

	// フレーム
	int startFrame_;
	int finishFrame_;
	float time_;
};

