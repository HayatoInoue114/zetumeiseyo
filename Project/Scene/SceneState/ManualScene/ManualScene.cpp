 #include "ManualScene.h"



/// <summary>
/// コンストラクタ
/// </summary>
ManualScene::ManualScene() {

	particleManager_ = ParticleManager::GetInstance();
}


/// <summary>
/// デストラクタ
/// </summary>
ManualScene::~ManualScene()
{
	Audio::StopOnSound(gameBGM_);
}


/// <summary>
/// 初期化処理
/// </summary>
void ManualScene::Initialize() {

	/* ----- SaveValeu 値の保存クラス ----- */
	SaveValue::Clear();


	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Initialize();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Initialize();


	/* ----- Camera カメラ ----- */
	camera_ = make_unique<Camera>();
	camera_->Initialize();
	cameraInitPos_ = { 0.0f, 30.0f, -100.0f };
	camera_->translate = cameraInitPos_;
	camera_->rotate.x = 0.2f;
	camera_->UpdateMatrix();


	/* ----- Player プレイヤー ----- */
	player_ = make_unique<Player>();
	player_->Initialize();
	player_->SetGameScene(this);


	/* ----- GameWave ゲームウェーブ ----- */
	waveCount_ = 4;
	waveMax_ = 6;
	isWaveStart_ = true;
	isSceneChange_ = false;


	/* ----- Enemy 敵 ----- */
	enemyManager_.SetGameScene(this);
	enemies_ = enemyManager_.GetEnemy();

	Vector3 pos = player_.get()->GetWorldPos();
	pos.z = 40.0f;

	manu = std::make_unique<Manual>();
	manu->Initialize(player_.get(), pos,0 );
	for (int i = 0; i < 5; i++) {
		Vector3 posa = { i * 5.0f - 10.0f,0.0f,5.0f };
		std::shared_ptr<None> n;
		n = std::make_unique<None>();
		n->Initialize(player_.get(), posa, 0);
		none.push_back(n);
	}


	/* ----- Fade フェード ----- */
	fadeTexHD_ = TextureManager::LoadTexture("", "Fade.png");
	fadeSprite_ = make_unique<Sprite>();
	fadeSprite_->Initialize({ 1280, 720 }, fadeColor_);
	fadeWt_.Initialize();
	isFade_ = false;

	//
	aTexHD_ = TextureManager::LoadTexture("ManualScene", "a.png");
	aSprite_ = make_unique<Sprite>();
	aSprite_->Initialize({ 1280, 720 }, {1.0f,1.0f,1.0f,1.0f});
	aWt_.Initialize();

	bTexHD_ = TextureManager::LoadTexture("ManualScene", "b.png");
	bSprite_ = make_unique<Sprite>();
	bSprite_->Initialize({ 1280, 720 }, {1.0f,1.0f,1.0f,1.0f});
	bWt_.Initialize();
	
	cTexHD_ = TextureManager::LoadTexture("ManualScene", "c.png");
	cSprite_ = make_unique<Sprite>();
	cSprite_->Initialize({ 1280, 720 }, {1.0f,1.0f,1.0f,1.0f});
	cWt_.Initialize();

	isTookEnergy = false;
	isAlreadyPowerUp = false;

	/* ----- GameTimeCount ゲームカウント ----- */
	gameTimeCount_ = make_unique<GameTimeCount>();
	gameTimeCount_->Initialize();


	/* ----- CollisionManager コリジョンマネージャー ----- */
	collisionManager_ = make_unique<CollisionManager>();


	/* ----- ParticleManager パーティクルマネージャー ----- */
	particleManager_->Initialize();


	/* ----- FadeManager フェードマネージャー ----- */
	FadeManager::Initialize(func_FadeOut);
	isFadeFunc_ = false;

	isSetLevel = false;

	/* ----- Audio オーディオ----- */
	Audio::Initialize();
	gameBGM_ = Audio::LoadSound("BGM/BGM2.wav");
	Audio::PlayOnSound(gameBGM_, true, 0.02f);
}


/// <summary>
/// 更新処理
/// </summary>
void ManualScene::Update(GameManager* state) {

	fps_.Update();

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Update();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Update();

	manu->Update();
	for (int i = 0; i < 5; i++) {
		none[i]->Update();
	}

	// フェードアウトしていなかったら早期return
	if (!FadeManager::IsFadeOut()) {
		return;
	}


	/* ----- GameCamera ゲームカメラ----- */
	CameraUpdate();


	/* ----- GameWave ゲームウェーブ ----- */
	// ウェーブの更新処理
	WaveUpdate();

	if (isSetLevel) {
		//isWaveGaming_ = false;
		/* ----- Fade フェード ----- */
		fadeWt_.UpdateMatrix();
		fadeSprite_->SetColor(fadeColor_);

		// フェードの出現
		isFade_ = true;

		// レベルアップの処理に入る
		player_->SetNowLevelUp(true);

		// 終了処理に入る
		WaveExit(state);
	}

	ChangeScene(state);

#ifdef _DEBUG

	//ImGui::Begin("GameScene");
	//ImGui::Text("Description of scene change");
	//ImGui::Text("Scene change to 'ClearScene' by pressing X button while holding down Right.");
	//ImGui::Text("Scene change to 'OverScene' by pressing Y button while holding down Right.");
	//ImGui::Text("");

	//ImGui::Text("Camera");
	//ImGui::DragFloat3("Rotate", &camera_->rotate.x, 0.01f);
	//ImGui::DragFloat3("Translate", &camera_->translate.x, 0.01f);
	//ImGui::Text("");

	//ImGui::End();

#endif // _DEBUG
}


