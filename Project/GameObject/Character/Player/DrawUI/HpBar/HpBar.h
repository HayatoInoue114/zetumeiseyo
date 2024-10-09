#pragma once

#include "GameObject.h"


// Playerの前方宣言
class Player;


/* HpBarクラス */
class HpBar {

public:

	// コンストラクタ、デストラクタ
	HpBar() {};
	~HpBar() {};

	// 初期化処理、描画処理
	void Initialize();
	void Draw2DFront(Camera* camera);

#pragma region Set

	void SetPlayer(Player* setPlayer) { this->player_ = setPlayer; }

#pragma endregion 

private:

	// HpBarの調整
	void AdjustmentHpBarSize();

private:

	// Player
	Player* player_ = nullptr;

	// HpBar
	uint32_t hpBarTexHD_ = 0;
	unique_ptr<Sprite> hpBarSprite_;
	WorldTransform hpBarTransform_{};
	Vector2 hpBarInitTexSize_{};
	Vector2 hpBarTexSize_{};

	// HpBarFrame
	uint32_t frameTexHD_ = 0;
	unique_ptr<Sprite> frameSprite_;
	WorldTransform frameTransform_{};
	Vector2 frameInitTexSize_{};
};

