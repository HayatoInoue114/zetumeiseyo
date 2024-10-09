#include "Tsumi.h"

/// <summary>
/// インスタンスの取得
/// </summary>
Tsumi* Tsumi::GetInstance() {
	static Tsumi instance;
	return &instance;
}


/// <summary>
/// 初期化処理
/// </summary>
void Tsumi::Initialize() {

	Tsumi::GetInstance();

	// WinAppの初期化処理
	WinApp::Initialize(L"絶命せよ！！");

	// DirectXCommonの初期化処理
	DirectXCommon::Initialize();

	// ShaderManagerの初期化処理
	ShaderManager::Initialize();

	// GraphicPiplelineManagerの初期化処理
	GraphicPipelineManager::Initialize();

	// TextureManagerの初期化処理
	TextureManager::Initialize();

	// ImGuiの初期化処理
	ImGuiManager::Initialize();

	// Inputの初期化処理
	KeysInput::Initialize();
	GamePadInput::Initialize();

	// Audioの初期化処理
	Audio::Initialize();

	// DescriptorManagerの初期化処理
	DescriptorManager::Initialize();

	// DrawSystemの初期化処理
	DrawSystem::GetInstance()->Initialize();

	// ParticleManagerの初期化処理
	ParticleManager::GetInstance()->Initialize();
}


/// <summary>
/// 解放処理
/// </summary>
void Tsumi::Finalize() {

	Audio::Finalize();
	TextureManager::Finalize();
	DescriptorManager::Clear();
}


/// <summary>
/// フレームの開始
/// </summary>
void Tsumi::BeginFlame() {

	DirectXCommon::PreDraw();
	ImGuiManager::BeginFrame();
	KeysInput::BeginFrame();
	GamePadInput::BeginFrame();
	DescriptorManager::BeginFrame();

}


/// <summary>
/// フレームの終了
/// </summary>
void Tsumi::EndFlame() {

	ImGuiManager::EndFrame();
	DirectXCommon::PostDraw();
}


/// <summary>
/// メッセージの処理
/// </summary>
bool Tsumi::ProcessMessage() {

	if (WinApp::ProcessMessage()) {
		return true;
	}
	else {

		return false;
	}
}