/// <summary>
/// 背景スプライトの描画処理
/// </summary>
void ManualScene::BackSpriteDraw() {

	/* ----- Player プレイヤー ----- */
	player_->Draw2DBack(camera_.get());
}


/// <summary>
/// ３Dオブジェクトの描画処理
/// </summary>
void ManualScene::ModelDraw() {

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Draw(camera_.get());

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Draw(camera_.get());

	/* ----- Player プレイヤー ----- */
	player_->Draw3D(camera_.get());
	for (shared_ptr<IPlayerBullet> bullet : playerBullets_) {
		bullet->Draw3D(camera_.get());
	}

	/* ----- Enemy 敵 ----- */
	enemyManager_.Draw(camera_.get());
	if (isTookEnergy) {
		manu->Draw(camera_.get());
	}
	for (int i = 0; i < 5; i++) {
		none[i]->Draw(camera_.get());
	}
	/* ----- ParticleManager パーティクルマネージャー ----- */
	particleManager_->Draw(camera_.get());
}


/// <summary>
/// 前景スプライトの描画処理
/// </summary>
void ManualScene::FrontSpriteDraw() {

	/* ----- GameTimeCount ゲームカウント ----- */
	gameTimeCount_->DrawFront2D(camera_.get());

	int numEnemy = 0;
	for (int i = 0; i < 5; i++) {
		if (!none[i]->IsAlive()) {
			numEnemy++;
		}
	}
	if (numEnemy <= 4) {
		aSprite_->Draw(aTexHD_, aWt_, camera_.get());
	}
	else {
		isTookEnergy = true;
		if (!isAlreadyPowerUp) {
			bSprite_->Draw(bTexHD_, bWt_, camera_.get());
		}
	}

	if (isAlreadyPowerUp) {
		cSprite_->Draw(cTexHD_, cWt_, camera_.get());
	}

	/* ----- Fade フェード ----- */
	if (isFade_) {
		fadeSprite_->Draw(fadeTexHD_, fadeWt_, camera_.get());
	}

	/* ----- Player プレイヤー ----- */
	player_->Draw2DFront(camera_.get());


	/* ----- FadeManager フェードマネージャー ----- */
	FadeManager::Draw(camera_.get());
}


/// <summary>
/// ウェーブの初期化処理
/// </summary>
void ManualScene::WaveInit()
{

}


/// <summary>
/// ウェーブの更新処理
/// </summary>
void ManualScene::WaveUpdate()
{
	if (!isFade_) {

		/* ----- CollisionManager コリジョンマネージャー ----- */
		CheckAllCollision();

		/* ----- ParticleManager パーティクルマネージャー ----- */
		particleManager_->Update();


		/* ----- Player プレイヤー ----- */
		PlayerUpdate();
		// 死んでいたら処理を抜ける
		if (player_->GetDeadFuncIsFinish()) {

			// シーン遷移のフラグを立てる
			isSceneChange_ = true;
		}
		if (player_->GetIsDead()) {
			return;
		}


		/* ----- Enemy 敵 ----- */
		enemyManager_.Update();


		/* ----- LevelManager レベルマネージャー ----- */
		levelManager_.Update(&enemyManager_, player_.get(), waveCount_);
	}
}


/// <summary>
/// ウェーブの終了処理
/// </summary>
void ManualScene::WaveExit(GameManager* state)
{
	state;
	if (GamePadInput::TriggerButton(PadData::B) || KeysInput::TriggerKey(DIK_SPACE)) {

		// ウェーブ開始のフラグを立ててウェーブの初期化処理に入るようにする
		isWaveStart_ = true;

		// ウェーブのカウントをインクリメント
		waveCount_++;

		// フェードを消す
		isFade_ = false;

		gameTimeCount_->ReSetTimer();

		// レベルアップの処理を抜ける
		player_->SetNowLevelUp(false);

		// レベルアップによる値の設定
		player_->LevelCheck();

		isSetLevel = false;

		//state->ChangeSceneState(new GameScene());

		return;
	}

	player_->ChangeLevelPrope();
}

