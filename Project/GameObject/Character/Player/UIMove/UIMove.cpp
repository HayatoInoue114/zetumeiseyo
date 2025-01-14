#include "UIMove.h"

void UIMove::Init()
{
	worldTransform_.Initialize();

    worldTransform_.translate.x = 200;
}

void UIMove::Update()
{
    if (worldTransform_.translate.x >= 0.0f) {
        worldTransform_.translate.x -= 2;
    }
   

    backArrow_.SetTranslate(worldTransform_.translate);
    pageSelect_.SetTranslate(worldTransform_.translate);
    levelCount_.SetTranslate(worldTransform_.translate);
    selectFrame_.SetTranslate(worldTransform_.translate);
    drawEnergy_.SetTranslate(worldTransform_.translate);
    drawCost_.SetTranslate(worldTransform_.translate);
    bulletLavel_.SetTranslate(worldTransform_.translate);
    operationUI_.SetTranslate(worldTransform_.translate);
}
