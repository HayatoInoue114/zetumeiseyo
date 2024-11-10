#include "Camera.h"
#include "FollowCamera/FollowCamera.h"


// 初期化処理
void Camera::Initialize(Vector3 initRotate, Vector3 initTranslate)
{
	rotate = initRotate;
	translate = initTranslate;

	CreateBuffer();
	Map();
	UpdateMatrix();
}


// 行列の更新処理
void Camera::UpdateMatrix()
{
	rotateMat = MakeRotateXYZMatrix(rotate);
	translateMat = MakeTranslateMatrix(translate);
	matWorld = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);

	matView = Inverse(translateMat) * Inverse(rotateMat);
	matProjection = MakePerspectiveFovMatrix(fov, aspectRatio, nearZ, farZ);

	orthoGraphicMat = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 100.0f);

	matViewPort = MakeViewportMatrix(0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 1.0f);
	matViewProjectionViewPort = matView * (matProjection * matViewPort);
	matInverseVPV = Inverse(matViewProjectionViewPort);

	TransferMatrix();
}


// ワールド座標の取得
Vector3 Camera::GetWorldPos()
{
	return { matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2] };
}


// 定数バッファの生成
void Camera::CreateBuffer()
{
	CreateResource::CreateBufferResource(sizeof(TransformationViewMatrix), constBuffer);

	// constBuffer が nullptr の場合はエラーログを出力してアサーションでプログラムを停止させる
	if (!constBuffer) {
		assert(false && "Failed to create constBuffer.");
	}
}


// マッピングする
void Camera::Map()
{
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
void Camera::UnMap()
{
	constBuffer->Unmap(0, nullptr);
}


// 行列の計算・転送
void Camera::TransferMatrix()
{
	Map();

	constMap->view = matView;
	constMap->viewProjection = matProjection;
	constMap->orthoGraphic = orthoGraphicMat;
	constMap->cameraPosition = translate;

	UnMap();
}

//void Camera::LookAt(const Vector3& targetPosition) {
//	// カメラの方向ベクトル（ターゲットへの方向）
//	Vector3 forward = (targetPosition - translate).Normalize();
//
//	// 上方向ベクトル（ワールドのY軸を基準に設定）
//	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
//
//	// サイド方向ベクトル（上方向と前方向の外積で計算）
//	Vector3 right = up.Cross(forward).Normalize();
//
//	// カメラの新しい上方向ベクトル（前方向と右方向の外積）
//	up = forward.Cross(right);
//
//	// カメラのビュー行列を設定
//	matView = Matrix4x4(
//		right.x, right.y, right.z, 0.0f,
//		up.x, up.y, up.z, 0.0f,
//		-forward.x, -forward.y, -forward.z, 0.0f,
//		0.0f, 0.0f, 0.0f, 1.0f
//	);
//
//	// カメラ位置の平行移動を行列に適用
//	matView.Translate(-translate.x, -translate.y, -translate.z);
//}

void Camera::LookAt(const Vector3& targetPosition, const Vector3& forwardVec, const Vector3& rightVec, const Vector3& up = { 0.0f, 1.0f, 0.0f }) {
	// 前方ベクトルを正規化
	Vector3 forward = forwardVec.Normalize();

	// 上向きベクトルを正規化
	Vector3 upNormalized = up.Normalize();

	// 右ベクトルを正規化
	Vector3 right = rightVec.Normalize();

	// 新しい上向きベクトルを計算
	Vector3 newUp = forward.Cross(right).Normalize();

	// ビュー行列を設定
	matView.m[0][0] = right.x;
	matView.m[1][0] = right.y;
	matView.m[2][0] = right.z;
	matView.m[3][0] = -right.Dot(translate);

	matView.m[0][1] = newUp.x;
	matView.m[1][1] = newUp.y;
	matView.m[2][1] = newUp.z;
	matView.m[3][1] = -newUp.Dot(translate);

	matView.m[0][2] = -forward.x;
	matView.m[1][2] = -forward.y;
	matView.m[2][2] = -forward.z;
	matView.m[3][2] = forward.Dot(translate);

	matView.m[3][3] = 1.0f;

	// カメラの回転を計算 (ターゲットと上向きベクトルから回転を計算)
	float angleY = atan2(forward.x, forward.z); // Y軸回転 (水平方向)
	float angleX = atan2(forward.y, sqrt(forward.x * forward.x + forward.z * forward.z)); // X軸回転 (垂直方向)

	// 回転をcamera_->rotateに反映
	rotate = { angleX, angleY, 0.0f }; // Z軸回転は0に設定 (今回は不要)
}


