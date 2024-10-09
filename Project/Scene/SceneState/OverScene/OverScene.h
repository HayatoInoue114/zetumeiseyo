#pragma once

#include "IScene.h"
#include "GameManager.h"
#include "GameObject.h"

#include "Skydome/Skydome.h"
#include "Ground/Ground.h"

#include "SaveValue/SaveValue.h"
#include "TotalLevelUI/TotalLevelUI.h"


class OverScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	OverScene() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~OverScene() {

		// ここでストップしておく
		Audio::StopOnSound(gameBGM_);
	};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(GameManager* state) override;

	/// <summary>
	/// 背景スプライトの描画処理
	/// </summary>
	void BackSpriteDraw() override;

	/// <summary>
	/// ３Dオブジェクトの描画処理
	/// </summary>
	void ModelDraw() override;

	/// <summary>
	/// 前景スプライトの描画処理
	/// </summary>
	void FrontSpriteDraw() override;


	/// <summary>
	/// プレイヤーバレットリストの追加
	/// </summary>
	void AddPlayerBullets(shared_ptr<IPlayerBullet> bullet) override { bullet; }


private:

	// メインカメラ
	unique_ptr<Camera> camera_ = nullptr;

	// フェード
	unique_ptr<Sprite> fadeSprite_;
	WorldTransform fadeWt_;
	uint32_t fadeTexHD_ = 0;

	// オーバーラベル
	uint32_t overTexHD_ = 0;
	WorldTransform overWt_{};
	unique_ptr<Sprite> overSp_;

	// プッシュAラベル
	uint32_t pushATexHD_ = 0;
	WorldTransform pushAWt_{};
	unique_ptr<Sprite> pushASp_;

	// トータルレベルUI
	std::unique_ptr<TotalLevelUI> totalLevelUI_;

	/* ----- Audio オーディオ----- */
	uint32_t gameBGM_;
	uint32_t tugiheSE_;

	bool isSceneChange_ = 0;
};

