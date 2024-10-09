#include "Field.h"



// インスタンスの取得
Field* Field::GetInstance()
{
    static Field instance;
    return &instance;
}


// 初期化処理
void Field::Initialize()
{
	this->model_ = make_unique<Model>();
	this->model_->CreateFromObj("field");
	this->model_->SetModelDrawType(Non);
	this->worldTransform_.Initialize();
	this->worldTransform_.scale = { 100.0f, 100.0f, 100.0f };
	this->worldTransform_.translate = { 0.0f, -40.0f, 0.0f };
	this->color_ = { 0.7f, 0.1f, 0.1f, 0.1f };
}


// 更新処理
void Field::Update()
{
	// ワールド座標の更新
	this->worldTransform_.UpdateMatrix();

	// カラーの設定
	this->model_->SetColor(this->color_);

#ifdef _DEBUG

	if (ImGui::TreeNode("Field")) {

		ImGui::ColorEdit4("color", &color_.x);
		ImGui::TreePop();
	}
#endif // DEBUG
}


// 描画処理
void Field::Draw(Camera* camera)
{
	this->model_->Draw(this->worldTransform_, camera);
}
