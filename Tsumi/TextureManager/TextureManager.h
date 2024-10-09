#pragma once

#include "DirectXTex.h"
#include "DirectXCommon.h"
#include "DescriptorManager.h"
#include "TextureDataResource.h"
#include "MyMath.h"

#include <cassert>
#include <map>


class TextureManager {

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextureManager() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureManager() {};

	/// <summary>
	/// TextureManagerのインスタンス取得
	/// </summary>
	static TextureManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// Textuerデータを読み込む
	/// </summary>
	static uint32_t LoadTexture(const std::string& routeFilePath, const std::string& filePath, bool fromObjTex = false);

	/// <summary>
	/// Textureデータの解放
	/// </summary>
	static void UnLoadTexture();


private: // メンバ関数

	/// <summary>
	/// 一回読み込んだものは読み込まない
	/// </summary>
	static bool CheckTextureDatas(std::string filePath);

	/// <summary>
	/// Textureファイルを開く
	/// </summary>
	static DirectX::ScratchImage CreateMipImage(const std::string& filePath);

	/// <summary>
	/// DirectX12のTextureResourceを作る
	/// </summary>
	static ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// TextureResourceにデータを転送する
	/// </summary>
	static void UpdateTextureData(const DirectX::TexMetadata& metadata, DirectX::ScratchImage& mipImages, TextureData textureData);

	/// <summary>
	/// metaDataを基にSRVの設定
	/// </summary>
	static D3D12_SHADER_RESOURCE_VIEW_DESC SettingSRVDesc(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// metadataを基にResourceの設定
	/// </summary>
	static D3D12_RESOURCE_DESC SettingResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// 利用するHeapの設定
	/// </summary>
	static D3D12_HEAP_PROPERTIES SettingUseHeap();

	/// <summary>
	/// Resourceを生成する
	/// </summary>
	static ComPtr<ID3D12Resource> CreateResource(D3D12_RESOURCE_DESC resourceDesc, D3D12_HEAP_PROPERTIES heapProperties);


private: // メンバ変数

	// ロードできるテクスチャの最大数
	const static uint32_t TexLoadMax = 128;

	// デスクリプターヒープの場所指定のインデックス
	uint32_t LoadTextureIndex_ = 0;

	// リソース
	ComPtr<ID3D12Resource> Resource_[TexLoadMax];


	map<std::string, unique_ptr<TextureDataResource>> textureDatas_;
};