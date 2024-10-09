#include "SelectScene.h"



// 初期化処理
void SelectScene::Initialize()
{
	// メインカメラ
	camera_ = make_unique<Camera>();
	camera_->Initialize();
	camera_->translate.z = -50.0f;
	camera_->translate.y = 10.0f;
	

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Initialize();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Initialize();

	/* ----- Manual マニュアル ----- */
	manualTexHD_ = TextureManager::LoadTexture("SelectScene", "Label_Manual.png");
	manualSp_ = make_unique<Sprite>();
	manualSp_->Initialize({ 288.0f, 128.0f });
	manualSp_->SetSpriteOrigin(SpriteOrigin::Center);
	manualWt_.Initialize();
	manualWt_.scale = Vector3::one;
	manualWt_.translate = { WinApp::kHalfWindowWidth, WinApp::kHalfWindowHeight - 64.0f, 0.0f };


	/* ----- GameStart ゲームスタート ----- */
	gameStartTexHD_ = TextureManager::LoadTexture("SelectScene", "Label_GameStart.png");
	gameStartSp_ = make_unique<Sprite>();
	gameStartSp_->Initialize({ 288.0f, 128.0f });
	gameStartSp_->SetSpriteOrigin(SpriteOrigin::Center);
	gameStartWt_.Initialize();
	gameStartWt_.scale = Vector3::one;
	gameStartWt_.translate = { WinApp::kHalfWindowWidth, WinApp::kHalfWindowHeight + 64.0f, 0.0f };


	/* ----- BackArrow バックアロー  ----- */
	backTexHD_ = TextureManager::LoadTexture("SelectScene", "Label_BackArrowTitle.png");
	backSp_ = make_unique<Sprite>();
	backSp_->Initialize({ 256, 128.0f });
	backSp_->SetSpriteOrigin(SpriteOrigin::TopLeft);
	backWt_.Initialize();
	backWt_.scale = Vector3::one;
	backWt_.translate = { 0.0f, 592.0f, 0.0f };

	nowSelect_ = toManualScene;


	/* ----- FadeManager フェードマネージャー ----- */
	FadeManager::Initialize(func_FadeOut);
	isFadeFunc_ = false;

	Audio::GetInstance()->Initialize();
	SE1_ = Audio::LoadSound("SE/kettei.wav");
	SE2_ = Audio::LoadSound("SE/idou.wav");

	flame_ = 0;
}


// 更新処理
void SelectScene::Update(GameManager* state)
{

	flame_++;
	Scope scope = { 200,300 };
	int dom = (int)RandomGenerator::getRandom(scope);

	if (flame_ % dom == 0 || flame_ == 30) {
		enemyManager_.TitleSpawn(camera_.get());
	}

	enemyManager_.SelectUpdate();

	/* ----- GameCamera ゲームカメラ----- */
	camera_->UpdateMatrix();


	/* ----- GameStart ゲームスタート ----- */
	gameStartWt_.UpdateMatrix();


	/* ----- Manual マニュアル ----- */
	manualWt_.UpdateMatrix();


	/* ----- BackArrow バックアロー  ----- */
	backWt_.UpdateMatrix();

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Update();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Update();

	/* ----- GameCamera ゲームカメラ----- */
	/*camera_->UpdateMatrix();
	camera_->translate.z += 0.05f;
	if (camera_->translate.z >= 50.0f) {
		camera_->translate.z = -50.0f;
	}*/

	// フェードアウトしていなかったら早期return
	if (FadeManager::IsFadeOut()) {

		if (!isFadeFunc_) {

			// セレクト操作
			ChangeSelect();

			// 画像のカラーを変える処理
			ChangeSpriteColor();
		}
	}


	// ボタン or キー 押下でフェード処理に入るフラグを立てる
	if (GamePadInput::TriggerButton(PadData::A) ||
		KeysInput::TriggerKey(DIK_A) || KeysInput::TriggerKey(DIK_SPACE)) {
		Audio::PlayOnSound(SE1_, false, 0.3f);
		isFadeFunc_ = true;
	}
	// フェードの処理に入ってインしたらシーンチェンジ
	if (isFadeFunc_) {

		if (FadeManager::IsFadeIn()) {
			if (nowSelect_ == toManualScene) {
				state->ChangeSceneState(new ManualScene());
			}
			else if (nowSelect_ == toGameScene) {
				state->ChangeSceneState(new GameScene());
			}
			else if (nowSelect_ == toTitleScene) {
				state->ChangeSceneState(new TitleScene());
			}
		}
	}

#ifdef _DEBUG

	ImGui::Begin("SelectScene");
	ImGui::Text("Scene change to 'ManualScene' by pressing X button.");
	ImGui::Text("Scene change to 'GameScene' by pressing A button.");

	ImGui::Text("");
	if (nowSelect_ == toManualScene) {
		ImGui::Text("toManualScene");
	}
	else if (nowSelect_ == toGameScene) {
		ImGui::Text("toGameScene");
	}
	else if (nowSelect_ == toTitleScene) {
		ImGui::Text("toTitleScene");
	}
	ImGui::End();

#endif // _DEBUG
}


