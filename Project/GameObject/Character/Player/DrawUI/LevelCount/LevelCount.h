#pragma once

#include "GameObject.h"
#include "Player/PlayerBaseStatus/PlayerBaseStatus.h"

/* LevelCountクラス */
class LevelCount {

public:

	// コンストラクタ、デストラクタ
	LevelCount() {};
	~LevelCount() {};

	// 初期化処理、描画処理
	void Initialize(int prope, Vector2 pos);
	void Draw2DFront(uint32_t num, Camera* camera);

private:

	// ラベル
	uint32_t lavelTexHD_ = 0;
	unique_ptr<Sprite> lavelSprite_;
	WorldTransform lavelTransform_{};
	Vector2 lavelSpriteSize_{};

	// レベルカウント
	size_t size_ = 0;
	uint32_t countTexHD_ = 0;
	vector<unique_ptr<Sprite>> countSprite_;
	vector<WorldTransform> countTransform_;
}; 