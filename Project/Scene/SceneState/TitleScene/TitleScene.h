#pragma once

#include "IScene.h"
#include "GameManager.h"
#include "GameObject.h"
#include <memory>
#include "Sprite.h"
#include "FadeManager.h"
#include "Enemy/EnemyManager/EnemyManager.h"


class TitleScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();
	
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

	/// <summary>
	/// カメラの動き
	/// </summary>
	void CameraMove();

	void SpriteMove();

private:

	// メインカメラ
	unique_ptr<Camera> camera_ = nullptr;


	// タイトルラベル
	uint32_t titleTexHD_{};
	WorldTransform titleWt_{};
	unique_ptr<Sprite> titleSp_;

	// プッシュAラベル
	uint32_t pushATexHD_{};
	WorldTransform pushAWt_{};
	unique_ptr<Sprite> pushASp_;

	// フェードのフラグ
	bool isFadeFunc_ = false;

	int flame_;

	EnemyManager enemyManager_;

	uint32_t BGM_;
	uint32_t SE_;

	int change_{};

	Vector3 cameraVel_{};
	Vector3 cameraRotateVel_{};

	Vector3 start_{};
	Vector3 end_{};

	bool isCameraInPosition_;

	Vector4 spriteColor_{};
};
