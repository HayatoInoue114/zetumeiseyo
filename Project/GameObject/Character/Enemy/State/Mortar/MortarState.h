#pragma once
#include "../IEnemyState.h"

class MortarState : public IEnemyState
{
public:
    MortarState();
    void Initialize() override;
    void Update(float deltaTime) override;

private:
    float mortarDuration;
    float elapsedTime;
};


