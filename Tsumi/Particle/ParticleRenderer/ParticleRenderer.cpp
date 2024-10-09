#include "ParticleRenderer.h"



// 　初期化処理
void ParticleRenderer::Initialize(uint32_t maxInstance)
{
	// 使うテクスチャは設定していなかったらuvChecker
	useTexture_ = 0;

	// 最大描画数
	instanceNum_ = maxInstance;

	// リソースの作成
	resource_.Vertex = CreateResource::CreateBufferResource(sizeof(VertexData) * 4);
	resource_.Material = CreateResource::CreateBufferResource(sizeof(MaterialParticle));
	resource_.VertexBufferView = CreateResource::CreateVertexBufferView(sizeof(VertexData) * 4, resource_.Vertex.Get(), 4);

	resource_.Index = CreateResource::CreateBufferResource(sizeof(uint32_t) * 6);
	resource_.IndexBufferView = CreateResource::CreateIndexBufferview(sizeof(uint32_t) * 6, resource_.Index.Get());

	resource_.instancing = CreateResource::CreateBufferResource(sizeof(ParticleTransformationMatrix) * instanceNum_);
	dsvIndex_ = DescriptorManager::CreateInstancingSRV(instanceNum_, resource_.instancing, sizeof(ParticleTransformationMatrix));
}


// 描画処理
void ParticleRenderer::Draw(const list<unique_ptr<IParticle>>& p, uint32_t useTexture, Camera* camera)
{
	VertexData* vertexData = nullptr;
	MaterialParticle* materialData = nullptr;
	uint32_t* indexData = nullptr;
	ParticleTransformationMatrix* instancingData = nullptr;

	// 書き込みができるようにする
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	resource_.Index->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	resource_.instancing->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));


	// 左下
	vertexData[0].position = { pos_.x - size_, pos_.y - size_, pos_.z, 1.0f };
	vertexData[0].texCoord = { 0.0f, 1.0f };
	vertexData[0].normal = { 0.0f, 1.0f, 0.0f };

	// 左上
	vertexData[1].position = { pos_.x - size_, pos_.y + size_, pos_.z, 1.0f };
	vertexData[1].texCoord = { 0.0f, 0.0f };
	vertexData[1].normal = { 0.0f, 1.0f, 0.0f };

	// 右下
	vertexData[2].position = { pos_.x + size_, pos_.y - size_, pos_.z, 1.0f };
	vertexData[2].texCoord = { 1.0f, 1.0f };
	vertexData[2].normal = { 0.0f, 1.0f, 0.0f };

	// 右上
	vertexData[3].position = { pos_.x + size_, pos_.y + size_, pos_.z, 1.0f };
	vertexData[3].texCoord = { 1.0f, 0.0f };
	vertexData[3].normal = { 0.0f, 1.0f, 0.0f };


	// 頂点インデックス
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;


	// マテリアルの設定
	materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData->uvTransform =
		MakeAffineMatrix(Vector3::one, Vector3::zero, Vector3::zero);

	
	// Textureの設定
	useTexture_ = useTexture;


	instanceNum_ = 0;
	for (list<unique_ptr<IParticle>>::const_iterator itr = p.begin(); itr != p.end(); ++itr) {

		// 最大描画数を超えないようにする
		if (instanceNum_ <= kMaxInstanceNum_) {

			// ビルボードの計算
			Matrix4x4 billMat = CalcBillBord(camera, (*itr)->GetTransform().rotate);
			Matrix4x4 scaleMat = MakeScaleMatrix((*itr)->GetTransform().scale);
			Matrix4x4 translateMat = MakeTranslateMatrix((*itr)->GetTransform().translate);
			Matrix4x4 worldPos = scaleMat * (billMat * translateMat);
			Matrix4x4 worldView = camera->matView * camera->matProjection;
			Matrix4x4 matWorld = worldPos * worldView;

			(*itr)->GetUvTransform().matWorld = MakeAffineMatrix(
				(*itr)->GetUvTransform().scale, (*itr)->GetUvTransform().rotate, (*itr)->GetUvTransform().translate);

			instancingData[instanceNum_].World = matWorld;
			instancingData[instanceNum_].Color = (*itr)->GetColor();
			instancingData[instanceNum_].uvTansform = (*itr)->GetUvTransform().matWorld;
			instanceNum_++;
		}
	}

	// コマンドコール
	CommandCall();
}


// コマンドコール
void ParticleRenderer::CommandCall()
{
	// コマンドの取得
	Commands commands = DirectXCommon::GetInstance()->GetCommands();

	// RootSignatureを設定。
	commands.List->SetGraphicsRootSignature(ParticleGraphicPipeline::GetInstance()->GetPsoProperty().rootSignature);
	// PSOを設定
	commands.List->SetPipelineState(ParticleGraphicPipeline::GetInstance()->GetPsoProperty().graphicsPipelineState);

	///// いざ描画！！！！！
	// VBVを設定
	commands.List->IASetVertexBuffers(0, 1, &resource_.VertexBufferView);
	commands.List->IASetIndexBuffer(&resource_.IndexBufferView);

	// 形状を設定
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// CBVを設定する
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	// DescriptorTableを設定する
	DescriptorManager::SetGraphicsRootDescriptorTable(1, dsvIndex_);

	// Texture
	if (!useTexture_ == 0) {
		DescriptorManager::SetGraphicsRootDescriptorTable(2, useTexture_);
	}

	// 描画！(DrawCall / ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commands.List->DrawIndexedInstanced(6, instanceNum_, 0, 0, 0);
}


// ビルボードの処理
Matrix4x4 ParticleRenderer::CalcBillBord(Camera* camera, Vector3 rotate)
{
	// X軸とY軸のビルボード行列を計算
	Matrix4x4 viewMat = camera->matView;
	Matrix4x4 invViewMat = Inverse(viewMat);

	// Z軸の回転を保持するための行列
	Matrix4x4 rotZMat = MakeRotateZMatrix(rotate.z);

	// カメラの回転を無視したビルボード行列を計算
	Matrix4x4 billBoardMat = invViewMat * rotZMat;
	billBoardMat.m[3][0] = 0.0f;
	billBoardMat.m[3][1] = 0.0f;
	billBoardMat.m[3][2] = 0.0f;
	return billBoardMat;
}

