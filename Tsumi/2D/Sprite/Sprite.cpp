#include "Sprite.h"



/// <summary>
/// デストラクタ
/// </summary>
Sprite::~Sprite() {}


/// <summary>
/// 初期化処理
/// </summary>
void Sprite::Initialize(Vector2 size, Vector4 color) {

	// 座標の設定
	worldTransform_.translate = Vector3::zero;

	// サイズの設定
	size_ = size;

	// 色の設定
	color_ = color;

	// uvTransformの設定
	uvTransform_ = {
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
	};

	// テクスチャの設定
	// デフォルトではuvCheckerを使う
	useTexture_ = 1;

	// 
	origin_ = SpriteOrigin::TopLeft;

	// リソースの作成
	resource_.Vertex = CreateResource::CreateBufferResource(sizeof(VertexData) * 4);
	resource_.Material = CreateResource::CreateBufferResource(sizeof(MaterialSprite));
	resource_.VertexBufferView = CreateResource::CreateVertexBufferView(sizeof(VertexData) * 4, resource_.Vertex.Get(), 4);

	resource_.Index = CreateResource::CreateBufferResource(sizeof(uint32_t) * 6);
	resource_.IndexBufferView = CreateResource::CreateIndexBufferview(sizeof(uint32_t) * 6, resource_.Index.Get());
}


/// <summary>
/// 描画処理
/// </summary>
void Sprite::Draw(uint32_t texHandle, WorldTransform& transform, Camera* camera) {

	// 頂点データを設定する
	SetVertex(transform);

	// RootSignatureを設定。
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(SpriteGraphicPipeline::GetInstance()->GetPsoProperty().rootSignature);
	// PSOを設定
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(SpriteGraphicPipeline::GetInstance()->GetPsoProperty().graphicsPipelineState);

	// 頂点の設定
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &resource_.VertexBufferView); // VBVを設定
	DirectXCommon::GetInstance()->GetCommandList()->IASetIndexBuffer(&resource_.IndexBufferView);

	// 形状を設定
	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Material用のCBufferの場所を設定
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	// wvp用のCBufferの場所を設定
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transform.constBuffer->GetGPUVirtualAddress());

	// View用のCBufferの場所を設定
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera->constBuffer->GetGPUVirtualAddress());

	// DescriptorTableを設定する
	if (!texHandle == 0) {
		DescriptorManager::SetGraphicsRootDescriptorTable(3, texHandle);
	}

	// 描画！(DrawCall/ドローコール)
	DirectXCommon::GetInstance()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}



/// <summary>
/// 頂点データを設定する
/// </summary>
void Sprite::SetVertex(WorldTransform transform) {

	VertexData* vertexData = nullptr;
	MaterialSprite* materialData = nullptr;
	uint32_t* indexData = nullptr;

	// 書き込みができるようにする
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	resource_.Index->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	// ワールドトランスフォームの設定
	worldTransform_ = transform;


	switch (origin_) 
	{
	case SpriteOrigin::TopLeft:

		// 左下
		vertexData[0].position = { 0.0f, size_.y, 0.0f, 1.0f };

		// 左上
		vertexData[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };

		// 右下
		vertexData[2].position = { size_.x, size_.y, 0.0f, 1.0f };

		// 右上
		vertexData[3].position = { size_.x, 0.0f, 0.0f, 1.0f };
		break;

	case SpriteOrigin::Center:

		// 左下
		vertexData[0].position = { -size_.x / 2.0f, size_.y / 2.0f, 0.0f, 1.0f };

		// 左上
		vertexData[1].position = { -size_.x / 2.0f, -size_.y / 2.0f, 0.0f, 1.0f };

		// 右下
		vertexData[2].position = { size_.x / 2.0f, size_.y / 2.0f, 0.0f, 1.0f };

		// 右上
		vertexData[3].position = { size_.x / 2.0f, -size_.y / 2.0f, 0.0f, 1.0f };
		break;
	}


	// 左下
	vertexData[0].texCoord = src_.LeftDown;

	// 左上
	vertexData[1].texCoord = src_.LeftUp;


	// 右下
	vertexData[2].texCoord = src_.RightDown;

	// 右上
	vertexData[3].texCoord = src_.RightUp;


	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;


	materialData->color = color_;
	materialData->uvTransform = MakeAffineMatrix(uvTransform_.scale, uvTransform_.rotate, uvTransform_.translate);
}


/// <summary>
/// 色の変換
/// </summary>
Vector4 Sprite::FloatColor(unsigned int color) {

	Vector4 colorf = {
		((color >> 24) & 0xff) / 255.0f, // R
		((color >> 16) & 0xff) / 255.0f, // G
		((color >> 8) & 0xff) / 255.0f,  // B
		((color >> 0) & 0xff) / 255.0f   // A
	};

	return colorf;
}