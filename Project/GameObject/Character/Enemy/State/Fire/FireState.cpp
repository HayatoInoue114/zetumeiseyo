#include "FireState.h"
FireState::FireState()
    : fireDuration(7.0f), elapsedTime(0.0f)
{
}

void FireState::Initialize()
{
    elapsedTime = 0.0f;
}

void FireState::Update(float deltaTime)
{
    elapsedTime += deltaTime;
    if (elapsedTime >= fireDuration)
    {
        // 火炎攻撃が終了したときの処理をここに追加
    }
}