// 背景スプライトの描画処理
void SelectScene::BackSpriteDraw()
{

}


// ３Dオブジェクトの描画処理
void SelectScene::ModelDraw()
{
	enemyManager_.Draw(camera_.get());

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Draw(camera_.get());

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Draw(camera_.get());
}


// 前景スプライトの描画処理
void SelectScene::FrontSpriteDraw()
{
	/* ----- GameStart ゲームスタート ----- */
	gameStartSp_->Draw(gameStartTexHD_, gameStartWt_, camera_.get());


	/* ----- Manual マニュアル ----- */
	manualSp_->Draw(manualTexHD_, manualWt_, camera_.get());


	/* ----- BackArrow バックアロー  ----- */
	backSp_->Draw(backTexHD_, backWt_, camera_.get());


	/* ----- FadeManager フェードマネージャー ----- */
	FadeManager::Draw(camera_.get());
}


// セレクト操作
void SelectScene::ChangeSelect()
{
	// ボタン押下でnowSelectの変更
	if (GamePadInput::TriggerButton(PadData::UP) || KeysInput::TriggerKey(DIK_UP) || KeysInput::TriggerKey(DIK_W)) {
		Audio::PlayOnSound(SE2_, false, 0.3f);
		if (nowSelect_ == toManualScene) {
			nowSelect_ = toTitleScene;
		}
		else if (nowSelect_ == toGameScene) {
			nowSelect_ = toManualScene;
		}
		else if (nowSelect_ == toTitleScene) {
			nowSelect_ = toGameScene;
		}
	}
	else if (GamePadInput::TriggerButton(PadData::DOWN) || KeysInput::TriggerKey(DIK_DOWN) || KeysInput::TriggerKey(DIK_S)) {
		Audio::PlayOnSound(SE2_, false, 0.3f);
		if (nowSelect_ == toManualScene) {
			nowSelect_ = toGameScene;
		}
		else if (nowSelect_ == toGameScene) {
			nowSelect_ = toTitleScene;
		}
		else if (nowSelect_ == toTitleScene) {
			nowSelect_ = toManualScene;
		}
	}
}


// 画像のカラーを変える処理
void SelectScene::ChangeSpriteColor()
{
	// なにが選択されているかでスプライトの色を変える
	if (nowSelect_ == toManualScene) {
		manualSp_->SetColor(DefaultColor::Red);
		gameStartSp_->SetColor(DefaultColor::White);
		backSp_->SetColor(DefaultColor::White);
	}
	else if (nowSelect_ == toGameScene) {
		manualSp_->SetColor(DefaultColor::White);
		gameStartSp_->SetColor(DefaultColor::Red);
		backSp_->SetColor(DefaultColor::White);
	}
	else if (nowSelect_ == toTitleScene) {
		manualSp_->SetColor(DefaultColor::White);
		gameStartSp_->SetColor(DefaultColor::White);
		backSp_->SetColor(DefaultColor::Red);
	}
}
