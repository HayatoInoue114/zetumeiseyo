#include "Indicator.h"
#include "Player/Player.h"

void Indicator::Init()
{
    // 相対位置を設定
    constOffset_ = { 0.0f, 20.0f, -50.0f };
    playerOffset_ = constOffset_;

    worldTransform_.Initialize();
    model_ = std::make_unique<Model>();
    model_->CreateFromObj("ind", "indicator");
}

// 更新処理
void Indicator::Update(const Vector3& enemyPosition)
{
    // プレイヤーと敵の位置の差分ベクトルを計算
    Vector3 diff = (enemyPosition - player_->GetWorldPos()).Normalize();

    // プレイヤーの周りを回転するための行列を計算
    FuncFollow(diff);

    // 前方ベクトルと右ベクトルを更新
    CalcForwardVec();
    CalcRightVec();

    worldTransform_.UpdateMatrix();
}

void Indicator::Draw3D(Camera* camera)
{
    //model_->Draw(worldTransform_, camera);
}

// フォロー処理
void Indicator::FuncFollow(const Vector3& directionToEnemy)
{
    // プレイヤーの周りを回転する行列の設定
    float angle = atan2(directionToEnemy.x, directionToEnemy.z);

    // 回転行列を作成
    Matrix4x4 rotateMat = MakeRotateYMatrix(angle);

    // worldTransform_の回転を更新
    worldTransform_.rotate.y = angle;

    // オフセットを回転
    playerOffset_ = TransformWithPerspective(constOffset_, rotateMat);

    // プレイヤーの位置にオフセットを加えてインジケータの位置を設定
    worldTransform_.translate = player_->GetWorldPos() + playerOffset_;

    // Y軸位置の固定
    worldTransform_.translate.y = constOffset_.y;
}

// 前方ベクトルを求める
void Indicator::CalcForwardVec()
{
    Vector3 defaultForwardVec = Vector3::oneZ;
    Matrix4x4 rotateYMat = MakeRotateYMatrix(worldTransform_.rotate.y);
    forwardVec_ = TransformWithPerspective(defaultForwardVec, rotateYMat);
}

// 右方ベクトルを求める
void Indicator::CalcRightVec()
{
    Vector3 defaultRightVec = Vector3::oneX;
    Matrix4x4 rotateYMat = MakeRotateYMatrix(worldTransform_.rotate.y);
    rightVec_ = TransformWithPerspective(defaultRightVec, rotateYMat);
}
