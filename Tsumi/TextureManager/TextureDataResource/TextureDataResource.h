#pragma once


#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include "MyMath.h"
#include "Struct.h"

#include <cassert>



// テクスチャデータ
struct TextureData {
	uint32_t index;
	ComPtr<ID3D12Resource> resource;
	Vector2 size;
};



/* TextureDataResouceクラス */
class TextureDataResource {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextureDataResource(std::string filePath, TextureData textureData);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureDataResource();


#pragma region Get

	/// <summary>
	/// テクスチャデータの取得
	/// </summary>
	TextureData GetTextureData() { return textureData_; }

	/// <summary>
	/// テクスチャハンドルの取得
	/// </summary>
	uint32_t GetTextureHandle() { return textureData_.index; }

#pragma endregion 


private:

	std::string filePath_;
	TextureData textureData_{};
};