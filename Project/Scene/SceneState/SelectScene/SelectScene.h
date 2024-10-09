#pragma once

#include "IScene.h"
#include "GameManager.h"
#include "GameObject.h"
#include "FadeManager.h"
#include "Enemy/EnemyManager/EnemyManager.h"


enum SelectSceneNowSelect {
	toGameScene,
	toManualScene,
	toTitleScene
};

class SelectScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectScene() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectScene() {};

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

	// セレクト操作
	void ChangeSelect();

	// 画像のカラーを変える処理
	void ChangeSpriteColor();

private:

	// メインカメラ
	unique_ptr<Camera> camera_ = nullptr;


	// ゲームスタートラベル
	uint32_t gameStartTexHD_{};
	WorldTransform gameStartWt_{};
	unique_ptr<Sprite> gameStartSp_;

	// マニュアル
	uint32_t manualTexHD_{};
	WorldTransform manualWt_{};
	unique_ptr<Sprite> manualSp_;

	// バックアロー
	uint32_t backTexHD_{};
	WorldTransform backWt_{};
	unique_ptr<Sprite> backSp_;


	SelectSceneNowSelect nowSelect_ = toManualScene;


	// フェードのフラグ
	bool isFadeFunc_ = false;

	uint32_t SE1_;
	uint32_t SE2_;

	int flame_;

	EnemyManager enemyManager_;
};