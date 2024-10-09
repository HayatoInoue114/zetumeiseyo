#pragma once

#include "IScene.h"
#include "GameManager.h"
#include "GameObject.h"

#include "Skydome/Skydome.h"
#include "Ground/Ground.h"

#include "ResultUI/ResultUI.h"
#include "TotalLevelUI/TotalLevelUI.h"

#include "MyMath.h"


class ClearScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ClearScene() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ClearScene() {

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
	unique_ptr<Camera> camera_;

	// フェード
	unique_ptr<Sprite> fadeSprite_;
	WorldTransform fadeWt_;
	uint32_t fadeTexHD_ = 0;

	// クリアラベル
	uint32_t ClearTexHD_ = 0;
	WorldTransform ClearWt_{};
	unique_ptr<Sprite> ClearSp_;

	// プッシュAラベル
	uint32_t pushATexHD_ = 0;
	WorldTransform pushAWt_{};
	unique_ptr<Sprite> pushASp_;

	// リザルトUI
	vector<unique_ptr<ResultUI>> resultUI_;
	size_t resultUISize_;
	vector<std::string> valueKey_;

	// トータルレベルUI
	std::unique_ptr<TotalLevelUI> totalLevelUI_;

	/* ----- Audio オーディオ----- */
	uint32_t gameBGM_;
	uint32_t tugiheSE_;

	bool isSceneChange_ = 0;
};

