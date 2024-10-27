#pragma once

#include "IScene.h"
#include "GameManager.h"
#include "GameObject.h"

#include "Skydome/Skydome.h"
#include "Ground/Ground.h"
#include "Field/Field.h"

#include "Player/Player.h"
#include "Player/PlayerBullet/IPlayerBullet.h"
#include "Player/PlayerBullet/NormalBullet/NormalBullet.h"

#include "CollisionManager.h"
#include "Enemy/Trace/Trace.h"
#include "Enemy/EnemyManager/EnemyManager.h"
#include "LevelManager/LevelManager.h"

#include "FPSCounter/FPSCounter.h"
#include "GameTimeCount/GameTimeCount.h"
#include "WaveCountUI/WaveCountUI.h"
#include "FadeManager.h"
#include "SaveValue/SaveValue.h"



class GameScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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
	void AddPlayerBullets(shared_ptr<IPlayerBullet> bullet) override{ playerBullets_.push_back(bullet); }

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
	void WaveExit();

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
	/// スタート演出
	/// </summary>
	void StartTexture();

	/// <summary>
	/// プレイヤー死亡時のカメラ演出
	/// </summary>
	void PlayerDieCmaera();

	/// <summary>
	/// プレイヤーに追従するカメラ
	/// </summary>
	void PlayerCamera();

private:
	FPSCounter fps_;


	/* ----- Camera カメラ ----- */
	unique_ptr<Camera> camera_ = nullptr;
	Vector3 cameraInitPos_{};
	Vector3 cameraDiffRotate_{};
	Vector3 cameraDiffPos_{};
	bool startCameraAnimIsFinish_ = false;
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
	EnemyManager enemyManager_{};
	std::list<IEnemy*>* enemies_{};
	LevelManager levelManager_{};
	bool isEnemyDraw_ = 0;

	/* ----- Fade フェード ----- */
	uint32_t fadeTexHD_ = 0;
	unique_ptr<Sprite> fadeSprite_ = nullptr;
	WorldTransform fadeWt_{};
	Vector4 fadeColor_ = { 0.0f, 0.0f, 0.0f, 0.8f };
	bool isFade_ = false;


	/* ----- GameTimeCount ゲームカウント ----- */
	unique_ptr<GameTimeCount> gameTimeCount_;

	/* ----- WaveCountUI ウェーブカウント ----- */
	unique_ptr<WaveCountUI> waveCountUI_;


	/* ----- CollisionManager コリジョンマネージャー ----- */
	unique_ptr<CollisionManager> collisionManager_ = nullptr;


	/* ----- ParticleManager パーティクルマネージャー ----- */
	ParticleManager* particleManager_;


	/* ----- FadeManager フェードマネージャー ----- */
	bool isFadeFunc_ = false;


	/* ----- Audio オーディオ----- */
	uint32_t gameBGM_;
	uint32_t decisionSE_;

	/* ----- UI ----- */
	uint32_t goTex_;
	unique_ptr<Sprite> goSp_;
	WorldTransform goWT_{};

	uint32_t readyTex_;
	unique_ptr<Sprite> readySp_;
	WorldTransform readyWT_{};

	float startAnimTime_;
	float nowFrame_;
	float endFrame_;
	float animTime_;
	Vector4 goColor_;

	//プレイヤーが死んだとき用
	float pDieNowFrame_;
	float pDieEndFrame_;
	float pDieAnimTime_;

	WorldTransform pDieWT_;

	uint32_t pDieTex_;
	unique_ptr<Sprite> pDieSp_;
	WorldTransform pDieSpriteWT_;
};
