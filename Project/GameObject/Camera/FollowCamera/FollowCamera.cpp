#include "FollowCamera.h"
#include "../../Character/Player/Player.h"


// 初期化処理
void FollowCamera::Init()
{
	// Inputクラス
	input_ = GamePadInput::GetInstance();

	// カメラリソース
	camera_.Initialize();
	camera_.rotate = { 0.2f, 0.0f, 0.0f };

	// 相対位置
	constOffset_ = { 0.0f, 20.0f, -50.0f };

	// オフセットの設定
	playerOffset_ = constOffset_;
}


// 更新処理
void FollowCamera::Update()
{
	// カメラデータの更新
	camera_.UpdateMatrix();

	// stickの入力を受け取る
	iRStick_ = input_->GetRStick();

	// 回転処理
	FuncOrientation();

	// フォロー処理
	FuncFollow();

	// 前方ベクトルと右方ベクトルを求める
	CalcForwardVec();
	CalcRightVec();

#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG
}


// 描画処理
void FollowCamera::Draw3D()
{
}
void FollowCamera::Draw2DFront()
{
}
void FollowCamera::Draw2DBack()
{
}


// 回転処理
void FollowCamera::FuncOrientation()
{
	if (std::abs(iRStick_.x) > DZone_ || std::abs(iRStick_.y) > DZone_) {

		// 目標回転角度
		float addAngle = iRStick_.x * addOrientationSpeed_;

		// 現在の角度と目標角度の計算
		float targetAngle = camera_.rotate.y + addAngle;

		// 現在の角度を目標角度の間を補間
		camera_.rotate.y =
			Lerp(camera_.rotate.y, targetAngle, orientationLerpSpeed_);
	}
}


// フォロー処理
void FollowCamera::FuncFollow()
{
	// オフセットの設定
	playerOffset_ = constOffset_;

	// カメラの前方方向に基づいてカメラのオフセットを回転させる
	Matrix4x4 rotateMat = Matrix4x4::identity;
	rotateMat = MakeRotateYMatrix(camera_.rotate.y);

	// カメラのオフセットの回転
	playerOffset_ = TransformWithPerspective(playerOffset_, rotateMat);

	// プレイヤーの位置にオフセットを加えてカメラの位置を更新
	camera_.translate = player_->GetWorldPos() + playerOffset_;

	// Yの位置だけ固定
	camera_.translate.y = constOffset_.y;
}


// 前方ベクトルを求める
void FollowCamera::CalcForwardVec()
{
	// デフォルトの前方ベクトル
	Vector3 deffForwardVec = Vector3::oneZ;

	// Y軸の回転行列
	Matrix4x4 rotateYMat =
		MakeRotateYMatrix(camera_.rotate.y);

	// 前方ベクトルを求める
	forwardVec_ =
		TransformWithPerspective(deffForwardVec, rotateYMat);
}


// 右方ベクトルを求める
void FollowCamera::CalcRightVec()
{
	// デフォルトの前方ベクトル
	Vector3 deffRightVec = Vector3::oneX;

	// Y軸の回転行列
	Matrix4x4 rotateYMat =
		MakeRotateYMatrix(camera_.rotate.y);

	// 前方ベクトルを求める
	rightVec_ =
		TransformWithPerspective(deffRightVec, rotateYMat);
}


// ImGuiの描画
void FollowCamera::DrawImGui()
{
	if (ImGui::TreeNode("FollowCamera")) {
		ImGui::Text("Camera");
		ImGui::DragFloat3("Rotate", &camera_.rotate.x, 0.1f);
		ImGui::DragFloat3("Translate", &camera_.translate.x, 0.1f);
		ImGui::TreePop();
	}
		
}

