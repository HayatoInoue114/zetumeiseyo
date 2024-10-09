#include "EngineManual.h"



/// <summary>
/// デストラクタ
/// </summary>
EngineManual::~EngineManual() 
{


}


/// <summary>
/// 初期化処理
/// </summary>
void EngineManual::Initialize() 
{
	// オーディオ
	kakkoiiAHD_ = Audio::LoadSound("kakkoii.wav");
	meriAHD_ = Audio::LoadSound("meri.wav");
	mokugyoAHD_ = Audio::LoadSound("mokugyo.wav");

	// 3Dモデル
	demoModel_ = make_unique<Model>();
	demoModel_->CreateFromObj("ball"); // <! (objデータが入ってるファイル名), これがObjの初期化処理だからInitializeは別途書かなくていい
	demoModelWt_.Initialize(); // <! Initializeは通さないとエラー吐く

	// 前景スプライト
	FSprite_ = make_unique<Sprite>();
	FSprite_->Initialize({ 128.0f, 128.0f }); // <! 初期座標とサイズ, 引数を入れなくても通る. その場合 pos={0.0f, 0.0f}, size={128.0f, 128.0f} で初期化
	FSpriteWt_.Initialize();
	FSpriteTexHD_ = TextureManager::LoadTexture("", "circle.png");

	// 前景スプライト
	BSprite_ = make_unique<Sprite>();
	BSprite_->Initialize({ 1280.0f, 720.0f });
	BSpriteWt_.Initialize();
	BSpriteTexHD_ = TextureManager::LoadTexture("", "uvChecker.png");
}


/// <summary>
/// 更新処理
/// </summary>
void EngineManual::Update(Camera* camera)
{
	camera;
	// 3Dモデル
	demoModelWt_.UpdateMatrix();

	// 前景スプライト
	FSpriteWt_.UpdateMatrix();

	// 背景スプライト
	BSpriteWt_.UpdateMatrix();


	// オーディオ
	if (KeysInput::TriggerKey(DIK_1)) 
	{
		if (!Audio::IsPlaying(kakkoiiAHD_)) // <! IsPlaying(オーディオハンドル) オーディオが再生中か
		{

			Audio::PlayOnSound(kakkoiiAHD_, true, 0.5f); // <! PlayOnSound(オーディオハンドル, ループフラグ, ボリューム)
		}
		else {
			Audio::StopOnSound(kakkoiiAHD_);
		}
	}
	if (KeysInput::TriggerKey(DIK_2))
	{
		if (!Audio::IsPlaying(meriAHD_))
		{

			Audio::PlayOnSound(meriAHD_, true, 0.5f);
		}
		else {
			Audio::StopOnSound(meriAHD_);
		}
	}
	if (KeysInput::TriggerKey(DIK_3))
	{
		Audio::PlayOnSound(mokugyoAHD_, false, 0.5f);
	}


#ifdef _DEBUG

	// ImGuiは TreeNode が一番画面がごちゃつかないんで基本 TreeNode 
	if (ImGui::TreeNode("3DModel")) {
		ImGui::Text("WorldTransform");
		ImGui::DragFloat3("Scale", &demoModelWt_.scale.x, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &demoModelWt_.rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &demoModelWt_.translate.x, 0.01f);

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("FrontSprite")) {
		ImGui::Text("WorldTransform");
		ImGui::DragFloat2("Scale", &FSpriteWt_.scale.x, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat2("Rotate", &FSpriteWt_.rotate.x, 0.01f);
		ImGui::DragFloat2("Translate", &FSpriteWt_.translate.x, 1.0f);

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("BackSprite")) {
		ImGui::Text("WorldTransform");
		ImGui::DragFloat2("Scale", &BSpriteWt_.scale.x, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat2("Rotate", &BSpriteWt_.rotate.x, 0.01f);
		ImGui::DragFloat2("Translate", &BSpriteWt_.translate.x, 1.0f);

		ImGui::TreePop();
	}

#endif // _DEBUG
}


/// <summary>
/// 背景スプライトの描画処理
/// </summary>
void EngineManual::BackSpriteDraw(Camera* camera)
{
	camera;
	// 背景スプライト
	//BSprite_->Draw(BSpriteTexHD_, BSpriteWt_, view);
}


/// <summary>
/// ３Dオブジェクトの描画処理
/// </summary>
void EngineManual::ModelDraw(Camera* camera)
{
	camera;
	// 3Dモデル
	//demoModel_->Draw(demoModelWt_, view);
}


/// <summary>
/// 前景スプライトの描画処理
/// </summary>
void EngineManual::FrontSpriteDraw(Camera* camera)
{
	camera;
	// 前景スプライト
	//FSprite_->Draw(FSpriteTexHD_, FSpriteWt_, view);

}
