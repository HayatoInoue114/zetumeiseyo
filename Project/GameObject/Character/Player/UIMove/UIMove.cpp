#include "UIMove.h"

void UIMove::UIInit()
{

}

void UIMove::UIUpdate()
{
    if (isMoving_) {
        // 現在の進行割合を計算
        float t = static_cast<float>(currentFrame_) / totalFrames_;

        // イージングを適用して移動量を計算
        float easedT = Ease::OutExpo(t);

        // 座標を補間
        translate_ = {
            startTranslate_.x + (endTranslate_.x - startTranslate_.x) * easedT,
            startTranslate_.y + (endTranslate_.y - startTranslate_.y) * easedT,
            startTranslate_.z + (endTranslate_.z - startTranslate_.z) * easedT
        };

        // フレームを進める
        currentFrame_++;

        // 完了判定
        if (currentFrame_ >= totalFrames_) {
            isMoving_ = false; // 移動終了
            translate_ = endTranslate_; // 終点をセット
        }
    }
}

void UIMove::StartMove(const Vector3& start, const Vector3& end, int duration)
{
    startTranslate_ = start;
    endTranslate_ = end;
    currentFrame_ = 0;
    totalFrames_ = duration;
    isMoving_ = true;
}
