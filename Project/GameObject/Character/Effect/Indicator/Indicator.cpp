#include "Indicator.h"
#include "Player/Player.h"

Indicator::Indicator()
{
}

Indicator::~Indicator()
{
}


void Indicator::Init()
{
    // 相対位置を設定
    constOffset_ = { 0.0f, 0.0f, 3.0f };
    playerOffset_ = constOffset_;

    colorBlinkRange_ = 20;
    count_ = -colorBlinkRange_;

    worldTransform_.Initialize();
    model_ = std::make_unique<Model>();
    model_->CreateFromObj("ind", "indicator");

    worldTransform_.scale = {};

    isAlive_ = true;
}

// 更新処理
void Indicator::Update(const Vector3& enemyPosition)
{
    enemyPos_ = enemyPosition;
    if (!isAlive_) {
        return;
    }
    worldTransform_.scale = { 0.3f,0.3f,0.3f };
    // プレイヤーと敵の位置の差分ベクトルを計算
    Vector3 diff = (enemyPosition - player_->GetWorldPos()).Normalize();

    // プレイヤーの周りを回転するための行列を計算
    FuncFollow(diff);

    // 前方ベクトルと右ベクトルを更新
    CalcForwardVec();
    CalcRightVec();

    ColorChange();

    worldTransform_.UpdateMatrix();

#ifdef _DEBUG
    // ImGuiの描画
    DrawImGui();
#endif // _DEBUG
}

void Indicator::Draw3D(Camera* camera)
{
    if (!isAlive_) {
        return;
    }
    model_->Draw(worldTransform_, camera);
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

// ImGuiの描画
void Indicator::DrawImGui()
{
    if (ImGui::TreeNode("Indicator")) {
        ImGui::DragFloat3("Rotate", &worldTransform_.rotate.x, 0.1f);
        ImGui::DragFloat3("Translate", &worldTransform_.translate.x, 0.1f);
        ImGui::TreePop();
    }

}

void Indicator::ColorChange() {
    float distance = CalculateEuclideanDistance(player_->GetWorldPos(), enemyPos_);

    colorBlinkRange_ = distance;

    if (distance <= 20.0f) {
        count_+= 0.3f;
        //色を何秒で変化させるか
        if (count_ < 0) {
            isColor_ = true;
        }
        if (count_ >= 0) {
            isColor_ = false;
        }

        if (count_ >= colorBlinkRange_) {
            count_ = -colorBlinkRange_;
        }

        if (isColor_) {
            color_ = { 1,0,0,1 };
        }
        else {
            color_ = { 1,1,1,1 };
        }
    }
    else {
        color_ = { 1,1,1,1 };
    }

    model_->SetColor(color_);

    ImGui::Begin("dis");
    ImGui::Text("%f", distance);
    ImGui::End();
}

float Indicator::CalculateEuclideanDistance(const Vector3& point1, const Vector3& point2) {
    float deltaX = point2.x - point1.x;
    float deltaY = point2.y - point1.y;
    float deltaZ = point2.z - point1.z;

    return std::sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}