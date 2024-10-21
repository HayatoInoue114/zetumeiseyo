#include "GameManager.h"



/// <summary>
/// コンストラクタ
/// </summary>
GameManager::GameManager() {

	Tsumi::Initialize();
	Scene_ = new GameScene();
	Scene_->Initialize();
}


/// <summary>
/// デストラクタ
/// </summary>
GameManager::~GameManager() {

	delete Scene_;
	Tsumi::Finalize();
}


/// <summary>
/// ゲームループ
/// </summary>
void GameManager::Run() {

	TextLog::Init();
	TextLog::Load("Run_Start");
	while (Tsumi::ProcessMessage() == 0) {

		Tsumi::BeginFlame();

		Scene_->Update(this);
		Scene_->BackSpriteDraw();
		Scene_->ModelDraw();
		Scene_->FrontSpriteDraw();

		Tsumi::EndFlame();
	}
	TextLog::Load("Run_End");
	TextLog::Close();
}


/// <summary>
/// シーンチェンジ
/// </summary>
void GameManager::ChangeSceneState(IScene* newScene) {

	Audio::SoundUnload();
	delete Scene_;
	Scene_ = newScene;
	Scene_->Initialize();
	return;
}