void ManualScene::ChangeScene(GameManager* state) {
	//if (KeysInput::PressKeys(DIK_Q)) {
	//	state->ChangeSceneState(new TitleScene());
	//}

	//if (KeysInput::PressKeys(DIK_E)) {
	//	state->ChangeSceneState(new GameScene());
	//}

	if (isAlreadyPowerUp) {
		if (GamePadInput::PressButton(PadData::X)) {
			// ここでストップしておく
			Audio::StopOnSound(gameBGM_);
			state->ChangeSceneState(new TitleScene());
		}
	}
}


// コライダーの衝突判定
void ManualScene::CheckAllCollision()
{
	// プレイヤーとエネミーの個別処理
	int a = 0;
	for (int i = 0; i < 5; i++) {
		if (!none[i]->IsAlive()) {
			a++;
			if (a >= 5) {
				if (CollisionManager::CheckOBBxOBB(player_.get(), manu->GetOBBCollider())) {
					//player_->OnCollisionWithEnemy(manu.get());
					if (!preIsSetLevel) {
						isSetLevel = true;
						preIsSetLevel = true;
						player_.get()->SetEnergy(500);
						isAlreadyPowerUp = true;
					}
					//manu->OnCollisionWithPlayer(player_.get());
				}
				else {
					preIsSetLevel = false;
				}
			}
		}
	}

	for (int i = 0; i < 5; i++) {
		if (CollisionManager::CheckOBBxOBB(player_.get(), none[i]->GetOBBCollider())) {
			if (none[i]->IsFeed()) {
				none[i]->OnCollisionWithPlayer(player_.get());
			}
		}
	}

	// プレイヤーバレットとエネミーの個別処理
	for (shared_ptr<IPlayerBullet> bullet : playerBullets_) {
		if (!manu->IsFeed()) {
			if (CollisionManager::CheckOBBxOBB(bullet->GetOBBCOllider(), manu->GetOBBCollider())) {
				//bullet->onCollisionWithEnemy(manu.get());
				//manu->OnCollisionWithPlayerBullet(bullet.get());
			}
		}
	}

	for (shared_ptr<IPlayerBullet> bullet : playerBullets_) {
		for (int i = 0; i < 5; i++) {
			if (!none[i]->IsFeed()) {
				if (CollisionManager::CheckOBBxOBB(bullet->GetOBBCOllider(), none[i]->GetOBBCollider())) {
					bullet->onCollisionWithEnemy(none[i].get());
					none[i]->OnCollisionWithPlayerBullet(bullet.get());
				}
			}
		}
	}

	// コライダーリストのクリア
	collisionManager_->ColliderClear();

	// コライダーリストの追加
	collisionManager_->AddOBBColliders(player_.get());
	for (shared_ptr<IPlayerBullet> bullet : playerBullets_) {
		collisionManager_->AddOBBColliders(bullet->GetOBBCOllider());
	}

	for (IEnemy* enemy : *enemies_) {
		collisionManager_->AddOBBColliders(enemy->GetOBBCollider());
	}

	// コライダーの衝突判定
	collisionManager_->CheckAllCollision();
}


// カメラ関連の更新処理
void ManualScene::CameraUpdate()
{
	// トランスフォームの更新処理
	camera_->UpdateMatrix();

	// カメラの追従処理
		camera_->translate = player_->GetWorldPos() + cameraInitPos_;
		camera_->rotate.x = 0.2f;

	// スタート時のカメラ演出
	CameraStartMove();
}


// スタート時のカメラの演出
void ManualScene::CameraStartMove()
{
}


// プレイヤーへのズーム処理
void ManualScene::CameraZoomFunc()
{



}


// プレイヤー関連の更新処理
void ManualScene::PlayerUpdate()
{
	// プレイヤー本体
	player_->Update(camera_.get());

	// プレイヤーバレット
	for (shared_ptr<IPlayerBullet> bullet : playerBullets_) {
		bullet->Update();
	}
	playerBullets_.remove_if([](shared_ptr<IPlayerBullet> bullet) {
		if (bullet->isDead()) {
			bullet.reset();
			return true;
		}
		return false;
		}
	);
}


// エネミー関連の更新処理
void ManualScene::EnemyUpdate()
{
	//enemyManager_.Update();
}


// 値を保存しておく
void ManualScene::SaveValue()
{
	// プレイヤーのレベル
	SaveValue::Save("Hp_Level", player_->GetLevel(LevelPrope::HP));
	SaveValue::Save("Vel_Level", player_->GetLevel(LevelPrope::Vel));
	SaveValue::Save("Damage_Level", player_->GetLevel(LevelPrope::Damage));
	SaveValue::Save("Rate_Level", player_->GetLevel(LevelPrope::Rate));
	SaveValue::Save("Dash_Level", player_->GetLevel(LevelPrope::Dash));
	SaveValue::Save("Total_Level", player_->GetTotalLevel());
}

