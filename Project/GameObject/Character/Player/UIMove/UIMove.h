#pragma once
#include "GameObject.h"
#include "Player/DrawUI/BackScreen/BackScreen.h"
#include "Player/DrawUI/BackArrow/BackArrow.h"
#include "Player/DrawUI/PageSelect/PageSelect.h"
#include "Player/DrawUI/LevelCount/LevelCount.h"
#include "Player/DrawUI/SelectFrame/SelectFrame.h"
#include "Player/DrawUI/DrawEnergy/DrawEnergy.h"
#include "Player/DrawUI/DrawCost/DrawCost.h"
#include "Player/DrawUI/BulletLavel/BulletLavel.h"
#include "Player/DrawUI/OperationUI/OperationUI.h"
class UIMove
{
public:
	void Init();

	void Update();

private:
	WorldTransform worldTransform_;
    BackScreen backScreen_;
    BackArrow backArrow_;
    PageSelect pageSelect_;
    LevelCount levelCount_;
    SelectFrame selectFrame_;
    DrawEnergy drawEnergy_;
    DrawCost drawCost_;
    BulletLavel bulletLavel_;
    OperationUI operationUI_;
};

