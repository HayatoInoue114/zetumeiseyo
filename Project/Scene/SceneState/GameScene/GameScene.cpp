#include "GameScene.h"



/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene() {

	particleManager_ = ParticleManager::GetInstance();
}


/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene() {

	// ここでストップしておく
	Audio::StopOnSound(gameBGM_);
}


/// <summary>
/// 初期化処理
/// </summary>
void GameScene::Initialize() {

	/* ----- SaveValeu 値の保存クラス ----- */
	SaveValue::Clear();


	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Initialize();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Initialize();

	/* ----- Field フィールド ----- */
	Field::GetInstance()->Initialize();

	
	/* ----- Camera カメラ ----- */
	camera_ = make_unique<Camera>();
	camera_->Initialize();
	cameraInitPos_ = { 0.0f, 1.5f, -4.0f };
	camera_->translate = cameraInitPos_;
	cameraDiffRotate_ = { 0.2f, 0.0f, 0.0f };
	cameraDiffPos_ = { 0.0f, 6.0f, -30.0f };
	camera_->UpdateMatrix();


	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();
	/* ----- Player プレイヤー ----- */
	player_ = make_unique<Player>();
	player_->Initialize();
	player_->SetGameScene(this);
	player_->SetFollowCamera(followCamera_.get());


	/* ----- GameWave ゲームウェーブ ----- */
	waveCount_ = 1;
	waveMax_ = 6;
	isWaveStart_ = true;
	isSceneChange_ = false;


	/* ----- Enemy 敵 ----- */
	enemyManager_.SetGameScene(this);
	enemies_ = enemyManager_.GetEnemy();


	/* ----- Fade フェード ----- */
	fadeTexHD_ = TextureManager::LoadTexture("", "Fade.png");
	fadeSprite_ = make_unique<Sprite>();
	fadeSprite_->Initialize({ 1280, 720 }, fadeColor_);
	fadeWt_.Initialize();
	isFade_ = false;


	/* ----- GameTimeCount ゲームカウント ----- */
	gameTimeCount_ = make_unique<GameTimeCount>();
	gameTimeCount_->Initialize();


	/* ----- WaveCountUI ウェーブカウント ----- */
	waveCountUI_ = make_unique<WaveCountUI>();
	waveCountUI_->Initialize(waveCount_, waveMax_);


	/* ----- CollisionManager コリジョンマネージャー ----- */
	collisionManager_ = make_unique<CollisionManager>();


	/* ----- ParticleManager パーティクルマネージャー ----- */
	particleManager_->Initialize();


	/* ----- FadeManager フェードマネージャー ----- */
	FadeManager::Initialize(func_FadeOut);
	isFadeFunc_ = false;


	/* ----- Audio オーディオ----- */
	Audio::Initialize();
	gameBGM_ = Audio::LoadSound("BGM/BGM2.wav");
	Audio::PlayOnSound(gameBGM_, true, 0.02f);
	decisionSE_ = Audio::LoadSound("SE/kettei.wav");

	/* ----- UI ----- */
	goTex_ = TextureManager::LoadTexture("GameScene/UI", "go.png");
	goSp_ = std::make_unique<Sprite>();
	goSp_->Initialize({ 512,512 });
	goSp_->SetSpriteOrigin(SpriteOrigin::Center);
	goWT_.Initialize();
	goWT_.scale = {};
	goWT_.translate = { 640,360,0 };
	nowFrame_ = 0;
	endFrame_ = 20;
	animTime_ = 0;
	goColor_ = { 1,1,1,1 };
	
	readyTex_ = TextureManager::LoadTexture("GameScene/UI", "ready.png");
	readySp_ = std::make_unique<Sprite>();
	readySp_->Initialize({512,512});
	readySp_->SetSpriteOrigin(SpriteOrigin::Center);
	readyWT_.Initialize();
	readyWT_.scale = {};
	readyWT_.translate = { 640,360,0 };

	//プレイヤー死んだとき用
	pDieNowFrame_ = 0;
	pDieEndFrame_ = 150;
	pDieAnimTime_ = 0;

	pDieWT_.Initialize();
	Vector3 tmpCameraPos = { 0.0f, 1.5f, -4.0f };
	pDieWT_.translate = player_->GetWorldPos() + tmpCameraPos;
	pDieWT_.rotate = { 0.2f, 0.0f, 0.0f };

	pDieTex_ = TextureManager::LoadTexture("GameScene/UI", "gameOverTex.png");
	pDieSp_ = std::make_unique<Sprite>();
	pDieSp_->Initialize({ 2048,2048 });
	pDieSp_->SetSpriteOrigin(SpriteOrigin::Center);
	pDieSpriteWT_.Initialize();
	pDieSpriteWT_.scale = { 25,25,25 };
	pDieSpriteWT_.translate = { 640,360,0 };

	blackTex_ = TextureManager::LoadTexture("GameScene/UI", "backBlack.png");
	blackSp_ = std::make_unique<Sprite>();
	blackSp_->Initialize({ 1280,720 });
	blackSp_->SetSpriteOrigin(SpriteOrigin::Center);
	blackSpriteWT_.Initialize();
	blackSpriteWT_.scale = { 15,15,15 };
	blackSpriteWT_.translate = { 640,360,0 };

	followCamera_->SetPlayer(player_.get());
}


