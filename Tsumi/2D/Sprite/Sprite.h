#pragma once

#include "MyMath.h"
#include "Struct.h"
#include "SpriteTransform.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Camera.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteGraphicPipeline.h"
#include "CreateResource.h"
#include "DescriptorManager.h"

//
//enum class SpriteOrigin {
//	TopLeft,
//	Center,
//};
//

class Sprite {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite();


	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Vector2 size = { 128.0f, 128.0f }, Vector4 color = Vector4::one);


	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(uint32_t texHandle, WorldTransform& transform, Camera* camera);

	/// <summary>
	/// 色の変換
	/// </summary>
	Vector4 FloatColor(unsigned int color);


#pragma region Set 設定

	void SetSize(Vector2 size) { size_ = size; }
	void SetUVTransform(UVTransform uvTransform) { uvTransform_ = uvTransform; }
	void SetTextureHandle(uint32_t texHD) { useTexture_ = texHD; }
	void SetColor(Vector4 color) { color_ = color; }
	void SetSrc(QuadVertex2 src) { src_ = src; }
	void SetSpriteOrigin(SpriteOrigin setOrigin) { origin_ = setOrigin; }

#pragma endregion


private:

	/// <summary>
	/// 頂点データを設定する
	/// </summary>
	void SetVertex(WorldTransform transform);


private:

	// スプライトトランスフォーム
	WorldTransform worldTransform_{};

	// サイズ
	Vector2 size_{};

	// テクスチャ
	uint32_t useTexture_{};

	// 色データ
	Vector4 color_;

	// リソース
	ResourcePeroperty resource_{};

	// uvTransform 
	UVTransform uvTransform_;

	// SRC
	QuadVertex2 src_ = {
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
	};

	// 
	SpriteOrigin origin_ = SpriteOrigin::TopLeft;
};


