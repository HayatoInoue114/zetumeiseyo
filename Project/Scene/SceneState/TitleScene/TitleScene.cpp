#include "TitleScene.h"


TitleScene::~TitleScene() {
	Audio::StopOnSound(BGM_);
}

// 初期化処理
void TitleScene::Initialize()
{
	/* ----- GameCamera ゲームカメラ ----- */
	camera_ = make_unique<Camera>();
	camera_->Initialize();
	camera_->translate.z = 0.0f;
	camera_->translate.y = 10.0f;
	camera_->rotate.x = 0.2f;

	start_ = camera_->translate;

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Initialize();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Initialize();

	/* ----- TitleName タイトル ----- */
	titleTexHD_ = TextureManager::LoadTexture("TitleScene", "Label_Title2.png");
	titleSp_ = make_unique<Sprite>();
	titleSp_->Initialize({ 1280.0f, 720.0f });
	titleWt_.Initialize();
	titleSp_->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	/* ----- PushA プッシュA ----- */
	pushATexHD_ = TextureManager::LoadTexture("TitleScene", "Label_TitlePushA2.png");
	pushASp_ = make_unique<Sprite>();
	pushASp_->Initialize({ 1280.0f, 720.0f });
	pushAWt_.Initialize();
	pushASp_->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	/* ----- FadeManager フェードマネージャー ----- */
	FadeManager::Initialize(func_FadeIn);
	isFadeFunc_ = false;

	Audio::GetInstance()->Initialize();
	BGM_ = Audio::LoadSound("BGM/TitleBGM.wav");
	Audio::PlayOnSound(BGM_, true, 0.1f);

	SE_ = Audio::LoadSound("SE/kettei.wav");

	flame_ = 0;
	change_ = 0;

	isCameraInPosition_ = false;

	titleSp_->SetColor({ 1,1,1,0 });
	pushASp_->SetColor({ 1,1,1,0 });

	spriteColor_ = { 1,0,0,0 };

	fadeBlast_ = std::make_unique<Blast>();
	fadeBlast_->Initialize(camera_->GetWorldPos(), 1);
	fadeBlast_->isSpawn = true;
}


// 更新処理
void TitleScene::Update(GameManager* state)
{
	state;
	flame_++;
	Scope scope = { 200,300 };
	int dom = (int)RandomGenerator::getRandom(scope);
	if (flame_ % dom == 0) {

	}
	if (flame_ == 5) {
		enemyManager_.TitleSpawn(camera_.get());
	}
	

	enemyManager_.TitleUpdate();

	fadeBlastMove();
	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Update();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Update();

	/* ----- GameCamera ゲームカメラ----- */
	camera_->UpdateMatrix();
	CameraMove();

	SpriteMove();

	/* ----- TitleName タイトル ----- */
	titleWt_.UpdateMatrix();


	/* ----- PushA プッシュA ----- */
	pushAWt_.UpdateMatrix();


	// ボタン or キー 押下でフェード処理に入るフラグを立てる
	if (GamePadInput::TriggerButton(PadData::A) || 
		KeysInput::TriggerKey(DIK_A) || KeysInput::TriggerKey(DIK_SPACE)) {
		Audio::PlayOnSound(SE_, false, 0.3f);
		isFadeFunc_ = true;
	}
	// フェードの処理に入ってインしたらシーンチェンジ
	if (isFadeFunc_) {

		if (FadeManager::IsFadeIn()) {
			Audio::StopOnSound(BGM_);
			//state->ChangeSceneState(new SelectScene());
		}
	}

#ifdef _DEBUG

	ImGui::Begin("TitleScene");
	ImGui::Text("Scene change to 'SelectScene' by pressing A button.");
	ImGui::DragFloat3("rotate", &titleWt_.rotate.x, 0.1f);
	ImGui::End();

#endif // _DEBUG
}


// 背景スプライトの描画処理
void TitleScene::BackSpriteDraw()
{

}


// ３Dオブジェクトの描画処理
void TitleScene::ModelDraw()
{
	enemyManager_.Draw(camera_.get());

	fadeBlast_->Draw(camera_.get());

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Draw(camera_.get());

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Draw(camera_.get());
}


// 前景スプライトの描画処理
void TitleScene::FrontSpriteDraw()
{

	/* ----- TitleName タイトル ----- */
	titleSp_->Draw(titleTexHD_, titleWt_, camera_.get());


	/* ----- PushA プッシュA ----- */
	if (flame_ % 30 == 0) {
		change_++;
	}
	if (change_ % 2 == 0) {
		pushASp_->Draw(pushATexHD_, pushAWt_, camera_.get());
	}
	
	

	/* ----- FadeManager フェードマネージャー ----- */
	//FadeManager::Draw(camera_.get());
}

void TitleScene::CameraMove() {

	int count = 100;
	float y = 0.0003f;

	const float duration = static_cast<float>(count * 2);

	end_ = { 20,2,10 };

	if (flame_ <= count) {
		cameraRotateVel_.y += y;
		camera_->rotate.y -= cameraRotateVel_.y;
	}
	else if (flame_ <= count * 2) {
		cameraRotateVel_.y -= y;
		camera_->rotate.y -= cameraRotateVel_.y;
	}

	if (flame_ <= count * 2) {
		// 現在の進行度（0.0 ～ 1.0 の範囲）
		float t = static_cast<float>(flame_) / duration;

		// イージングを適用した進行度
		float easeT = easeOutCubic(t);

		camera_->translate = start_ + (end_ - start_) * easeT;
	}
	else {
		isCameraInPosition_ = true;
	}
}

void TitleScene::SpriteMove()
{
	//スプライトの透明度
	if(isCameraInPosition_ && spriteColor_.w <= 1.0f){
		spriteColor_.w += 0.01f;
	}

	titleSp_->SetColor(spriteColor_);
	pushASp_->SetColor(spriteColor_);
}

void TitleScene::fadeBlastMove()
{
	if (isFadeFunc_) {
		fadeBlast_->SetPos(camera_->translate);
	}

	ImGui::Begin("fade");
	ImGui::Text("%f", fadeBlast_->GetPos().x);
	ImGui::End();
}
