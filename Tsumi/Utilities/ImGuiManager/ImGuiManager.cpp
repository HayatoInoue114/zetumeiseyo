#include "ImGuiManager.h"

ImGuiManager* ImGuiManager::GetInstance() {

	static ImGuiManager instance;
	return &instance;
}


void ImGuiManager::Initialize() {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("Resources/font/komorebi-gothic-P.ttf", 13.0f, nullptr, glyphRangesJapanese);
	
	ImGui_ImplWin32_Init(WinApp::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(
		DirectXCommon::GetInstance()->GetDevice(),
		DirectXCommon::GetInstance()->GetSwapChains().Desc.BufferCount,
		DirectXCommon::GetInstance()->GetRTV().Desc.Format,
		DirectXCommon::GetInstance()->GetSrvDescriptorHeap(),
		DirectXCommon::GetInstance()->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		DirectXCommon::GetInstance()->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
}


void ImGuiManager::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::PushFont(io.Fonts->Fonts[1]);

	ID3D12DescriptorHeap* descriptorHeap[] = { DirectXCommon::GetInstance()->GetSrvDescriptorHeap() };
	DirectXCommon::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, descriptorHeap);
}


void ImGuiManager::EndFrame() {

	ImGui::PopFont();
	ImGui::Render();

	//実際のCommandListのImGuiの描画コマンドを進む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXCommon::GetInstance()->GetCommandList());
}