/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update(GameManager* state) {

	fps_.Update();

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Update();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Update();

	/* ----- Field フィールド ----- */
	Field::GetInstance()->Update();


	// フェードアウトしていなかったら早期return
	if (!FadeManager::IsFadeOut()) {
		return;
	}


	/* ----- GameCamera ゲームカメラ----- */
	CameraUpdate();

	//プレイヤーが死んだときのカメラ演出
	PlayerDieCmaera();

	/* ----- GameWave ゲームウェーブ ----- */
	// ウェーブ開始フラグがたっていたら初期化処理に入る
	if (isWaveStart_) {
		WaveInit();
	}
	// ウェーブの更新処理
	WaveUpdate();

	// シーン遷移のフラグが立っていたらシーン遷移
	if (isSceneChange_) {

		// 値を保存しておく
		SaveValue();

		if (FadeManager::IsFadeIn()) {

			// ここでストップしておく
			Audio::StopOnSound(gameBGM_);

			// 死んでいたらゲームオーバーシーンへ
			if (player_->GetIsDead()) {

				// 時間いっぱい生き残ったってことだからクリアシーンへ
				state->ChangeSceneState(new OverScene());
				ParticleManager::Clear();
				return;
			}

			// 時間いっぱい生き残ったってことだからクリアシーンへ
			state->ChangeSceneState(new ClearScene());
			ParticleManager::Clear();
			return;
		}
	}

#ifdef _DEBUG

	ImGui::Begin("GameScene");
	ImGui::Text("");
	ImGui::Text("Camera");
	ImGui::DragFloat3("Rotate", &camera_->rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &camera_->translate.x, 0.01f);
	ImGui::End();

#endif // _DEBUG
}


/// <summary>
/// 背景スプライトの描画処理
/// </summary>
void GameScene::BackSpriteDraw() {

	/* ----- Player プレイヤー ----- */
	player_->Draw2DBack(camera_.get());
}


/// <summary>
/// ３Dオブジェクトの描画処理
/// </summary>
void GameScene::ModelDraw() {

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
	if (startCameraAnimIsFinish_) {
		enemyManager_.Draw(camera_.get());
	}

	/* ----- ParticleManager パーティクルマネージャー ----- */
	particleManager_->Draw(camera_.get());

	/* ----- Field フィールド ----- */
	Field::GetInstance()->Draw(camera_.get());
}


/// <summary>
/// 前景スプライトの描画処理
/// </summary>
void GameScene::FrontSpriteDraw() {

	/* ----- GameTimeCount ゲームカウント ----- */
	gameTimeCount_->DrawFront2D(camera_.get());

	/* ----- WaveCountUI ウェーブカウント ----- */
	waveCountUI_->Draw2DFront(waveCount_, camera_.get());

	/* ----- Fade フェード ----- */
	if (isFade_) {
		fadeSprite_->Draw(fadeTexHD_, fadeWt_, camera_.get());
	}

	/* ----- Player プレイヤー ----- */
	player_->Draw2DFront(camera_.get());

	//スタート演出用
	StartTexture();

	//死亡時演出
	MarioSprite();
}


