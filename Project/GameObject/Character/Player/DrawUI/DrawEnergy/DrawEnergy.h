#pragma once


#include "GameObject.h"

/* DrawEnergyクラス */
class DrawEnergy {

public:

	// コンストラクタ、デストラクタ
	DrawEnergy() {};
	~DrawEnergy() {};

	// 初期化処理、更新処理、描画処理
	void Initialize();
	void Draw(uint32_t energy, Camera* camera);

private:

	// 制限時間の各位を求める
	void CalcTimerPlace(uint32_t energy);

	// 各位にあったテクスチャを設定する
	void SetSrc();

private:

	// ラベル
	uint32_t lavelTexHD_ = 0;
	unique_ptr<Sprite> lavelSprite_;
	WorldTransform lavelTransform_{};
	Vector2 lavelSize_{};

	// 数字
	uint32_t numberTexHD_ = 0;
	vector<unique_ptr<Sprite>> numberSprite_;
	vector<WorldTransform> numberTransform_;
	vector<QuadVertex2> src_;
	Vector2 numberSize_{};
	uint32_t eachTimes_[3]{};
};

