#include "PlayerBaseStatus.h"



// レベルの加算
int PlayerBaseStatus::AddLevel(int Level)
{
	return Level < 5 ? Level + 1 : Level;
}


// レベルチェック
int PlayerBaseStatus::LevelCheck_HP(int nowLv)
{
	// HP
	if (nowLv == LvOne) {
		return 10;
	}
	else if (nowLv == LvTwo) {
		return  20;
	}
	else if (nowLv == LvThree) {
		return  30;
	}
	else if (nowLv == LvFour) {
		return  40;
	}
	else if (nowLv >= LvFive) {
		return 50;
	}

	return 0;
}

float PlayerBaseStatus::LevelCheck_Velocity(int nowLv)
{
	//Vel
	if (nowLv == LvOne) {
		return 0.2f;
	}
	else if (nowLv == LvTwo) {
		return 0.25f;
	}
	else if (nowLv == LvThree) {
		return 0.3f;
	}
	else if (nowLv == LvFour) {
		return 0.35f;
	}
	else if (nowLv >= LvFive) {
		return 0.4f;
	}

	return 0.0f;
}

int PlayerBaseStatus::LevelCheck_Damage(int nowLv)
{
	// Attack
	if (nowLv == LvOne) {
		return 3;
	}
	else if (nowLv == LvTwo) {
		return 5;
	}
	else if (nowLv == LvThree) {
		return 7;
	}
	else if (nowLv == LvFour) {
		return 9;
	}
	else if (nowLv >= LvFive) {
		return 11;
	}

	return 0;
}

int PlayerBaseStatus::LevelCheck_Rate(int nowLv, PlayerBulletType type)
{
	// AttackInterval
	if (nowLv == LvOne) {

		if (type == TypeNormal || type == TypePenetration) {
			return 20;
		}
		else if (type == TypeShotgun) {
			return 40;
		}
	}
	else if (nowLv == LvTwo) {

		if (type == TypeNormal || type == TypePenetration) {
			return 17;
		}
		else if (type == TypeShotgun) {
			return 36;
		}
	}
	else if (nowLv == LvThree) {

		if (type == TypeNormal || type == TypePenetration) {
			return 13;
		}
		else if (type == TypeShotgun) {
			return 32;
		}
	}
	else if (nowLv == LvFour) {

		if (type == TypeNormal || type == TypePenetration) {
			return 10;
		}
		else if (type == TypeShotgun) {
			return 28;
		}
	}
	else if (nowLv >= LvFive) {

		if (type == TypeNormal || type == TypePenetration) {
			return 7;
		}
		else if (type == TypeShotgun) {
			return 10;
		}
	}

	return 0;
}

float PlayerBaseStatus::LevelCheck_Dash(int nowLv)
{
	// Dash
	if (nowLv == LvOne) {
		return 3;
	}
	else if (nowLv == LvTwo) {
		return 4;
	}
	else if (nowLv == LvThree) {
		return 6;
	}
	else if (nowLv == LvFour) {
		return  8;
	}
	else if (nowLv >= LvFive) {
		return 10;
	}

	return 0.0f;
}
