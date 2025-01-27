// IBlastState.h
#pragma once
#include "GameObject.h"

class IBlastState
{
public:
    virtual ~IBlastState() = default;
    virtual void Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void SetWorldTransform(const WorldTransform* transform) = 0;
    virtual const WorldTransform* GetWorldTransform() const = 0;

protected:
    WorldTransform worldTransform;
};


