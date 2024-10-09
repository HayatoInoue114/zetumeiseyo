#include "PlayerLevelUpPrope.h"
#include "Player/Player.h"


// 初期化処理
void PlayerLevelUpPrope::Initialize()
{
	// レベルの種類は 5 : 初期レベルは全部 1
	levels_.resize(5);
	for (int i = 0; i < levels_.size(); ++i) {
		levels_[i] = 1;
	}

	// ページは0でスタート
	nowPage_ = One;

	// プレイヤーの基本ステータス
	baseStatus_ = make_unique<PlayerBaseStatus>();

	// バックスクリーン
	backScreen_ = make_unique<BackScreen>();
	backScreen_->Initialize();

	// バックアロー
	backArrow_ = make_unique<BackArrow>();
	backArrow_->Initialize();

	// ページセレクト
	pageSelect_ = make_unique<PageSelect>();
	pageSelect_->Initialize();

	// ドローエネルギー
	drawEnergy_ = make_unique<DrawEnergy>();
	drawEnergy_->Initialize();

	// ドローコスト
	drawCostOne_ = make_unique<DrawCost>();
	drawCostOne_->Initialize(20);
	drawCostTwo_ = make_unique<DrawCost>();
	drawCostTwo_->Initialize(50);

	// セレクトフレーム
	selectFrame_ = make_unique<SelectFrame>();
	selectFrame_->Initialize();

	// レベルカウント
	levelCounts_.resize(levels_.size());
	for (int i = 0; i < levels_.size(); ++i) {
		Vector2 pos = { 312.0f, 212.0f + (i * 76.0f) };

		levelCounts_[i] = make_unique<LevelCount>();
		levelCounts_[i]->Initialize(i, pos);

		selectFrame_->levelVectors_push_back(pos);
	}

	// バレットラベル
	bulletLavel_.resize(3);
	for (int i = 0; i < 3; ++i) {
		Vector2 pos = { 312.0f, 224.0f + (i * 112.0f) };

		bulletLavel_[i] = make_unique<BulletLavel>();
		bulletLavel_[i]->Initialize(i, pos);

		selectFrame_->bulletVectors_push_back(pos);
	}

	// オーディオ
	selectSE_ = Audio::LoadSound("SE/idou.wav");
	decisionSE_ = Audio::LoadSound("SE/kettei.wav");
}


// 更新処理
void PlayerLevelUpPrope::Update()
{
	// ページの変更
	ChangePage();

	// 今どのページにいるかで通す処理を変える
	if (nowPage_ == DrawPage::One) {

		// レベルタイプの変更
		ChangeLevelType();

		// レベルアップ処理
		LevelUpFunc();
	}
	else if (nowPage_ == DrawPage::Two) {

		// バレットタイプの変更
		ChangeBulletType();

		// バレットタイプ処理
		BulletTypeFunc();
	}

#ifdef _DEBUG
	if (ImGui::TreeNode("PlayerLevelUpPrope")) {

		ImGui::Text("Page = %d", nowPage_);
		ImGui::Text("SelectLevelType = %d", selectLevelType_);
		ImGui::Text("HP  = %d", levels_[LevelPrope::HP]);
		ImGui::Text("Vel = %d", levels_[LevelPrope::Vel]);
		ImGui::Text("Att = %d", levels_[LevelPrope::Damage]);
		ImGui::Text("Int = %d", levels_[LevelPrope::Rate]);
		ImGui::Text("Das = %d", levels_[LevelPrope::Dash]);
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("SelectBulletType = %d", selectBulletType_);
		ImGui::Text("BulletType = %d", bulletType_);
		ImGui::Text("");
		ImGui::Text("Energy = %d", player_->GetEnergy());

		ImGui::TreePop();
	}
#endif // _DEBUG
}


// 描画処理
void PlayerLevelUpPrope::Draw2DBack(Camera* camera)
{
	if (player_->GetNowLevelUp()) {
		camera;
	}
}
void PlayerLevelUpPrope::Draw2DFront(Camera* camera)
{
	if (player_->GetNowLevelUp()) {

		// バックスクリーン
		backScreen_->Draw2DFront(camera);

		// バックアロー
		backArrow_->Draw2DFront(nowPage_, camera);

		// ページセレクト
		pageSelect_->Draw2DFront(nowPage_, camera);

		// ドローエネルギー
		drawEnergy_->Draw(player_->GetEnergy(), camera);

		// ドローコスト
		if (nowPage_ == One) {
			drawCostOne_->Draw(camera);
		}
		else if (nowPage_ == Two) {
			drawCostTwo_->Draw(camera);
		}

		// レベルカウント
		if (nowPage_ == DrawPage::One) {
			for (int i = 0; i < levels_.size(); ++i) {
				levelCounts_[i]->Draw2DFront(levels_[i], camera);
			}
		}

		// バレットラベル
		for (size_t i = 0; i < 3; ++i) {
			bulletLavel_[i]->Draw2DFront(nowPage_, camera);
		}

		// セレクトフレーム
		if (nowPage_ == DrawPage::One) {
			selectFrame_->Draw2DFront(nowPage_, selectLevelType_, camera);
		}
		else if (nowPage_ == DrawPage::Two) {
			selectFrame_->Draw2DFront(nowPage_, selectBulletType_, camera);
		}
	}
}


