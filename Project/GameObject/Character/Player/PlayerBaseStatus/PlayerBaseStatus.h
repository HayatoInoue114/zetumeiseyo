#pragma once
#include "Player/PlayerBullet/IPlayerBullet.h"


enum LevelNum {
	LvZero,
	LvOne,
	LvTwo,
	LvThree,
	LvFour,
	LvFive,
};

enum LevelPrope {
	HP,
	Vel,
	Damage,
	Rate,
	Dash,
};


class PlayerBaseStatus {

public:

	// レベルの加算
	int AddLevel(int level);

	// レベルチェック
	int LevelCheck_HP(int nowLv);
	float LevelCheck_Velocity(int nowLv);
	int LevelCheck_Damage(int nowLv);
	int LevelCheck_Rate(int nowLv, PlayerBulletType type);
	float LevelCheck_Dash(int nowLv);
};

