#pragma once

#include "Struct.h"
#include "DirectXCommon.h"



struct DescriptorSize {
	uint32_t SRV;
	uint32_t RTV;
	uint32_t DSV;
};
struct DescriptorSRVHandle {
	D3D12_CPU_DESCRIPTOR_HANDLE _CPU;
	D3D12_GPU_DESCRIPTOR_HANDLE _GPU;
};


/* DescriptorManagerクラス */
class DescriptorManager {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DescriptorManager() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DescriptorManager() {};

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static DescriptorManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// Descriptorがindex数を超えていないか
	/// </summary>
	static void BeginFrame();

	/// <summary>
	/// indexをクリアにする
	/// </summary>
	static void Clear();

	/// <summary>
	/// SRVを作成する
	/// </summary>
	static uint32_t CreateInstancingSRV(uint32_t instancingNum, ComPtr<ID3D12Resource>& resource, UINT size);

	/// <summary>
	/// CPUとGPUの.ptrをずらす
	/// </summary>
	static void ShiftSRVHandlePtr();

	/// <summary>
	/// indexをインクリメント
	/// </summary>
	static void IncrementIndex();

	/// <summary>
	/// DescriptorTableを設定する
	/// </summary>
	static void SetGraphicsRootDescriptorTable(UINT rootPatramerterIndex, uint32_t texHandle);

	/// <summary>
	/// SRVの生成
	/// </summary>
	static void CreateShaderResourceView(ComPtr<ID3D12Resource> resource, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, uint32_t index);


#pragma region Get

	/// <summary>
	/// DescriptorHandleの取得
	/// </summary>
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(
		ComPtr<ID3D12DescriptorHeap> descriptorHeap,
		uint32_t descriptorSize, uint32_t index);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(
		ComPtr<ID3D12DescriptorHeap> descriptorHeap,
		uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// indexの取得
	/// </summary>
	/// <returns></returns>
	static uint32_t GetIndex() { return DescriptorManager::GetInstance()->index_; }

	/// <summary>
	/// descriptorSizeの取得
	/// </summary>
	static DescriptorSize GetDescriptorSize() { return DescriptorManager::GetInstance()->descriptorSize_; }

#pragma endregion 


#pragma region Set

	/// <summary>
	/// srvHandle_._CPUの設定
	/// </summary>
	static void SetDescriptorHandle_CPU(D3D12_CPU_DESCRIPTOR_HANDLE CPU, uint32_t index) {
		DescriptorManager::GetInstance()->srvHandle_[index]._CPU = CPU;
	}

	/// <summary>
	/// srvHandle_._GPUの設定
	/// </summary>
	static void SetDescriptorHandle_GPU(D3D12_GPU_DESCRIPTOR_HANDLE GPU, uint32_t index) {
		DescriptorManager::GetInstance()->srvHandle_[index]._GPU = GPU;
	}

#pragma endregion 


private:

	static const uint32_t descriptor_Max = 64;
	DescriptorSize descriptorSize_{};
	DescriptorSRVHandle srvHandle_[descriptor_Max]{};
	uint32_t index_;
};