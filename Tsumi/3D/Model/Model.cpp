#include "Model.h"



/// <summary>
/// 初期化処理
/// </summary>
void Model::Initialize(IModelState* state, WorldTransform worldTransform) {

	// ワールド座標のデフォルト設定
	this->worldTransform_ = worldTransform;

	// テクスチャの初期設定
	this->useTexture_ = 1;

	// 色の設定
	this->color_ = Vector4::one;

	// 光の設定
	this->light_.color = Vector4::one;
	this->light_.direction = { 0.0f, -1.0f, 0.0f };
	//this->light_.SpecularFColor = { 1.0f, 1.0f, 1.0f };
	this->light_.intensity = 1.0f;
	this->light_.sininess = 1.0f;
	this->light_.enableLightting = false;

	// モデルの描画タイプ
	modelDrawType_ = Phong;

	// ステートパターンの初期化処理
	this->state_ = state;
	this->state_->Initialize(this);
}


/// <summary>
/// Objファイルの読み込み & Obj初期化処理
/// </summary>
void Model::CreateFromObj(const std::string& directoryPath, const std::string& routeFilePath, WorldTransform worldTransform) {

	// ワールド座標のデフォルト設定
	this->worldTransform_ = worldTransform;

	// テクスチャの初期設定
	this->useTexture_ = 1;

	// 色の設定
	this->color_ = Vector4::one;

	// 光の設定
	this->light_.color = Vector4::one;
	this->light_.direction = { 0.0f, -1.0f, 0.0f };
	//this->light_.SpecularFColor = { 1.0f, 1.0f, 1.0f };
	this->light_.intensity = 1.0f;
	this->light_.sininess = 1.0f;
	this->light_.enableLightting = false;

	// Objファイルパス
	this->directoryPath_ = directoryPath;
	this->routeFilePath_ = routeFilePath;

	// Objの読み込み
	objData_ = ModelManager::LoadObjFile(directoryPath_, routeFilePath_);

	// モデルの描画タイプ
	modelDrawType_ = Phong;

	// ステートパターンの初期化処理
	this->state_ = new ModelObjState();
	this->state_->Initialize(this);
}


/// <summary>
/// 描画処理
/// </summary>
void Model::Draw(WorldTransform worldTransform, Camera* camera) {

	this->state_->Draw(this, worldTransform, camera);
}