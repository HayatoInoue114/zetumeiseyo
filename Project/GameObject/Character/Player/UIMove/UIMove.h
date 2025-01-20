#pragma once
#include "GameObject.h"

class UIMove
{
public:
	void UIInit();

	void UIUpdate();
    
    void StartMove(const Vector3& start, const Vector3& end, int duration);

protected:
    Vector3 startTranslate_{};    // 移動開始地点（A）
    Vector3 endTranslate_{};      // 移動終了地点（B）
    int currentFrame_ = 0;      // 現在のフレーム数
    int totalFrames_ = 60;      // イージングにかけるフレーム数
    bool isMoving_ = false;     // 移動中かどうか
    Vector3 translate_{};         // 最終的な座標
};

