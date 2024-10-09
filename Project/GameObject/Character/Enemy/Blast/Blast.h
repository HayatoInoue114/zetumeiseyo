#pragma once
#include "../IEnemy/IEnemy.h"
#include "OBBCollider.h"
#include "Effect/BlastParticle/BlastParticle.h"

class Blast : public IEnemy, public OBBCollider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player, Vector3 position, int level) override;

	void Initialize(Vector3 position, int level);

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
	static std::unique_ptr<Blast> Spawn(Player* player, Vector3 position, int level);

	WorldTransform& GetWorldTransform() { return worldTransform_; }

public:
#pragma region Collider 衝突判定

	// 衝突判定
	void OnCollision(uint32_t id) override;

	void SettingColliderBit();

	void SettingCollider();

	// コライダーのゲッター
	Vector3 GetOBBWorldPos() override { return worldTransform_.GetWorldPos(); }
	Vector3 GetSize() override { return this->size_; }
	Vector3 GetRotate() override { return this->worldTransform_.rotate; }

	void SetBlast(bool isBlast) { isBlast_ = isBlast; }
	void SetPos(Vector3 pos) { this->worldTransform_.translate = pos; }


	OBBCollider* GetOBBCollider() override { return this; }

	void OnCollisionWithPlayer(Player* player) override;
	void OnCollisionWithPlayerBullet(IPlayerBullet* bullet) override;
#pragma endregion 

private: ///メンバ関数///

	/// <summary>
	/// ユークリッド距離を計算する関数
	/// </summary>
	/// <param name="point1"></param>
	/// <param name="point2"></param>
	/// <returns></returns>
	float CalculateEuclideanDistance(const Vector3& point1, const Vector3& point2);

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
	/// 色を点滅させる関数
	/// </summary>
	void ColorChange();

	/// <summary>
	/// タイトルで点滅させる関数
	/// </summary>
	void TitleColorBlink();

	/// <summary>
	/// パラメーターをレベルによって変化させる関数
	/// </summary>
	void ParameterInitialize();

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

	/// <summary>
	/// 近づいて爆発する関数
	/// </summary>
	void BlastOnNearby();

	/// <summary>
	/// 爆発するカウントをする関数
	/// </summary>
	void BlastCount();

	/// <summary>
	/// 2つの点の間の角度を計算する関数
	/// </summary>
	/// <param name="current : 現在位置"></param>
	/// <param name="target : 角度を求めたい位置"></param>
	/// <returns></returns>
	float CalculateAngle(const Vector3& current, const Vector3& target);

	/// <summary>
	/// プレイヤーを発見したときのアニメーションを追いかける前に追加
	/// </summary>
	void DiscoverPlayer();

	/// <summary>
	/// 死んだときに追いかける処理
	/// </summary>
	void FeedMove();

	Vector3 Multiply(const Vector3& v, const Matrix4x4& m);

	float ToPlayerDot() { return Dot(player_->GetWorldPos(), worldTransform_.translate); }

	Vector3 Jump(const Vector3& initialPosition, float jumpHeight, float jumpDuration, float currentTime);

	Vector3 JumpScale(const Vector3& initialScale, float jumpHeight, float jumpDuration, float currentTime);

	void Shake(const Vector3& initialPosition, float jumpHeight, float jumpDuration, float currentTime);

	Vector3 calculateJumpScale(float currentTime, float jumpDuration);

	Vector3 calculateLandingScale(float currentTime, float jumpDuration, const Vector3& initialScale);
private: ///イージング///
	float easeInOutQuad(float startValue, float movement, float currentTime);

	Vector3 easeInOutQuad(Vector3 startValue, Vector3 movement, float currentTime);

	float easeInOutQuad(float x);

	float ToPlayerDis();

	double easeOutBounce(double x);

	double easeInBounce(double x);

	double easeInOutBounce(double x);

	double easeOutQuint(double x);

	double easeInCirc(double x);

	

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

	//モデルサイズ
	Vector3 size_{};

	//色
	Vector4 color_{};

	//色の透明度
	float colorW_{};

	//Playerとの距離
	Vector3 toPlayerDistance_{};

	//爆発するまでのカウント
	int blastCount_{};

	//爆発するフラグ
	bool isBlast_{};

	//
	Vector3 vel3_{};

	//色を点滅させる
	int count_{};

	//色のフラグ
	int isColor_{};

	//色の点滅範囲
	int colorBlinkRange_{};

	//爆発範囲
	Vector3 blastRange_ = { 1,1,1 };

	//Playerを見つけた時のアニメーションフラグ
	bool isDiscover_;

	//アニメーションのための
	float animationTime_;

	//元の位置
	Vector3 originalPos_;

	Vector3 originalScale_;

	Vector4 hitColor_;

	Parameter initParam_;

	std::unique_ptr<BlastParticle> blastParticle_;

	uint32_t particletex_;

	//追いかける強度
	float chaseIntensity_ = 0.04f;
	
	bool isBlastParticle_ = false;
private: ///メンバ定数///

	//曲がるまでの間隔
	const int kTurnInterval_ = 10;

	//爆発範囲
	const float kBlastRange_ = 4.0f;
};