/// <summary>
/// ウェーブの初期化処理
/// </summary>
void GameScene::WaveInit()
{
	enemyManager_.EnemySpawn(player_.get());
	enemies_ = enemyManager_.GetEnemy();

	// ウェーブ開始のフラグを折って初期化処理に入らないようにする
	isWaveStart_ = false;
}


/// <summary>
/// ウェーブの更新処理
/// </summary>
void GameScene::WaveUpdate()
{
	// カメラの演出をしていたら早期return
	if (!startCameraAnimIsFinish_) {
		return;
	}

	// フラグがたっていたら入らない
	if (!gameTimeCount_->IsTimeUp()) {

		/* ----- GameTimeCount ゲームカウント ----- */
		gameTimeCount_->Update();

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

	// タイマーが既定値になっていたら終了処理に入る
	if (gameTimeCount_->IsTimeUp()) {

		// ウェーブのカウントがマックスだったらシーン遷移
		if (waveCount_ == waveMax_) {

			// シーン遷移のフラグを立てる
			isSceneChange_ = true;
			return;
		}

		//isWaveGaming_ = false;
		/* ----- Fade フェード ----- */
		fadeWt_.UpdateMatrix();
		fadeSprite_->SetColor(fadeColor_);

		// フェードの出現
		isFade_ = true;

		// レベルアップの処理に入る
		player_->SetNowLevelUp(true);

		// 終了処理に入る
		WaveExit();
	}
}


/// <summary>
/// ウェーブの終了処理
/// </summary>
void GameScene::WaveExit()
{
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

		return;
	}

	player_->ChangeLevelPrope();
}


// コライダーの衝突判定
void GameScene::CheckAllCollision()
{
	// プレイヤーとエネミーの個別処理
	for (IEnemy* enemy : *enemies_) {

		if (enemy->isSpawn) {
			// 距離が遠ければ衝突処理には入らない
			float length = std::abs(Length(player_->GetWorldPos() - enemy->GetOBBCollider()->GetOBBWorldPos()));
			if (length >= 3.0f) {
				continue;
			}
			if (CollisionManager::CheckOBBxOBB(player_.get(), enemy->GetOBBCollider())) {

				player_->OnCollisionWithEnemy(enemy);
				enemy->OnCollisionWithPlayer(player_.get());
			}
		}
	}

	// プレイヤーバレットとエネミーの個別処理
	for (shared_ptr<IPlayerBullet> bullet : playerBullets_) {

		for (IEnemy* enemy : *enemies_) {
			if (!enemy->IsFeed()) {

				// 距離が遠ければ衝突処理には入らない
				float length = std::abs(Length(bullet->GetOBBCOllider()->GetOBBWorldPos() - enemy->GetOBBCollider()->GetOBBWorldPos()));
				if (length >= 3.0f) {
					continue;
				}
				if (CollisionManager::CheckOBBxOBB(bullet->GetOBBCOllider(), enemy->GetOBBCollider())) {

					bullet->onCollisionWithEnemy(enemy);
					enemy->OnCollisionWithPlayerBullet(bullet.get());
				}
			}
		}
	}

	// これ何？
	for (IEnemy* enemy : *enemies_) {
		for (EnemyBullet* bullet : *enemy->GetEnemyBullets()) {

			// 距離が遠ければ衝突処理には入らない
			float length = std::abs(Length(player_->GetWorldPos() - bullet->GetWorldTransform().GetWorldPos()));
			if (length >= 3.0f) {
				continue;
			}
			if (CollisionManager::CheckOBBxOBB(player_.get(), bullet)) {

				player_->OnCollisionWithEnemyBullet();
				bullet->OnCollisionWithPlayer();
			}
		}
	}

	//弾同士の判定
	for (shared_ptr<IPlayerBullet> pbullet : playerBullets_) {
		for (IEnemy* enemy : *enemies_) {
			for (EnemyBullet* ebullet : *enemy->GetEnemyBullets()) {

				// 距離が遠ければ衝突処理には入らない
				float length = std::abs(Length(pbullet->GetOBBCOllider()->GetOBBWorldPos() - ebullet->GetOBBWorldPos()));
				if (length >= 3.0f) {
					continue;
				}
				if (CollisionManager::CheckOBBxOBB(pbullet->GetOBBCOllider(), ebullet)) {
					pbullet->onCollisionWithEnemyBullet(ebullet);
					ebullet->OnCollisionWithPlayerBullet();
				}
			}
		}
	}
}


