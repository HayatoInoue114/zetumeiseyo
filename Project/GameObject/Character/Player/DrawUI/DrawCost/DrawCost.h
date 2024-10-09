#pragma once


#include "GameObject.h"

/* DrawCostクラス */
class DrawCost {

public:

	// コンストラクタ、デストラクタ
	DrawCost() {};
	~DrawCost() {};

	// 初期化処理、更新処理、描画処理
	void Initialize(uint32_t cost);
	void Draw(Camera* camera);


private:

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
	uint32_t eachTimes_[2]{};
};

