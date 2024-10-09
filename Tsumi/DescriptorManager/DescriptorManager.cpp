#include "DescriptorManager.h"



// インスタンスの取得
DescriptorManager* DescriptorManager::GetInstance() {
	static DescriptorManager instance;
	return &instance;
}


// 初期化処理
void DescriptorManager::Initialize() {

	DescriptorManager::GetInstance()->descriptorSize_.SRV = 
		DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DescriptorManager::GetInstance()->descriptorSize_.RTV = 
		DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	DescriptorManager::GetInstance()->descriptorSize_.DSV = 
		DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	DescriptorManager::GetInstance()->index_ = 0;
}


// Descriptorがindex数を超えていないか
void DescriptorManager::BeginFrame() {

	if (DescriptorManager::GetInstance()->index_ >= descriptor_Max) {

		Log("The number of DescripterIndexes has exceeded the limit.");
		assert(0);
	}
}


// indexをクリアにする
void DescriptorManager::Clear() {

	DescriptorManager::GetInstance()->index_ = 0;
}


// SRVを作成する
uint32_t DescriptorManager::CreateInstancingSRV(uint32_t instancingNum, ComPtr<ID3D12Resource>& resource, UINT size) {

	// indexをインクリメント
	DescriptorManager::GetInstance()->index_++;


	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = instancingNum;
	srvDesc.Buffer.StructureByteStride = size;


	// SRVを作成するDescriptorHeapの場所を決める
	DescriptorManager::GetInstance()->srvHandle_[DescriptorManager::GetInstance()->index_]._CPU = GetCPUDescriptorHandle(
		DirectXCommon::GetInstance()->GetSrvDescriptorHeap(),
		DescriptorManager::GetInstance()->descriptorSize_.SRV,
		DescriptorManager::GetInstance()->index_);

	DescriptorManager::GetInstance()->srvHandle_[DescriptorManager::GetInstance()->index_]._GPU = GetGPUDescriptorHandle(
		DirectXCommon::GetInstance()->GetSrvDescriptorHeap(),
		DescriptorManager::GetInstance()->descriptorSize_.SRV,
		DescriptorManager::GetInstance()->index_);


	// 先頭はImGuiが使っているのでその次を使う
	DescriptorManager::GetInstance()->srvHandle_[DescriptorManager::GetInstance()->index_]._CPU.ptr +=
		DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DescriptorManager::GetInstance()->srvHandle_[DescriptorManager::GetInstance()->index_]._GPU.ptr +=
		DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	// SRVの生成
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(
		resource.Get(),
		&srvDesc,
		DescriptorManager::GetInstance()->srvHandle_[DescriptorManager::GetInstance()->index_]._CPU);

	return DescriptorManager::GetInstance()->index_;
}


// CPUとGPUの.ptrをずらす
void DescriptorManager::ShiftSRVHandlePtr() {

	DescriptorManager::GetInstance()->srvHandle_[DescriptorManager::GetInstance()->index_]._CPU.ptr +=
		DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	DescriptorManager::GetInstance()->srvHandle_[DescriptorManager::GetInstance()->index_]._GPU.ptr +=
		DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}


// indexをインクリメント
void DescriptorManager::IncrementIndex() {

	DescriptorManager::GetInstance()->index_++;
}


// DescriptorTableを設定する
void DescriptorManager::SetGraphicsRootDescriptorTable(UINT rootPatramerterIndex, uint32_t texHandle) {

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(
		rootPatramerterIndex, 
		DescriptorManager::GetInstance()->srvHandle_[texHandle]._GPU);
}


// SRVの生成
void DescriptorManager::CreateShaderResourceView(ComPtr<ID3D12Resource> resource, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, uint32_t index) {

	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(
		resource.Get(),
		&srvDesc,
		DescriptorManager::GetInstance()->srvHandle_[index]._CPU);
}


// DescriptorHandleを取得する
// CPU
D3D12_CPU_DESCRIPTOR_HANDLE DescriptorManager::GetCPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (static_cast<unsigned long long>(descriptorSize) * index);
	return handleCPU;
}
// GPU
D3D12_GPU_DESCRIPTOR_HANDLE DescriptorManager::GetGPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (static_cast<unsigned long long>(descriptorSize) * index);
	return handleGPU;
}