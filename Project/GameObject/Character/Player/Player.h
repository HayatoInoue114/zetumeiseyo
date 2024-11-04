#pragma once

#include "GameObject.h"
#include "PlayerBaseStatus/PlayerBaseStatus.h"
#include "PlayerReticle/PlayerReticle.h"
#include "PlayerBullet/IPlayerBullet.h"
#include "PlayerBullet/NormalBullet/NormalBullet.h"
#include "PlayerBullet/PenetrationBullet/PenetrationBullet.h"
#include "PlayerBullet/Shotgun/Shotgun.h"
#include "Player/PlayerLevelUpPrope/PlayerLevelUpPrope.h"
#include "Effect/DeadFuncParticle/DeadFuncParticle.h"
#include "DrawUI/OperationUI/OperationUI.h"
#include "DrawUI/HpBar/HpBar.h"
#include "PlayerSound/PlayerSound.h"

#include "NormalParticle/NormalParticle.h"

//#include "PlayerDashParticle/PlayerDashParticle.h"

#include "OBBCollider.h"

/* GameSceneの前方宣言 */
class GameScene;
class IScene;

/* Enemyの前方宣言 */
class IEnemy;

/* Playerクラス */
class Player : public OBBCollider {

public: // プレイヤー本体 : メンバ関数

	// コンストラクタとデストラクタ
	Player() {};
	~Player() {};

	// 初期化処理　更新処理　描画処理
	void Initialize();
	void Update(Camera* camera);
	void Draw3D(Camera* camera);
	void Draw2DBack(Camera* camera);
	void Draw2DFront(Camera* camera);


	// レベルの変更処理
	void ChangeLevelPrope();

	// レベルチェック
	void LevelCheck();

#pragma region Get 取得

	// ワールド座標の取得
	Vector3 GetWorldPos() { return this->bodyWt_.GetWorldPos(); }

	// レベルアップの処理に入ってるかのフラグ
	bool GetNowLevelUp() const { return this->nowLevelUp_; }

	// エネルギー量の取得
	int GetEnergy() const { return this->energy_; }

	// 死亡フラグの取得
	bool GetIsDead() const { return this->isDead_; }

	// 死亡演出終了フラグの取得
	bool GetDeadFuncIsFinish() const { return this->deadFuncFinish_; }

	int GetLevelPrope() const { return this->levelPrope_; }
	PlayerBulletType GetPlayerBulletType() const { return this->bulletType_; }

	// Hpの取得
	int GetHp() const { return this->hp_; }

	// HpMaxの取得
	int GetHpMax() const { return this->hpMax_; }

#pragma endregion

#pragma region Set 設定

	// エネルギーの設定
	void AddEnergy(int add) { this->energy_ += add; }
	void SubEnergy(int sub) { this->energy_ -= sub; }
	void SetEnergy(int sub) { this->energy_ = sub; }

	// レベルアップの処理に入ってるかのフラグ
	void SetNowLevelUp(bool setState) { this->nowLevelUp_ = setState; }

	// バレットの登録先のシーン
	void SetGameScene(IScene* scene) { this->registerScene_ = scene; }

	// 射撃のインターバルの設定
	void SetAttackInterval(uint32_t newInterval) { this->AttackTimer_ = newInterval; }

	//hp減らす関数
	void SetDamage(int damage) { hp_ = hp_ - damage; }

	//コンテストのために一時的に作った関数
	void SetHP(int hp) { hp_ = hp; }
#pragma endregion

#pragma region Collider 衝突判定

	// 衝突判定
	void OnCollision(uint32_t id) override;

	// エネミーとの個別衝突判定
	void OnCollisionWithEnemy(IEnemy* enemy);
	void OnCollisionWithEnemyBullet();

	// コライダーのゲッター
	Vector3 GetOBBWorldPos() override { return bodyWt_.GetWorldPos(); }
	Vector3 GetSize() override { return this->size_; }
	Vector3 GetRotate() override { return this->bodyWt_.rotate; }

#pragma endregion 

private:

	// Colliderのビットのセッティング
	void SettingColliderBit();

	// OBBColliderのセッティング
	void SettingCollider();

	// 移動処理
	void Move();

	// プレイヤー本体の姿勢処理
	void CalcBodyRotate();

