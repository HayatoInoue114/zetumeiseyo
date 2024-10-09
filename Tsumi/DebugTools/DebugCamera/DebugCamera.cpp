#include "DebugCamera.h"



// 初期化処理
void DebugCamera::Initialize()
{
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.rotate = Vector3::zero;
	worldTransform_.translate = Vector3::zero;

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.farZ = 1200.0f;
}


// 更新処理
void DebugCamera::Update()
{
	// ワールド座標の更新
	worldTransform_.UpdateMatrix();

	// ビューの更新
	viewProjection_.UpdateMatrix();

	// ビューの計算
	viewProjection_.matView = Inverse(worldTransform_.matWorld);

#ifdef _DEBUG

	if (ImGui::TreeNode("RailCamera"))
	{
		ImGui::DragFloat3("Rotate", &worldTransform_.rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &worldTransform_.translate.x, 0.01f);
		ImGui::TreePop();
	}

#endif // _DEBUG
}
