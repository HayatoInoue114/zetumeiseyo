// ChaceForStreightState.h
#pragma once
#include "../IBlastState.h"
#include "Enemy/Blast/Blast.h"

class ChaceForStreightState : public IBlastState
{
public:
    ChaceForStreightState(Blast* blast);

    void Initialize() override;
    void Update(float deltaTime) override;
    void SetWorldTransform(const WorldTransform* transform) override;
    const WorldTransform* GetWorldTransform() const override;

private:
    Blast* blast_;
};


