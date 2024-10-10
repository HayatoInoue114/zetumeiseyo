#include "OverScene.h"



// 初期化処理
void OverScene::Initialize()
{
	// メインカメラ
	camera_ = make_unique<Camera>();
	camera_->Initialize();

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Initialize();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Initialize();

	/* ----- Fade フェード ----- */
	fadeTexHD_ = TextureManager::LoadTexture("", "Fade.png");
	fadeSprite_ = make_unique<Sprite>();
	fadeSprite_->Initialize({ 1280.0f, 720.0f });
	fadeSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.8f });
	fadeWt_.Initialize();

	/* ----- Over オーバー ----- */
	overTexHD_ = TextureManager::LoadTexture("OverScene", "Label_GameOver.png");
	overSp_ = make_unique<Sprite>();
	overSp_->Initialize({ 1280.0f, 720.0f });
	overWt_.Initialize();
	overWt_.translate.y = -140.0f;

	/* ----- PushA プッシュA ----- */
	pushATexHD_ = TextureManager::LoadTexture("TitleScene", "Label_TitlePushA.png");
	pushASp_ = make_unique<Sprite>();
	pushASp_->Initialize({ 1280.0f, 720.0f });
	pushAWt_.Initialize();
	pushAWt_.translate.x = 500.0f;
	pushAWt_.translate.y = 64.0f;

	/* ----- TotalLevelUI トータルレベルUI ----- */
	totalLevelUI_ = make_unique<TotalLevelUI>();
	totalLevelUI_->Initialize();
	//int totalLv = SaveValue::GetIntValue("Total_Level");

	/* ----- FadeManager フェードマネージャー ----- */
	FadeManager::Initialize(func_FadeOut);

	/* ----- Audio オーディオ----- */
	Audio::Initialize();
	gameBGM_ = Audio::LoadSound("BGM/BGM1.wav");
	Audio::PlayOnSound(gameBGM_, true, 0.1f);

	tugiheSE_ = Audio::LoadSound("SE/tugihe.wav");
}


// 更新処理
void OverScene::Update(GameManager* state)
{
	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Update();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Update();

	/* ----- GameCamera ゲームカメラ----- */
	camera_->UpdateMatrix();
	camera_->translate.z += 0.01f;
	if (camera_->translate.z >= 50.0f) {
		camera_->translate.z = -50.0f;
	}

	// フェードアウト
	FadeManager::IsFadeOut();

	/* ----- Fade フェード ----- */
	fadeWt_.UpdateMatrix();

	/* ----- Over オーバー ----- */
	overWt_.UpdateMatrix();

	/* ----- PushA プッシュA ----- */
	pushAWt_.UpdateMatrix();

	// ボタン押下でシーンチェンジ
	if (GamePadInput::TriggerButton(PadData::A) || KeysInput::TriggerKey(DIK_SPACE)) {

		isSceneChange_ = true;
		Audio::PlayOnSound(tugiheSE_, false, 0.5f);
	}

	if (isSceneChange_) {

		if (FadeManager::IsFadeIn()) {

			// ここでストップしておく
			Audio::StopOnSound(gameBGM_);
			state->ChangeSceneState(new SelectScene());
			return;
		}
	}


#ifdef _DEBUG

	ImGui::Begin("OverScene");
	ImGui::End();

#endif // _DEBUG
}


// 背景スプライトの描画処理
void OverScene::BackSpriteDraw()
{

}


// ３Dオブジェクトの描画処理
void OverScene::ModelDraw()
{
	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Draw(camera_.get());

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Draw(camera_.get());
}


// 前景スプライトの描画処理
void OverScene::FrontSpriteDraw()
{
	/* ----- Fade フェード ----- */
	fadeSprite_->Draw(fadeTexHD_, fadeWt_, camera_.get());

	/* ----- Over オーバー ----- */
	overSp_->Draw(overTexHD_, overWt_, camera_.get());

	/* ----- PushA プッシュA ----- */
	pushASp_->Draw(pushATexHD_, pushAWt_, camera_.get());

	/* ----- TotalLevelUI トータルレベルUI ----- */
	totalLevelUI_->Draw2DFront(camera_.get());

	/* ----- FadeManager フェードマネージャー ----- */
	FadeManager::Draw(camera_.get());
}