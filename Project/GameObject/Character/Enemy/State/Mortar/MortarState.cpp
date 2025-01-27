#include "MortarState.h"
MortarState::MortarState()
    : mortarDuration(10.0f), elapsedTime(0.0f)
{
}

void MortarState::Initialize()
{
    elapsedTime = 0.0f;
}

void MortarState::Update(float deltaTime)
{
    elapsedTime += deltaTime;
    if (elapsedTime >= mortarDuration)
    {
        // 迫撃砲の動作が終了したときの処理をここに追加
    }
}