// カメラ関連の更新処理
void GameScene::CameraUpdate()
{
	// トランスフォームの更新処理
	camera_->UpdateMatrix();
	followCamera_->Update();
	// プレイヤーの死亡フラグが立っていたら
	// プレイヤーへのズーム処理へ行く
	if (player_->GetIsDead()) {

		return;
	}

	// カメラの追従処理
	if (startCameraAnimIsFinish_ && player_->GetHp() !=0) {
		/*cameraDiffPos_ = followCamera_->GetForwardVec();
		camera_->translate = player_->GetWorldPos() + cameraDiffPos_;*/

		PlayerCamera();

		// スタート演出の処理に入ってほしくないのでここでreturnを入れる
		return;
	}
	
	
	// スタート時のカメラ演出
	CameraStartMove();
}


// スタート時のカメラの演出
void GameScene::CameraStartMove()
{
	// フレームをインクリメント
	cameraNowFrame_++;

	// 今のフレームを終了フレームで割る
	startAnimTime_ = float(cameraNowFrame_) / float(cameraEndFrame_);

	// スタート演出処理
	// 設定した座標まで動かす
	camera_->translate.y =
		cameraInitPos_.y + (cameraDiffPos_.y - cameraInitPos_.y) * Ease::OutCubic(startAnimTime_);
	camera_->translate.z =
		cameraInitPos_.z + (cameraDiffPos_.z - cameraInitPos_.z) * Ease::OutCubic(startAnimTime_);
	// 設定した回転まで動かす
	camera_->rotate.x =
		0.0f + (cameraDiffRotate_.x - 0.0f) * Ease::OutCubic(startAnimTime_);

	// 演出が終わっていたら終了フラグを立てておく
	if (cameraNowFrame_ == cameraEndFrame_) {
		startCameraAnimIsFinish_ = true;
		cameraNowFrame_ = 0;
	}
}


// プレイヤー関連の更新処理
void GameScene::PlayerUpdate()
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
void GameScene::EnemyUpdate()
{
	enemyManager_.Update();
}


// 値を保存しておく
void GameScene::SaveValue()
{
	// プレイヤーのレベル
	SaveValue::Save("Hp_Level", player_->GetLevel(LevelPrope::HP));
	SaveValue::Save("Vel_Level", player_->GetLevel(LevelPrope::Vel));
	SaveValue::Save("Damage_Level", player_->GetLevel(LevelPrope::Damage));
	SaveValue::Save("Rate_Level", player_->GetLevel(LevelPrope::Rate));
	SaveValue::Save("Dash_Level", player_->GetLevel(LevelPrope::Dash));
	SaveValue::Save("Total_Level", player_->GetTotalLevel());
}

void GameScene::StartTexture()
{
	if (!startCameraAnimIsFinish_) {
		float t = Ease::OutExpo(startAnimTime_);

		Vector3 initialScale = { 0,0,0 };
		Vector3 initialRotation = { 0,0,-10 };

		// 最終的にスケール1に近づける
		readyWT_.scale.x = initialScale.x + (1.0f - initialScale.x) * t;
		readyWT_.scale.y = initialScale.y + (1.0f - initialScale.y) * t;

		// 最終的にRotateを0にする
		readyWT_.rotate.z = initialRotation.z * (1.0f - t);

		readySp_->Draw(readyTex_, readyWT_, camera_.get());

		readyWT_.UpdateMatrix();
	}

	if (startCameraAnimIsFinish_ && nowFrame_ <= 100) {
		nowFrame_++;

		animTime_ = nowFrame_ / endFrame_;

		float t = Ease::OutExpo(animTime_);

		Vector3 initialScale = { 0,0,0 };
		Vector3 initialRotation = { 0,0,-10 };

		goWT_.scale.x += 0.1f;
		goWT_.scale.y += 0.1f;

		// 最終的にRotateを0にする
		goWT_.rotate.z = initialRotation.z * (1.0f - t);

		goColor_.w -= 0.01f;
		goSp_->SetColor(goColor_);

		goSp_->Draw(goTex_, goWT_, camera_.get());

		goWT_.UpdateMatrix();
	}
	
}

