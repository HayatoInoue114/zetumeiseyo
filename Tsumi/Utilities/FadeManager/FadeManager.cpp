#include "FadeManager.h"



// 初期化処理
void FadeManager::Initialize(FunctionFade func)
{
	// テクスチャの読み込み
	FadeManager::GetInstance()->texHD_ = TextureManager::LoadTexture("", "Fade.png");

	// スプライトの初期化
	FadeManager::GetInstance()->sprite_ = make_unique<Sprite>();
	FadeManager::GetInstance()->sprite_->Initialize(WinApp::WindowSize());

	// スプライトトランスフォームの初期化
	FadeManager::GetInstance()->wt_.Initialize();

	if (func == func_FadeIn) {

		// カラーの設定
		FadeManager::GetInstance()->color_ = Vector4::zero;
		FadeManager::GetInstance()->state_.isFadeIn = false;
		FadeManager::GetInstance()->state_.isFadeOut = true;

		// フレームなどの変数初期化
		FadeManager::FrameInit();
	}
	else if (func == func_FadeOut) {

		// カラーの設定
		FadeManager::GetInstance()->color_ = Vector4::oneW;
		FadeManager::GetInstance()->state_.isFadeOut = true;
		FadeManager::GetInstance()->state_.isFadeOut = false;

		// フレームなどの変数初期化
		FadeManager::FrameInit();
	}
}


// 描画処理
void FadeManager::Draw(Camera* camera)
{
	// カラーの設定
	FadeManager::GetInstance()->sprite_->SetColor(FadeManager::GetInstance()->color_);

	uint32_t texHandle = FadeManager::GetInstance()->texHD_;
	WorldTransform spriteTransform = FadeManager::GetInstance()->wt_;
	FadeManager::GetInstance()->sprite_->Draw(texHandle, spriteTransform, camera);


#ifdef _DEBUG

	if (ImGui::TreeNode("Fade"))
	{
		ImGui::Text("startFrame = %.2f", FadeManager::GetInstance()->startFrame_);
		ImGui::Text("finishFrame = %.2f", FadeManager::GetInstance()->finishFrame_);
		ImGui::Text("time = %.2f", FadeManager::GetInstance()->time_);
		ImGui::DragFloat4("color", &FadeManager::GetInstance()->color_.x, 0.01f, 0.0f, 1.0f);
		ImGui::TreePop();
	}

#endif // _DEBUG
}


//フェードイン処理
bool FadeManager::IsFadeIn()
{
	// すでに画面が真っ暗の状態なら早期リターン
	if (FadeManager::GetInstance()->state_.isFadeIn &&
		!FadeManager::GetInstance()->state_.isFadeOut) {
		return true;
	}

	// 値をとってくる
	int start = FadeManager::GetInstance()->startFrame_;
	int finish = FadeManager::GetInstance()->finishFrame_;
	float t = FadeManager::GetInstance()->time_;
	Vector4 color = FadeManager::GetInstance()->color_;

	// フレームの加算
	start++;

	// 補間割合の計算
	t = float(start) / float(finish);

	// 補間の処理
	color.w = Lerp(float(0.0f), float(1.0f), t);

	FadeManager::GetInstance()->startFrame_ = start;
	FadeManager::GetInstance()->finishFrame_ = finish;
	FadeManager::GetInstance()->time_ = t;
	FadeManager::GetInstance()->color_ = color;
	FadeManager::GetInstance()->sprite_->SetColor(FadeManager::GetInstance()->color_);

	// 補間が終了したら、ステートの設定して、trueを返す
	if (FadeManager::GetInstance()->time_ >= 1.0f) {
		FadeManager::GetInstance()->state_.isFadeIn = true;
		FadeManager::GetInstance()->state_.isFadeOut = false;
		FadeManager::FrameInit();
	}

	// フェードインしたらtrueを返す
	if (FadeManager::GetInstance()->state_.isFadeIn && !FadeManager::GetInstance()->state_.isFadeOut) {
		return true;
	}

	return false;
}


// フェードアウト処理
bool FadeManager::IsFadeOut()
{
	// すでに画面が明けていたらの状態なら早期リターン
	if (!FadeManager::GetInstance()->state_.isFadeIn &&
		FadeManager::GetInstance()->state_.isFadeOut) {
		return true;
	}

	// 値をとってくる
	int start = FadeManager::GetInstance()->startFrame_;
	int finish = FadeManager::GetInstance()->finishFrame_;
	float t = FadeManager::GetInstance()->time_;
	Vector4 color = FadeManager::GetInstance()->color_;

	// フレームの加算
	start++;

	// 補間割合の計算
	t = float(start) / float(finish);

	// 補間の処理
	color.w = Lerp(float(1.0f), float(0.0f), t);

	FadeManager::GetInstance()->startFrame_ = start;
	FadeManager::GetInstance()->finishFrame_ = finish;
	FadeManager::GetInstance()->time_ = t;
	FadeManager::GetInstance()->color_ = color;
	FadeManager::GetInstance()->sprite_->SetColor(FadeManager::GetInstance()->color_);

	// 補間が終了したら、ステートの設定して、trueを返す
	if (FadeManager::GetInstance()->time_ >= 1.0f) {
		FadeManager::GetInstance()->state_.isFadeIn = false;
		FadeManager::GetInstance()->state_.isFadeOut = true;
		FadeManager::FrameInit();
	}

	// フェードインしたらtrueを返す
	if (!FadeManager::GetInstance()->state_.isFadeIn && FadeManager::GetInstance()->state_.isFadeOut) {
		return true;
	}

	return false;
}


// フレームの初期化
void FadeManager::FrameInit()
{
	FadeManager::GetInstance()->startFrame_ = 0;
	FadeManager::GetInstance()->finishFrame_ = 120;
	FadeManager::GetInstance()->time_ = 0.0f;
}


// フレームの更新処理
void FadeManager::FrameUpdate(FunctionFade func)
{
	// フレームの加算
	int start = FadeManager::GetInstance()->startFrame_;
	int finish = FadeManager::GetInstance()->finishFrame_;
	float t = FadeManager::GetInstance()->time_;
	Vector4 color = FadeManager::GetInstance()->color_;

	// 補間割合の計算
	t = float(start / finish);

	// 補間の処理
	if (func == func_FadeIn) {

		color.w = Lerp(float(0.0f), float(1.0f), t);
	}
	else if (func == func_FadeOut) {

		color.w = Lerp(float(1.0f), float(0.0f), t);
	}

	FadeManager::GetInstance()->startFrame_ = start;
	FadeManager::GetInstance()->finishFrame_ = finish;
	FadeManager::GetInstance()->time_ = t;
	FadeManager::GetInstance()->sprite_->SetColor(color);
}
