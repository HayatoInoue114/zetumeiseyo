#include "ModelObjState.h"
#include "Model.h"


/// <summary>
/// 初期化処理
/// </summary>
void ModelObjState::Initialize(Model* pModel) {

	pModel;

	modelData_.material = pModel->GetObjData().material;
	modelData_.vertices = pModel->GetObjData().vertices;

	// リソースの作成
	//modelData_ = LoadObjFile(pModel, pModel->GetObjDirectoryPath());
	resource_.Vertex = CreateResource::CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	resource_.Material = CreateResource::CreateBufferResource(sizeof(Material));
	resource_.Lighting = CreateResource::CreateBufferResource(sizeof(DirectionalLight));
	resource_.VertexBufferView = CreateResource::CreateVertexBufferView(sizeof(VertexData) * modelData_.vertices.size(), resource_.Vertex.Get(), int(modelData_.vertices.size()));
}


/// <summary>
/// 描画処理
/// </summary>
void ModelObjState::Draw(Model* pModel, WorldTransform worldTransform, Camera* camera) {

	VertexData* vertexData = nullptr;
	Material* material = nullptr;
	DirectionalLight* lightData = nullptr;


	// 書き込むためにアドレスを取得
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&material));
	resource_.Lighting->Map(0, nullptr, reinterpret_cast<void**>(&lightData));


	// 頂点データをリソースにコピー
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	// マテリアルの情報を書き込む
	material->color = pModel->GetColor();

	// ライティングの設定
	lightData->color = pModel->GetDirectionalLight().color;
	lightData->direction = pModel->GetDirectionalLight().direction;
	//lightData->SpecularFColor = pModel->GetDirectionalLight().SpecularFColor;
	lightData->intensity = pModel->GetDirectionalLight().intensity;
	lightData->sininess = pModel->GetDirectionalLight().sininess;
	lightData->enableLightting = pModel->GetDirectionalLight().enableLightting;


	// コマンドコール
	CommandCall(pModel, worldTransform, camera);
}


/// <summary>
/// コマンドコール処理
/// </summary>
void ModelObjState::CommandCall(Model* pModel, WorldTransform worldTransform, Camera* camera) {

	// コマンドの取得
	Commands commands = DirectXCommon::GetInstance()->GetCommands();

	if (pModel->GetModelDrawType() == Non) {

		// RootSignatureを設定。
		commands.List->SetGraphicsRootSignature(PhongGraphicPipeline::GetInstance()->GetPsoProperty().rootSignature);
		// PSOを設定
		commands.List->SetPipelineState(PhongGraphicPipeline::GetInstance()->GetPsoProperty().graphicsPipelineState);
	}
	else if (pModel->GetModelDrawType() == Lambert) {

		// RootSignatureを設定。
		commands.List->SetGraphicsRootSignature(LambertGraphicPipeline::GetInstance()->GetPsoProperty().rootSignature);
		// PSOを設定
		commands.List->SetPipelineState(LambertGraphicPipeline::GetInstance()->GetPsoProperty().graphicsPipelineState);
	}
	else if (pModel->GetModelDrawType() == Phong) {

		// RootSignatureを設定。
		commands.List->SetGraphicsRootSignature(PhongGraphicPipeline::GetInstance()->GetPsoProperty().rootSignature);
		// PSOを設定
		commands.List->SetPipelineState(PhongGraphicPipeline::GetInstance()->GetPsoProperty().graphicsPipelineState);
	}
	else if (pModel->GetModelDrawType() == PhongNormalMap) {

		// RootSignatureを設定。
		commands.List->SetGraphicsRootSignature(PhongNormalMap::GetInstance()->GetPsoProperty().rootSignature);
		// PSOを設定
		commands.List->SetPipelineState(PhongNormalMap::GetInstance()->GetPsoProperty().graphicsPipelineState);
	}


	///// いざ描画！！！！！
	// VBVを設定
	commands.List->IASetVertexBuffers(0, 1, &resource_.VertexBufferView);

	// 形状を設定
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// CBVを設定する
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	// wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, worldTransform.constBuffer->GetGPUVirtualAddress());

	// View用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(2, camera->constBuffer->GetGPUVirtualAddress());

	// DescriptorTableを設定する
	if (!pModel->GetObjData().textureHD == 0) {
		DescriptorManager::SetGraphicsRootDescriptorTable(3, pModel->GetObjData().textureHD);
	}

	// 光用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(4, resource_.Lighting->GetGPUVirtualAddress());

	// ノーマルマップ用のテクスチャの設定
	if (pModel->GetModelDrawType() == PhongNormalMap) {
		DescriptorManager::SetGraphicsRootDescriptorTable(5, pModel->GetNormalMapTex());
	}

	// 描画！(DrawCall / ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commands.List->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}