//プレイヤーが死んだときのカメラ演出
void GameScene::PlayerDieCmaera()
{
	if (player_->GetHp() == 0) {
		pDieNowFrame_++;

		pDieAnimTime_ = pDieNowFrame_ / pDieEndFrame_;

		float t = Ease::InCirc(pDieAnimTime_);

		Vector3 cameraDiffPos = cameraDiffPos_ + player_->GetWorldPos();

		camera_->translate.y =
			cameraDiffPos.y + (pDieWT_.translate.y - cameraDiffPos.y) * t;
		camera_->translate.z =
			cameraDiffPos.z + (pDieWT_.translate.z - cameraDiffPos.z) * t;
		// 設定した回転まで動かす
		camera_->rotate.x =
			cameraDiffRotate_.x + (pDieWT_.rotate.x - cameraDiffRotate_.x) * t;
	}
}

void GameScene::PlayerCamera()
{
	// プレイヤーの位置と前方ベクトルを取得
	Vector3 playerPosition = player_->GetWorldPos();      // プレイヤーの位置
	Vector3 forwardVec = followCamera_->GetForwardVec();  // プレイヤーの向きに基づいたカメラの前方ベクトル

	// カメラの設定
	float cameraDistance = 30.0f; // プレイヤーからの距離
	float cameraHeight = cameraDiffPos_.y;   // プレイヤーの高さからのオフセット

	// カメラ位置を前方ベクトルと高さを考慮して計算
	Vector3 cameraPosition = playerPosition - forwardVec * cameraDistance;
	cameraPosition.y += cameraHeight; // 高さのオフセットを加える

	// カメラの位置と向きを設定
	camera_->SetPosition(cameraPosition);
	camera_->LookAt(playerPosition, forwardVec, followCamera_->GetRightVec(), { 0.0f, 1.0f, 0.0f }); // 上向きベクトルを指定してプレイヤーを向く
	

	// ビュー行列を更新
	camera_->UpdateMatrix();
}


void GameScene::MarioSprite()
{
	//GameOver用のスプライトを描画
	float scaleDecayDie = 0.17f;
	float scaleDecayBlack = 0.1f;
	if (pDieSpriteWT_.scale.x > 0 && player_->GetHp() == 0) {
		pDieSpriteWT_.scale.x -= scaleDecayDie;
		pDieSpriteWT_.scale.y -= scaleDecayDie;
		if (pDieSpriteWT_.scale.x < 0.1f) {
			pDieSpriteWT_.scale.x = 0.1f;
			pDieSpriteWT_.scale.y = 0.1f;
		}

		pDieSpriteWT_.UpdateMatrix();
	}
	if (blackSpriteWT_.scale.x > 1 && player_->GetHp() == 0) {
		blackSpriteWT_.scale.x -= scaleDecayBlack;
		blackSpriteWT_.scale.y -= scaleDecayBlack;
		if (blackSpriteWT_.scale.x < 1) {
			blackSpriteWT_.scale.x = 1;
			blackSpriteWT_.scale.y = 1;
		}
		blackSpriteWT_.UpdateMatrix();
	}
	if (player_->GetHp() == 0) {
		blackSp_->Draw(blackTex_, blackSpriteWT_, camera_.get());
		pDieSp_->Draw(pDieTex_, pDieSpriteWT_, camera_.get());
	}
	else {
		/* ----- FadeManager フェードマネージャー ----- */
		FadeManager::Draw(camera_.get());
	}
}

