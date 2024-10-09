#include "DrawSystem.h"



// インスタンス取得
DrawSystem* DrawSystem::GetInstance() {
	static DrawSystem instance;
	return &instance;
}


// 初期化処理
void DrawSystem::Initialize() {

	///// リソースの作成
	// ライン
	lineResource_.Vertex = CreateResource::CreateBufferResource(sizeof(VertexData) * 2);
	lineResource_.Material = CreateResource::CreateBufferResource(sizeof(Vector4));
	lineResource_.VertexBufferView = CreateResource::CreateVertexBufferView(sizeof(VertexData) * 2, lineResource_.Vertex.Get(), 2);
	lineResource_.TransformationMatrix = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));
}


// 描画処理
void DrawSystem::Line(Segment segment, Camera* camera, Vector4 color) {

	VertexData* vertexData = nullptr;
	Vector4* materialData = nullptr;
	TransformationMatrix* transformation = nullptr;

	// 書き込みができるようにする
	DrawSystem::GetInstance()->lineResource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	DrawSystem::GetInstance()->lineResource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	DrawSystem::GetInstance()->lineResource_.TransformationMatrix->Map(0, nullptr, reinterpret_cast<void**>(&transformation));

	// 始点
	vertexData[0].position = { segment.origin.x, segment.origin.y, segment.origin.z, 1.0f };

	// 終点
	vertexData[1].position = { segment.diff.x, segment.diff.y, segment.diff.z, 1.0f };

	// マテリアルの設定
	*materialData = color;

	// Worldの設定
	transformation->World = MakeAffineMatrix(Vector3::one, Vector3::zero, segment.origin);

	// コマンドコール
	DrawSystem::GetInstance()->CommandCallTypeLine(camera);
}


// コマンドコール
void DrawSystem::CommandCallTypeLine(Camera* camera) {

	// コマンドの取得
	Commands commands = DirectXCommon::GetInstance()->GetCommands();

	// RootSignatureを設定。
	commands.List->SetGraphicsRootSignature(LineGraphicPipeline::GetInstance()->GetPsoProperty().rootSignature);
	// PSOを設定
	commands.List->SetPipelineState(LineGraphicPipeline::GetInstance()->GetPsoProperty().graphicsPipelineState);

	///// いざ描画！！！！！
	// VBVを設定
	commands.List->IASetVertexBuffers(0, 1, &DrawSystem::GetInstance()->lineResource_.VertexBufferView);

	// 形状を設定
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// CBVを設定する
	commands.List->SetGraphicsRootConstantBufferView(0, DrawSystem::GetInstance()->lineResource_.Material->GetGPUVirtualAddress());

	// wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, DrawSystem::GetInstance()->lineResource_.TransformationMatrix->GetGPUVirtualAddress());

	// View用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(2, camera->constBuffer->GetGPUVirtualAddress());

	// 描画！(DrawCall / ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commands.List->DrawInstanced(2, 1, 0, 0);
}

