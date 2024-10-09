#include "SpriteTransform.h"



// 初期化処理
void SpriteTransform::Initialize() {
	
	CreateBuffer();
	matWorld = Matrix4x4::identity;
	UpdateMatrix();
}

// 更新処理
void SpriteTransform::UpdateMatrix() {

	// Vector2をVector3に変換してmatWorldを作る
	Vector3 vector3Scale = { scale.x, scale.y, 1.0f };
	Vector3 vector3Rotate = { 0.0f, 0.0f, rotate };
	Vector3 vector3Translate = { translate.x, translate.y, 0.0f };

	// スケール、回転、並列移動を合成して行列を計算する
	matWorld = MakeAffineMatrix(vector3Scale, vector3Rotate, vector3Translate);

	// 行列の計算・転送
	TransferMatrix();
}

// ワールド座標の取得
Vector3 SpriteTransform::GetWorldPos() {
	return { matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2] };
}

// 定数バッファの生成
void SpriteTransform::CreateBuffer() {

	CreateResource::CreateBufferResource(sizeof(TransformationMatrix), constBuffer);

	// constBuffer が nullptr の場合はエラーログを出力してアサーションでプログラムを停止させる
	if (!constBuffer) {
		assert(false && "Failed to create constBuffer.");
	}
}

// マッピングする
void SpriteTransform::Map() {

	// constBuffer が nullptr の場合はエラーハンドリング
	if (!constBuffer) {
		// エラーログなどを出力するか、適切な対処を行う
		// この例ではエラーログを出力し、アサーションでプログラムを停止させる
		assert(false && "constBuff_ is nullptr. Make sure to create constBuffer before calling Map.");
		return;
	}

	HRESULT result = constBuffer->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
	assert(SUCCEEDED(result));
}

// マッピング終了
void SpriteTransform::UnMap() {

	constBuffer->Unmap(0, nullptr);
}

// 行列の計算・転送
void SpriteTransform::TransferMatrix() {

	Map();
	constMap->World = matWorld;
	UnMap();
}
