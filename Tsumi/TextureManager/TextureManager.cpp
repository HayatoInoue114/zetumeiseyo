#include "TextureManager.h"



/// <summary>
/// TextureManagerのインスタンス取得
/// </summary>
TextureManager* TextureManager::GetInstance() {

	static TextureManager instance;
	return &instance;
}



/// <summary>
/// 初期化処理
/// </summary>
void TextureManager::Initialize() {

	CoInitializeEx(0, COINIT_MULTITHREADED);
}



/// <summary>
/// 解放処理
/// </summary>
void TextureManager::Finalize() {

	CoUninitialize();

	// コンテナ内のResourceを削除
	UnLoadTexture();
}



/// <summary>
/// Textuerデータを読み込む
/// </summary>
uint32_t TextureManager::LoadTexture(const std::string& routeFilePath, const std::string& filePath, bool fromObjTex) {

	// パスを追加
	std::string FilePath{};
	if (!fromObjTex) {
		FilePath = "Resources/Texture/" + routeFilePath + "/" + filePath;
	}
	else {
		FilePath = "Resources/Obj/" + routeFilePath + "/" + filePath + "/" + filePath + ".png";
	}


	// filePaht同じものがあるならそれを返す
	if (TextureManager::GetInstance()->CheckTextureDatas(FilePath)) {

		// TextureDataを用意
		TextureData textureData{};


		// indexをインクリメント
		DescriptorManager::IncrementIndex();
		const uint32_t index = DescriptorManager::GetIndex();


		// TextureDataに登録
		textureData.index = index;


		// Textureを読んで転送する
		// 1枚目のTextureを読んで転送する
		DirectX::ScratchImage mipImages = CreateMipImage(FilePath);
		const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
		textureData.resource = CreateTextureResource(metadata);

		// 登録
		UpdateTextureData(metadata, mipImages, textureData);


		// metaDataを基にSRVの設定
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc = SettingSRVDesc(metadata);


		// SRVを作成するDescriptorHeapの場所を決める
		DescriptorManager::SetDescriptorHandle_CPU(
			DescriptorManager::GetCPUDescriptorHandle(
				DirectXCommon::GetInstance()->GetSrvDescriptorHeap(),
				DescriptorManager::GetDescriptorSize().SRV, index),
			index);

		DescriptorManager::SetDescriptorHandle_GPU(
			DescriptorManager::GetGPUDescriptorHandle(
				DirectXCommon::GetInstance()->GetSrvDescriptorHeap(),
				DescriptorManager::GetDescriptorSize().SRV, index),
			index);


		// 先頭はImGuiが使っているのでその次を使う
		DescriptorManager::ShiftSRVHandlePtr();


		// SRVの生成
		DescriptorManager::CreateShaderResourceView(
			textureData.resource.Get(), srvDesc, index);


		// textureのサイズの取得
		textureData.size = {
			static_cast<float>(metadata.width),
			static_cast<float>(metadata.height)};


		// コンテナに保存
		TextureManager::GetInstance()->textureDatas_[FilePath] =
			make_unique<TextureDataResource>(FilePath, textureData);
	}


	// ハンドルを返す
	return TextureManager::GetInstance()->textureDatas_[FilePath]->GetTextureHandle();
};



/// <summary>
/// Textureデータの解放
/// </summary>
void TextureManager::UnLoadTexture() {

	TextureManager::GetInstance()->textureDatas_.clear();
}



/// <summary>
/// 一回読み込んだものは読み込まない
/// </summary>
bool TextureManager::CheckTextureDatas(std::string filePath) {

	// filePaht同じものがあるならそれを返す
	if (TextureManager::GetInstance()->textureDatas_.find(filePath) == TextureManager::GetInstance()->textureDatas_.end()) {

		return true;
	}
	
	return false;
}



/// <summary>
/// Textureファイルを開く
/// </summary>
DirectX::ScratchImage TextureManager::CreateMipImage(const std::string& filePath) {

	// テクスチャファイルを読み込んでプログラムで扱えるようにする
	// Textureデータを読み込む
	DirectX::ScratchImage image{};
	std::wstring filePathw = ConverString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathw.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミニマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(
		image.GetImages(), image.GetImageCount(), image.GetMetadata(),
		DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	// ミニマップ付きのデータを探す	
	return mipImages;
}



/// <summary>
/// DirectX12のTExtureResourceを作る
/// </summary>
ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata) {

	// 1.metadataを基にResourceの設定
	// metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc = SettingResource(metadata);

	// 2. 利用するHeapの設定
	// 利用するHeapの設定。非常に特殊な運用 ※02_04exで一般的なケース版がある
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties = SettingUseHeap();

	// 3. Resourceを作る
	// Resourceを生成する
	ComPtr<ID3D12Resource> resource{};
	resource = CreateResource(resourceDesc, heapProperties);

	return resource;
}



/// <summary>
/// metadataを基にResourceの設定
/// </summary>
D3D12_RESOURCE_DESC TextureManager::SettingResource(const DirectX::TexMetadata& metadata) {

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);								// Textureの幅
	resourceDesc.Height = UINT(metadata.height);							// Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);						// mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);			    // 奥行き or 配列Textureの配列数
	resourceDesc.Format = metadata.format;									// TextureのFormat
	resourceDesc.SampleDesc.Count = 1;										// サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);  // Textureの次元数。普段使っているのは2次元

	return resourceDesc;
}



/// <summary>
/// 利用するHeapの設定
/// </summary>
D3D12_HEAP_PROPERTIES TextureManager::SettingUseHeap() {

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;                        // 細かい設定を行う
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;	         // プロセッサの近くに配置

	return heapProperties;
}



/// <summary>
/// Resourceを生成する
/// </summary>
ComPtr<ID3D12Resource> TextureManager::CreateResource(D3D12_RESOURCE_DESC resourceDesc, D3D12_HEAP_PROPERTIES heapProperties) {

	ComPtr<ID3D12Resource> resource;
	HRESULT hr = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,				   // Heapの設定
		D3D12_HEAP_FLAG_NONE,			   // Heapの特殊な設定。特になし
		&resourceDesc,					   // Resourceの設定
		D3D12_RESOURCE_STATE_GENERIC_READ, // 初回のResourceState。Textureは基本読むだけ
		nullptr,						   // Clear最適地。使わないのでnullptr
		IID_PPV_ARGS(&resource));		   // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	return resource;
}



/// <summary>
/// TextureResourceにデータを転送する
/// </summary>
void TextureManager::UpdateTextureData(const DirectX::TexMetadata& metadata, DirectX::ScratchImage& mipImages, TextureData textureData) {

	// Meta情報を取得
	//const DirectX::TexMetadata& metadata = mipImage.GetMetadata();

	// 全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {

		//MipMapLevelを指定して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);

		// Textureに転送
		HRESULT hr_ = textureData.resource->WriteToSubresource(
			UINT(mipLevel),
			nullptr,			  // 全領域へコピー
			img->pixels,		  // 元データアドレス
			UINT(img->rowPitch),  // 1ラインサイズ
			UINT(img->slicePitch) // 1枚サイズ
		);
		assert(SUCCEEDED(hr_));
	}
}



/// <summary>
/// metaDataを基にSRVの設定
/// </summary>
D3D12_SHADER_RESOURCE_VIEW_DESC TextureManager::SettingSRVDesc(const DirectX::TexMetadata& metadata) {

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	return srvDesc;
}
