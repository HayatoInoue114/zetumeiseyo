//#include "DebugScene.h"
//
//
//
///// <summary>
///// デストラクタ
///// </summary>
//DebugScene::~DebugScene() {}
//
//
//
///// <summary>
///// 初期化処理
///// </summary>
//void DebugScene::Initialize() {
//
//	viewProjection_.Initialize();
//	viewProjection_.translate = { 0.0f, 40.0f, -50.0f };
//	viewProjection_.rotate = { -5.6f,0.0f,0.0f };
//
//	engineManual_ = std::make_unique<EngineManual>();
//	engineManual_->Initialize();
//
//	player_ = std::make_unique<Player>();
//	player_->Initialize();
//
//	ground_ = std::make_unique<Ground>();
//	ground_->Initialize();
//}
//
//
//
///// <summary>
///// 更新処理
///// </summary>
//void DebugScene::Update(GameManager* state) {
//
//	//engineManual_->Update(viewProjection_);
//
//	ground_->Update();
//
//	viewProjection_.UpdateMatrix();
//
//
//#ifdef USE_IMGUI
//
//	ImGui::Begin("DebugScene");
//	ImGui::End();
//
//	ImGui::Begin("Camera");
//	ImGui::DragFloat3("Rotate", &viewProjection_.rotate.x, 0.005f);
//	ImGui::DragFloat3("Translate", &viewProjection_.translate.x, 0.01f);
//	ImGui::End();
//
//#endif // USE_IMGUI
//}
//
//
//
///// <summary>
///// 背景スプライトの描画処理
///// </summary>
//void DebugScene::BackSpriteDraw() {
//
//	//engineManual_->BackSpriteDraw(viewProjection_);
//}
//
//
//
///// <summary>
///// ３Dオブジェクトの描画処理
///// </summary>
//void DebugScene::ModelDraw() {
//	ground_->Draw(viewProjection_);
//
//	//engineManual_->ModelDraw(viewProjection_);
//}
//
//
//
///// <summary>
///// 前景スプライトの描画処理
///// </summary>
//void DebugScene::FrontSpriteDraw() {
//
//	//engineManual_->FrontSpriteDraw(viewProjection_);
//}
//
//