	// ダッシュの処理
	void DashFunc();

	// ダッシュ先の座標の計算
	Vector3 CalcToDashPos();

	// HPチェック
	void HPCheck();

	// 死亡演出
	void DeadFunc();

	// 震える処理
	void TrembleFunc();

	// 出血処理
	void BleedingFunc();

	// 衝突時無敵時間処理
	void InvincibleTime();


private: // プレイヤー本体 : メンバ変数

	// 本体
	unique_ptr<Model> bodyModel_ = nullptr;
	WorldTransform bodyWt_{};

	// 移動速度
	Vector3 velocity_{};
	float moveVector_;

	// サイズ
	Vector3 size_{};

	// 移動先のベクトル
	Vector2 moveV_{};

	// カラー
	Vector4 color_ = Vector4::one;

	// 死亡フラグ
	bool isDead_;

	// エネルギー
	int energy_;

	// LevelPrope
	int levelPrope_;

	// HP
	int hp_;
	int hpMax_;

	// Vel

	// Attack
	int attack_;

	// AttackInterval

	// DashLevel
	float toDash_;

	// totalLevel
	int totalLevel_;

	bool nowLevelUp_;

	// プレイヤーの基本ステータス
	unique_ptr<PlayerBaseStatus> baseStatus_;


	uint32_t deadFuncTex_ = 0;
	unique_ptr<DeadFuncParticle> deadFuncParticle_;

	uint32_t particletex_;


	// 死亡演出に使うフレーム
	uint32_t deadNowFrame_ = 0;
	uint32_t deadEndFrame_ = 300;
	bool deadFuncFinish_;

	// 死亡時の座標
	Vector3 deadPos_{};

	// 震える処理に使うフレーム
	uint32_t trembleNowFrame_ = 0;
	uint32_t trembleEndFrame_ = 60;
	bool trembleFuncFinish_ = false;

	// 無敵時間
	uint32_t invincibleNowFrame_ = 0;
	uint32_t invincibleEndFrame_ = 60;
	bool isInvincible_ = false;

#pragma region Dash

	// ダッシュしているかのフラグ
	bool isDash_;

	// ダッシュ先の座標
	Vector3 toDashPosition_{};
	Vector3 toInitDashPosition_{};

	// ダッシュに必要なフレーム
	int ASDashFrame_;

	// ダッシュにかかってるフレーム
	int nowDashFrame_;

#pragma endregion 


private:

	// 射撃処理
	void IsAttack();

	// バレットの設定
	void SettingNewBullet();

	// バレットの進行方向の計算
	Vector3 CalcBulletDirection();

	Vector3 CalculateRotation(const Vector3& direction);
	void LookAt(WorldTransform& enemyTransform, const Vector3& playerPosition);

private: // バレット : メンバ変数

	// Bullet
	unique_ptr<Model> bulletModel_;



	// バレットの速度
	float bulletVelocity_;

	// 射撃のインターバル
	int AttackInterval_;
	int AttackTimer_;

	// バレットのタイプ
	PlayerBulletType bulletType_;

	// 散弾の弾数
	int pelletNum_;


private: // レティクル : メンバ変数

	// Reticle
	unique_ptr<PlayerReticle> reticle_;



private: // シーン : メンバ変数

	// 登録先シーン
	IScene* registerScene_ = nullptr;


private: // プレイヤーLevelUpPrope :: メンバ変数

	// LevelUpPrope
	unique_ptr<PlayerLevelUpPrope> levelUpPrope_;


public:

	int GetLevel(LevelPrope prope) { return this->levelUpPrope_->GetLevel(prope); }
	int GetTotalLevel() { return this->levelUpPrope_->GetTotalLevel(); }

private: // プレイヤー操作UI :: メンバ変数

	// OperationUI
	unique_ptr<OperationUI> operationUI_;


private: // プレイヤーHpBar :: メンバ変数

	// HpBar
	unique_ptr<HpBar> hpBar_;


private: // ダッシュパーティクル :: メンバ変数

	// PlayerDashParticle
	//unique_ptr<PlayerDashParticle> dashParticle_;
	uint32_t dashParticleTexHD_;


private: // プレイヤーサウンド :: メンバ変数

	// PlayerSound
	unique_ptr<PlayerSound> sound_;
};

