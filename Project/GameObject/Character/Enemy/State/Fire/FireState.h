#pragma once
#include "../IEnemyState.h"

class FireState : public IEnemyState
{
public:
    FireState();
    void Initialize() override;
    void Update(float deltaTime) override;

private:
    float fireDuration;
    float elapsedTime;
};


