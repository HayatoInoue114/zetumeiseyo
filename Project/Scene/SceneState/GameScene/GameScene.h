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

#include "GameObject/Camera/FollowCamera/FollowCamera.h"
#include "Effect/Indicator/Indicator.h"

#include <chrono>


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

	/// <summary>
	/// マリオの死亡演出と同じようにするための関数
	/// </summary>
	void MarioSprite();

	void InitIndicators(int enemyCount);

	void InitEnemyPositions();

	void UpdateEnemyPositions();

	void UpdateIndicators();

	void Shake();

	void MorterShake();

	void DeltaTime();
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

	unique_ptr<FollowCamera> followCamera_ = nullptr;


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

	uint32_t blackTex_;
	unique_ptr<Sprite> blackSp_;
	WorldTransform blackSpriteWT_;

	float distanceFromPlayer = 5.0f;                  // プレイヤーからの距離
	float cameraAngle = 0.0f;                         // カメラの角度
	float rotationSpeed = 0.05f;                      // 回転速度（スティック入力に基づく）

	std::vector<std::unique_ptr<Indicator>> indicators;
	std::vector<Vector3> enemyPositions; // 敵の位置情報リスト

	float scopeRange_;
	bool isShake_ = false;
	float shakeDecrement_;

	float shakeDecayRate_ = 0.9f;           // 減衰率 (1秒間に減少する割合)
	float shakeThreshold_ = 0.01f;          // 停止判定の振動範囲
	float shakeFrequency_ = 10.0f;          // 振動の速さ（サイン波用）
	float shakeTime_ = 0.0f;                // 振動の経過時間（サイン波用）

	float deltaTime_;          // 経過時間（秒）
	std::chrono::steady_clock::time_point previousTime_; // 前回のフレーム時刻
private:
	//カメラシェイクの強度(2.0f)
	const float kShakeRange_ = 2.0f;
	//迫撃砲のシェイク強度(1.0f)
	const float kShakeRangeMorter_ = 1.0f;

	const Vector3 kDiffPos_ = { 0.0f, 10.0f, -60.0f };
};