// レベルにあった値を返す
int PlayerLevelUpPrope::SetHP()
{
	return baseStatus_->LevelCheck_HP(levels_[LevelPrope::HP]);
}
float PlayerLevelUpPrope::SetVelocity()
{
	return baseStatus_->LevelCheck_Velocity(levels_[LevelPrope::Vel]);
}
int PlayerLevelUpPrope::SetAttack()
{
	return baseStatus_->LevelCheck_Damage(levels_[LevelPrope::Damage]);
}
int PlayerLevelUpPrope::SetAttackInterval()
{
	return baseStatus_->LevelCheck_Rate(levels_[LevelPrope::Rate], bulletType_);
}
float PlayerLevelUpPrope::SetDash()
{
	return baseStatus_->LevelCheck_Dash(levels_[LevelPrope::Dash]);
}
PlayerBulletType PlayerLevelUpPrope::SetBulletType()
{
	return bulletType_;
}


// トータルレベルの計算
void PlayerLevelUpPrope::CalcTotalLevel()
{
	totalLevel_ = 0;
	for (int i = 0; i < levels_.size(); ++i) {
		totalLevel_ += levels_[i];
	}
}


// ページの変更
void PlayerLevelUpPrope::ChangePage()
{
	// 左右ボタン or キーでページ変更
	if (GamePadInput::TriggerButton(PadData::RIGHT) || KeysInput::TriggerKey(DIK_RIGHT)) {

		if (nowPage_ == DrawPage::One) {
			nowPage_ = DrawPage::Two;
			Audio::PlayOnSound(selectSE_, false, 0.5f);
		}
		
	}
	if (GamePadInput::TriggerButton(PadData::LEFT) || KeysInput::TriggerKey(DIK_LEFT)) {

		if (nowPage_ == DrawPage::Two) {
			nowPage_ = DrawPage::One;
			Audio::PlayOnSound(selectSE_, false, 0.5f);
		}
	}
}


// レベルタイプの変更
void PlayerLevelUpPrope::ChangeLevelType()
{
	// 上下ボタン or キーで変更
	if (GamePadInput::TriggerButton(PadData::UP) || KeysInput::TriggerKey(DIK_UP)) {

		if (selectLevelType_ > 0) {
			selectLevelType_--;
			Audio::PlayOnSound(selectSE_, false, 0.5f);
		}
	}
	if (GamePadInput::TriggerButton(PadData::DOWN) || KeysInput::TriggerKey(DIK_DOWN)) {
		
		if (selectLevelType_ < 4) {
			selectLevelType_++;
			Audio::PlayOnSound(selectSE_, false, 0.5f);
		}
	}
}


// レベルアップ処理
void PlayerLevelUpPrope::LevelUpFunc()
{
	// Aボタン or キーでレベルアップ
	if (GamePadInput::TriggerButton(PadData::A) || KeysInput::TriggerKey(DIK_RETURN)) {

		// エネルギー残量チェック
		if (player_->GetEnergy() < costLevelUpEnergy_) {
			return; // 消費エネルギー以下しか所持していなかったら早期return
		}
		// レベルチェック
		if (levels_[selectLevelType_] >= 5) {
			return; // 選択しているタイプのレベルがLv.5を以上であれば早期return
		}

		// セレクトでレベルアップ対象を分ける
		switch (selectLevelType_)
		{
		case LevelPrope::HP:

			levels_[LevelPrope::HP]++;
			break;

		case LevelPrope::Vel:

			levels_[LevelPrope::Vel]++;
			break;

		case LevelPrope::Damage:

			levels_[LevelPrope::Damage]++;
			break;

		case LevelPrope::Rate:

			levels_[LevelPrope::Rate]++;
			break;

		case LevelPrope::Dash:

			levels_[LevelPrope::Dash]++;
			break;
		}

		// エネルギーを消費
		player_->SubEnergy(costLevelUpEnergy_);

		Audio::PlayOnSound(decisionSE_, false, 0.2f);

		for (int i = 0; i < levels_.size(); ++i) {
			totalLevel_ += levels_[i];
		}
	}
}


// バレットタイプの変更
void PlayerLevelUpPrope::ChangeBulletType()
{
	// Aボタン or キーでバレットタイプ変更
	if (GamePadInput::TriggerButton(PadData::A) || KeysInput::TriggerKey(DIK_RETURN)) {

		// エネルギー残量チェック
		if (player_->GetEnergy() < costBulletTypeEnergy_) {
			return; // 消費エネルギー以下しか所持していなかったら早期return
		}

		// セレクトでレベルアップ対象を分ける
		switch (selectBulletType_)
		{
		case PlayerBulletType::TypeNormal:

			bulletType_ = PlayerBulletType::TypeNormal;
			break;

		case PlayerBulletType::TypePenetration:

			bulletType_ = PlayerBulletType::TypePenetration;
			break;

		case PlayerBulletType::TypeShotgun:

			bulletType_ = PlayerBulletType::TypeShotgun;
			break;
		}

		// エネルギーを消費
		player_->SubEnergy(costBulletTypeEnergy_);

		Audio::PlayOnSound(decisionSE_, false, 0.2f);
	}
}


// バレットタイプ処理
void PlayerLevelUpPrope::BulletTypeFunc()
{
	// 上下ボタン or キーで変更
	if (GamePadInput::TriggerButton(PadData::UP) || KeysInput::TriggerKey(DIK_UP)) {

		if (selectBulletType_ > 0) {
			selectBulletType_--;
			Audio::PlayOnSound(selectSE_, false, 0.5f);
		}
	}
	if (GamePadInput::TriggerButton(PadData::DOWN) || KeysInput::TriggerKey(DIK_DOWN)) {

		if (selectBulletType_ < 2) {
			selectBulletType_++;
			Audio::PlayOnSound(selectSE_, false, 0.5f);
		}
	}
}
