#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")

#include "WinApp.h"
#include "DirectXCommon.h"
#include "GraphicPipelineManager.h"
#include "MyMath.h"
#include "Struct.h"




// コマンド
struct Commands {
	ComPtr<ID3D12CommandQueue> Queue;		   // コマンドキュー
	ComPtr<ID3D12CommandAllocator> Allocator; // コマンドアロケータ
	ComPtr<ID3D12GraphicsCommandList> List;   // コマンドリスト
};

// スワップチェーン
struct SwapChains {
	ComPtr<IDXGISwapChain4> swapChain;				// スワップチェーン
	ComPtr<ID3D12Resource> Resources[2];	// スワップチェーンリソース
	DXGI_SWAP_CHAIN_DESC1 Desc{}; // スワップチェーンデスク
};

// RTV
struct RTV {
	ComPtr<ID3D12DescriptorHeap> DescriptorHeap;
	D3D12_RENDER_TARGET_VIEW_DESC Desc{};
	D3D12_CPU_DESCRIPTOR_HANDLE Handles[2];
	D3D12_CPU_DESCRIPTOR_HANDLE StartHandle;
};



/// <summary>
/// DirectXCommonクラス
/// </summary>
class DirectXCommon {

public: // メンバ関数

	/// <summary>
	/// DirectXCommonのインスタンスの取得
	/// </summary>
	static DirectXCommon* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Release();


#pragma region Get 取得

	/// <summary>
	/// 
	/// </summary>
	ID3D12Device* const GetDevice() { return DirectXCommon::GetInstance()->device_.Get(); };

	/// <summary>
	/// 
	/// </summary>
	Commands const GetCommands() { return DirectXCommon::GetInstance()->commands_; }

	/// <summary>
	/// 
	/// </summary>
	ID3D12GraphicsCommandList* const GetCommandList() { return DirectXCommon::GetInstance()->commands_.List.Get(); }

	/// <summary>
	/// 
	/// </summary>
	SwapChains const GetSwapChains() { return DirectXCommon::GetInstance()->swapChains_; }

	/// <summary>
	/// 
	/// </summary>
	RTV const GetRTV() { return DirectXCommon::GetInstance()->rtv_; }

	/// <summary>
	/// 
	/// </summary>
	ID3D12DescriptorHeap* const GetSrvDescriptorHeap() { return DirectXCommon::GetInstance()->srvDescriptorHeap_.Get(); }

	/// <summary>
	/// 
	/// </summary>
	D3D12_DEPTH_STENCIL_DESC const GetDepthStencilDesc() { return DirectXCommon::GetInstance()->depthStencilDesc_; }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	IDxcUtils* const GetDxcUtils() { return DirectXCommon::GetInstance()->dxcUtils_; }

	/// <summary>
	/// 
	/// </summary>
	IDxcCompiler3* const GetDxcCompiler() { return DirectXCommon::GetInstance()->dxcCompiler_; }

	/// <summary>
	/// 
	/// </summary>
	IDxcIncludeHandler* const GetIncludeHandler() { return DirectXCommon::GetInstance()->includeHandler_; }

	/// <summary>
	/// DescriptorHandleを取得する
	/// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

#pragma endregion


#pragma region Set 設定

	/// <summary>
	/// 
	/// </summary>
	void SetDepthStencilDesc(D3D12_DEPTH_STENCIL_DESC depthStencilDesc) { DirectXCommon::GetInstance()->depthStencilDesc_ = depthStencilDesc; }

#pragma endregion


private: // メンバ関数

	// DXGIファクトリーの生成
	static void CreateDxgiFactory();

	// D3D12Deviceの生成
	static void CreateDevice();

	// コマンドキューを生成する
	static void CreateCommandQueue();

	// エラーと警告の抑制
	static void DebugErrorInfoQueue();

	// コマンドアロケータを作成
	static void CreateCommandAllocator();

	// コマンドリストを生成する
	static void CreateCommandList();

	// スワップチェーンを生成する
	static void CreateSwapChain();

	// ディスクリプタヒープを生成する
	static void SetDescriptorHeap();

	// SwapChainからResourceを引っ張ってくる
	static void CreateSwapChainResources();

	// RTVを作る
	static void SettingRTV();

	// 状態を遷移する
	static void ChanegResourceState();

	// Fenceを生成する
	static void MakeFence();


	// ViewportとScissor
	static void SetViewport();

	static void SetScissor();


	// 
	static ID3D12DescriptorHeap* CreateDescriptorHeap(
		ID3D12Device* device,
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDescriptors,
		bool shaderVisible);


	// Textureの深度の設定をしていく
	static ComPtr<ID3D12Resource> CreateDepthStencilTexturerResource(int32_t width, int32_t height);


	// depthStencilResourceを作る
	static void CreateDepthStencilResource();


private: // メンバ変数

	// デバッグレイヤー
	ComPtr<ID3D12Debug1> debugController_ = nullptr;


	// DXGIFactory
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;


	// 使用するアダプタ用の変数
	ComPtr<IDXGIAdapter4> useAdapter_ = nullptr;

	ComPtr<ID3D12Device> device_ = nullptr;


	// エラー・警告・即ち停止
	ComPtr<ID3D12InfoQueue> infoQueue_ = nullptr;

	// なにこれ↓
	D3D12_INFO_QUEUE_FILTER filter_{};

	// コマンド
	Commands commands_;


	// バックバッファインデックス
	UINT backBufferIndex_;


	// TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier_{};


	// スワップチェーン
	SwapChains swapChains_{};


	// ディスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_ = nullptr;


	// RTV
	RTV rtv_{};


	// Fence
	ComPtr<ID3D12Fence> fence_ = nullptr;
	uint64_t fenceValue_ = 0;


	// Event
	HANDLE fenceEvent_;


	// dxcCompilerを初期化
	IDxcUtils* dxcUtils_ = nullptr;

	IDxcCompiler3* dxcCompiler_ = nullptr;

	IDxcIncludeHandler* includeHandler_ = nullptr;


	// ビューポート
	D3D12_VIEWPORT viewport_{};


	// シザー矩形
	D3D12_RECT scissorRect_{};


	// 
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;

	D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc_{};


	// 深度
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;


	// DepthStencilState
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};

	ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
};