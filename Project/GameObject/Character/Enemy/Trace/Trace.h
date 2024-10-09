#pragma once
#include "../IEnemy/IEnemy.h"
#include "OBBCollider.h"

class Trace : public IEnemy, public OBBCollider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player, Vector3 position, int level) override;

	/// <summary>
	/// 
	/// </summary>
	void ParameterInitialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="view"></param>
	void Draw(Camera* camera) override;

	

	/// <summary>
	///  新しいEnemyを生成する
	/// </summary>
	static std::unique_ptr<Trace> Spawn(Player* player,Vector3 position,int level);

	WorldTransform &GetWorldTransform() { return worldTransform_; }

	void SetLevel(int level) { level_ = level; }

	int GetLevel() { return level_; }

public:
#pragma region Collider 衝突判定

	// 衝突判定
	void OnCollision(uint32_t id) override;

	// コライダーのゲッター
	Vector3 GetOBBWorldPos() override { return worldTransform_.GetWorldPos(); }
	Vector3 GetSize() override { return this->size_; }
	Vector3 GetRotate() override { return this->worldTransform_.rotate; }

	OBBCollider* GetOBBCollider() override { return this; }


	void OnCollisionWithPlayer(Player* player) override;
	void OnCollisionWithPlayerBullet(IPlayerBullet* bullet) override;
#pragma endregion 

private:
	// Colliderのビットのセッティング
	void SettingColliderBit();

	// OBBColliderのセッティング
	void SettingCollider();


private: ///メンバ関数///

	/// <summary>
	/// ユークリッド距離を計算する関数
	/// </summary>
	/// <param name="point1"></param>
	/// <param name="point2"></param>
	/// <returns></returns>
	float CalculateEuclideanDistance(const Vector3& point1, const Vector3& point2);

	/// <summary>
	/// 死んだときの処理
	/// </summary>
	void OnDeath();

	/// <summary>
	/// 索敵範囲を計算する関数
	/// </summary>
	void CalculateDetectionRange();

	/// <summary>
	/// 追いかけるときの関数(isTrace_ == trueなら)
	/// </summary>
	void Chace();

	/// <summary>
	/// 追いかけていないときの関数(isTrace_ == falseなら)
	/// </summary>
	void Wander();

	/// <summary>
	/// 正面へのベクトルをとる関数
	/// </summary>
	Vector3 GetFrontVector();

	/// <summary>
	/// 移動先座標を計算する関数(現在位置から半径以内に生成)
	/// </summary>
	/// <param name="Vector3 position : 現在位置"></param>
	/// <param name="float radius : 移動先を生成する範囲(半径)"></param>
	/// <returns></returns>
	Vector3 CalculateNextPosition(const Vector3& position, float radius);

	/// <summary>
	/// 現在位置から移動先にイージングで動く関数
	/// </summary>
	/// <param name="Vector3 currentPosition : 現在位置"></param>
	/// <param name="Vector3 targetPosition : 移動先位置"></param>
	/// <param name="float easingFactor : t"></param>
	/// <returns></returns>
	Vector3 MoveWithEasing(const Vector3& currentPosition, const Vector3& targetPosition, float t);

	float easeInOutQuad(float startValue, float movement, float currentTime);

	Vector3 easeInOutQuad(Vector3 startValue, Vector3 movement, float currentTime);

	float easeInOutQuad(float x);

	double easeOutBounce(double x);

	double easeInBounce(double x);

	double easeInOutBounce(double x);

	float CalculateAngle(const Vector3& current, const Vector3& target);

	Vector3 Multiply(const Vector3& v, const Matrix4x4& m);

private: ///メンバ変数///

	//Slerpで使う回転軸
	float velocityXZ_{};

	//追いかけているか
	bool isTrace_{};

	// 直進カウンタ(毎フレーム--)
	int cntAhead_{};

	//半径
	float rad_{};

	//移動のイージング用の勇気のt
	float t_{};

	//動いているか
	bool isMove_{};

	//フレーム数(毎フレームカウント)
	int flame_{};

	//仮の移動先
	Vector3 tmpPos_{};

	Vector3 size_{};

	Vector3 vel3_{};

private: ///メンバ定数///

	//索敵範囲
	const float kSearchRange_ = 20;

	//追いかける強度
	const float kChaseIntensity_ = 0.04f;

	//曲がるまでの間隔
	const int kTurnInterval_ = 10;

};

