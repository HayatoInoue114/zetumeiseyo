#pragma once

#include "IScene.h"
#include "GameManager.h"
#include "GameObject.h"

#include "Skydome/Skydome.h"
#include "Ground/Ground.h"

#include "Player/Player.h"
#include "Player/PlayerBullet/IPlayerBullet.h"
#include "Player/PlayerBullet/NormalBullet/NormalBullet.h"

#include "CollisionManager.h"
#include "Enemy/Manual/Manual.h"
#include "Enemy/None/None.h"
#include "Enemy/EnemyManager/EnemyManager.h"
#include "LevelManager/LevelManager.h"

#include "FPSCounter/FPSCounter.h"
#include "GameTimeCount/GameTimeCount.h"
#include "FadeManager.h"
#include "SaveValue/SaveValue.h"



class ManualScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ManualScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ManualScene();

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
	void AddPlayerBullets(shared_ptr<IPlayerBullet> bullet) override { playerBullets_.push_back(bullet); }

public:

	/// <summary>
	/// ウェーブの初期化処理
	/// </summary>
	void WaveInit();

	/// <summary>
	/// ウェーブの更新処理
	/// </summary>
	void WaveUpdate();

	/// <summary>
	/// ウェーブの終了処理
	/// </summary>
	void WaveExit(GameManager* state);

private:

	/// <summary>
	/// コライダーの衝突判定
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// カメラ関連の更新処理
	/// </summary>
	void CameraUpdate();

	/// <summary>
	/// スタート時のカメラの演出
	/// </summary>
	void CameraStartMove();

	/// <summary>
	/// プレイヤーへのズーム処理
	/// </summary>
	void CameraZoomFunc();

	/// <summary>
	/// プレイヤー関連の更新処理
	/// </summary>
	void PlayerUpdate();

	/// <summary>
	/// エネミー関連の更新処理
	/// </summary>
	void EnemyUpdate();

	/// <summary>
	/// 値を保存しておく
	/// </summary>
	void SaveValue();

	/// <summary>
	/// レベルアップフェーズ
	/// </summary>
	void SetLevel();

	/// <summary>
	/// ボタン長押しでシーンチェンジの処理
	/// </summary>
	void ChangeScene(GameManager* state);
private:
	FPSCounter fps_;

	bool isSetLevel = false;
	bool preIsSetLevel = false;


	/* ----- Camera カメラ ----- */
	unique_ptr<Camera> camera_ = nullptr;
	Vector3 cameraInitPos_{};
	uint32_t cameraNowFrame_ = 0;
	uint32_t cameraEndFrame_ = 180;


	/* ----- GameWave ゲームウェーブ ----- */
	int waveCount_ = 0;
	int waveMax_ = 0;
	bool isWaveStart_ = 0;
	bool isSceneChange_ = 0;


	/* ----- Player プレイヤー ----- */
	unique_ptr<Player> player_ = nullptr;
	list<shared_ptr<IPlayerBullet>> playerBullets_{};


	/* ----- Enemy エネミー ----- */
	EnemyManager enemyManager_;
	std::list<IEnemy*>* enemies_{};
	LevelManager levelManager_;

	std::unique_ptr<Manual> manu;
	std::vector<std::shared_ptr<None>> none;


	/* ----- Fade フェード ----- */
	uint32_t fadeTexHD_ = 0;
	unique_ptr<Sprite> fadeSprite_ = nullptr;
	WorldTransform fadeWt_{};
	Vector4 fadeColor_ = { 0.0f, 0.0f, 0.0f, 0.8f };
	bool isFade_ = false;

	uint32_t aTexHD_ = 0;
	unique_ptr<Sprite> aSprite_ = nullptr;
	WorldTransform aWt_{};

	uint32_t bTexHD_ = 0;
	unique_ptr<Sprite> bSprite_ = nullptr;
	WorldTransform bWt_{};

	uint32_t cTexHD_ = 0;
	unique_ptr<Sprite> cSprite_ = nullptr;
	WorldTransform cWt_{};

	bool isTookEnergy;
	bool isAlreadyPowerUp;

	/* ----- GameTimeCount ゲームカウント ----- */
	unique_ptr<GameTimeCount> gameTimeCount_;


	/* ----- CollisionManager コリジョンマネージャー ----- */
	unique_ptr<CollisionManager> collisionManager_ = nullptr;


	/* ----- ParticleManager パーティクルマネージャー ----- */
	ParticleManager* particleManager_;


	/* ----- FadeManager フェードマネージャー ----- */
	bool isFadeFunc_ = false;

	/* ----- Audio オーディオ----- */
	uint32_t gameBGM_;
};
