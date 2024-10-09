#include "GameTimeCount.h"



// 初期化処理
void GameTimeCount::Initialize()
{
	// ゲームの制限時間 45秒
	gameTime_ = 30 * 60;
	gameFrame_ = gameTime_;
	nowGameTimer_ = ConvertToSecond();


	/* ----- Clock 時計 ----- */
	clockTex_ = TextureManager::LoadTexture("GameScene/UI/Clock", "Clock.png");
	clockSp_ = make_unique<Sprite>();
	clockSp_->Initialize({ 88.0f, 88.0f });
	clockSp_->SetSpriteOrigin(SpriteOrigin::Center);
	clockWt_.Initialize();
	clockWt_.translate.x = WinApp::kHalfWindowWidth;
	clockWt_.translate.y = 88.0f / 2.0f;


	/* ----- ClockHand 秒針 ----- */
	clockHandTex_ = TextureManager::LoadTexture("GameScene/UI/Clock", "ClockHand.png");
	clockHandSp_ = make_unique<Sprite>();
	clockHandSp_->Initialize({ 88.0f, 88.0f });
	clockHandSp_->SetSpriteOrigin(SpriteOrigin::Center);
	clockHandWt_.Initialize();
	clockHandWt_.translate.x = WinApp::kHalfWindowWidth;
	clockHandWt_.translate.y = 88.0f / 2.0f;

	// 秒針の等速前進のフレームの設定
	advanceNowFrame_ = 0;
	advanceEndFrame_ = float(gameFrame_);


	/* ----- TimeBar タイムバー ----- */
	timeBarTex_ = TextureManager::LoadTexture("GameScene/UI/Clock", "TimeBar.png");
	timeBarInitSize_ = { 880.0f, 128.0f }; // タイムバーの初期サイズ
	timeBarSize_ = timeBarInitSize_; // タイムバーのサイズ
	timeBarSp_ = make_unique<Sprite>();
	timeBarSp_->Initialize(timeBarSize_);
	timeBarSp_->SetSpriteOrigin(SpriteOrigin::Center);
	timeBarWt_.Initialize();
	timeBarWt_.translate.x = WinApp::kHalfWindowWidth;
	timeBarWt_.translate.y = 88.0f / 2.0f;

	// タイムバーの等速縮小のフレームの設定
	reductNowFrame_ = 0;
	reductEndFrame_ = float(gameFrame_);

	// 一回通す
	clockWt_.UpdateMatrix();
	clockHandWt_.UpdateMatrix();
	timeBarWt_.UpdateMatrix();


	/* ----- Back バック ----- */
	backTex_ = TextureManager::LoadTexture("GameScene/UI", "timeBack.png");
	backSp_ = make_unique<Sprite>();
	backSp_->Initialize({ 1280.0f, 720.0f });
	backSp_->SetSpriteOrigin(SpriteOrigin::Center);
	backSp_->SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });
	backWt_.Initialize();
	backWt_.translate = {
		WinApp::kHalfWindowWidth,
		WinApp::kHalfWindowHeight,
		0.0f,
	};
}


// 更新処理
void GameTimeCount::Update()
{
	
	/* ----- Clock 時計 ----- */
	clockWt_.UpdateMatrix();

	/* ----- ClockHand 秒針 ----- */
	clockHandWt_.UpdateMatrix();

	/* ----- TimeBar タイムバー ----- */
	timeBarWt_.UpdateMatrix();
	timeBarSp_->SetSize(timeBarSize_);

	/* ----- Back バック ----- */
	backWt_.UpdateMatrix();

	// 制限時間の減算処理
	SubTimer();

	// 秒針の回転の処理
	AdvanceClockHand();

	// タイムバーの縮小の処理
	ReductTimeBarSizeX();


#ifdef _DEBUG

	if (ImGui::TreeNode("GameTimeCount")) {

		ImGui::Text("NowFrame = %d", gameFrame_);
		ImGui::Text("NowGameTime = %d", nowGameTimer_);
		ImGui::Text("");


		ImGui::Text("ClockHand");
		ImGui::DragFloat3("rotate", &clockHandWt_.rotate.x, 0.01f);
		ImGui::Text("");

		ImGui::Text("TimeBar");
		ImGui::DragFloat2("Size", &timeBarSize_.x, 1.0f);

		ImGui::TreePop();
	}

#endif // _DEBUG
}


// 描画処理
void GameTimeCount::DrawFront2D(Camera* camera)
{
	/* ----- Back バック ----- */
	backSp_->Draw(backTex_, backWt_, camera);

	/* ----- TimeBar タイムバー ----- */  // 時計のスプライトの奥に描画 
	timeBarSp_->Draw(timeBarTex_, timeBarWt_, camera);

	/* ----- Clock 時計 ----- */
	clockSp_->Draw(clockTex_, clockWt_, camera);

	/* ----- ClockHand 秒針 ----- */
	clockHandSp_->Draw(clockHandTex_, clockHandWt_, camera);
}


// タイマーを減らす
void GameTimeCount::SubTimer()
{
	// 制限時間が残ってるとき
	if (!isTimeUp_) {

		// フレームをデクリメント
		gameFrame_--;
		nowGameTimer_ = ConvertToSecond();

		// 秒針の前進に使うフレームもインクリメント
		advanceNowFrame_++;

		// タイムバーの縮小に使うフレームもインクリメント
		reductNowFrame_++;
	}

	// 制限時間が0になったら
	if (gameFrame_ <= 0) {

		// 0で固定しとく
		nowGameTimer_ = 0;
		gameFrame_ = 0;

		// 制限時間切れのフラグを立てる
		isTimeUp_ = true;
	}
}


// 時間の追加処理
void GameTimeCount::AddTimer(uint32_t addTime)
{
	// 秒をフレームに変換して入れる
	gameFrame_ += addTime * 60;
}


// 制限時間をリセットする
void GameTimeCount::ReSetTimer()
{
	// 時間切れの時にのみ通る処理
	if (isTimeUp_) {

		// 制限時間のリセット 45秒
		gameFrame_ = gameTime_;
		nowGameTimer_ = ConvertToSecond();

		// 秒針のアニメーションフレームも初期化
		advanceNowFrame_ = 0;

		// 秒針の回転も0に戻しておく
		clockHandWt_.rotate = Vector3::zero;

		// タイムバーのアニメーションフレームも初期化
		reductNowFrame_ = 0;

		// タイムバーのサイズも戻しておく
		timeBarSize_ = timeBarInitSize_;

		// 時間切れのフラグを折る
		isTimeUp_ = false;
	}
}


// 秒針を回転させる
void GameTimeCount::AdvanceClockHand()
{
	// 等速で秒針を回転させる
	clockHandWt_.rotate.z =
		Lerp(0.0f, endClockHandRotateZ_, (advanceNowFrame_ / advanceEndFrame_));
}


// タイムバーを縮小させる
void GameTimeCount::ReductTimeBarSizeX()
{
	// 等速で秒針を回転させる
	timeBarSize_.x = 
		Lerp(timeBarInitSize_.x, endTimeBarSizeX_, (reductNowFrame_ / reductEndFrame_));
}


// フレームを秒に変換する
uint32_t GameTimeCount::ConvertToSecond()
{
	return nowGameTimer_ = (gameFrame_ + 60) / 60;
}

