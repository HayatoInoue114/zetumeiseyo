#pragma once

#include "GameObject.h"
#include "Player/PlayerBullet/IPlayerBullet.h"
#include "Player/PlayerBaseStatus/PlayerBaseStatus.h"
#include "Player/DrawUI/BackScreen/BackScreen.h"
#include "Player/DrawUI/BackArrow/BackArrow.h"
#include "Player/DrawUI/PageSelect/PageSelect.h"
#include "Player/DrawUI/LevelCount/LevelCount.h"
#include "Player/DrawUI/SelectFrame/SelectFrame.h"
#include "Player/DrawUI/DrawEnergy/DrawEnergy.h"
#include "Player/DrawUI/DrawCost/DrawCost.h"
#include "Player/DrawUI/BulletLavel/BulletLavel.h"
#include <vector>
#include <cstdint>
#include <iostream>
#include <string>
#include <map>


enum DrawPage {
	One,
	Two,
};


// Playerの前方宣言
class Player;


class PlayerLevelUpPrope{

public:

	// コンストラクタ、デストラクタ
	PlayerLevelUpPrope() {};
	~PlayerLevelUpPrope() {};

	// 初期化処理、更新処理、描画処理
	void Initialize();
	void Update();
	void Draw2DBack(Camera* camera);
	void Draw2DFront(Camera* camera);

	// レベルにあった値を返す
	int SetHP();
	float SetVelocity();
	int SetAttack();
	int SetAttackInterval();
	float SetDash();
	PlayerBulletType SetBulletType();

	// トータルレベルの計算
	void CalcTotalLevel();

#pragma region Get

	int GetLevel(LevelPrope prope) { return this->levels_[prope]; }
	int GetTotalLevel() { return this->totalLevel_; }

#pragma endregion 

#pragma region Set

	void SetPlayer(Player* setPlayer) { this->player_ = setPlayer; }

#pragma endregion 


private:

	// ページの変更
	void ChangePage();

	// レベルタイプの変更
	void ChangeLevelType();

	// レベルアップ処理
	void LevelUpFunc();

	// バレットタイプの変更
	void ChangeBulletType();

	// バレットタイプ処理
	void BulletTypeFunc();


private:

	// Player
	Player* player_ = nullptr;

	// プレイヤーの基本ステータス
	unique_ptr<PlayerBaseStatus> baseStatus_;

	// バックスクリーン
	unique_ptr<BackScreen> backScreen_;

	// バックアロー
	unique_ptr<BackArrow> backArrow_;

	// ページセレクト
	unique_ptr<PageSelect> pageSelect_;

	// ドローエネルギー
	unique_ptr<DrawEnergy> drawEnergy_;

	// ドローコスト
	unique_ptr<DrawCost> drawCostOne_;
	unique_ptr<DrawCost> drawCostTwo_;

	// セレクトフレーム
	unique_ptr<SelectFrame> selectFrame_;

	// レベルカウント
	vector<unique_ptr<LevelCount>> levelCounts_;

	// バレットラベル
	vector<unique_ptr<BulletLavel>> bulletLavel_;


	// ページ
	DrawPage nowPage_ = One;


	// レベル
	vector<int>levels_;

	// 今選択しているレベルアップタイプ
	int selectLevelType_ = 0;

	// レベルアップに必要なエネルギー消費量
	int costLevelUpEnergy_ = 20;

	// トータルレベル
	int totalLevel_ = 0;


	// バレットタイプ
	PlayerBulletType bulletType_;

	// 今選択しているバレットのタイプ
	int selectBulletType_ = 0;

	// バレットタイプ変更に必要なエネルギー
	int costBulletTypeEnergy_ = 50;


	// Audio SE
	uint32_t selectSE_ = 0;
	uint32_t decisionSE_ = 0;
};

