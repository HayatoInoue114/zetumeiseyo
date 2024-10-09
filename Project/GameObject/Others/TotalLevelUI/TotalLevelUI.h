#pragma once

#include "GameObject.h"
#include "SaveValue/SaveValue.h"

/* TotalLevelUIクラス */
class TotalLevelUI {

public:

	// コンストラクタ、デストラクタ
	TotalLevelUI() {};
	~TotalLevelUI() {};

	// 初期化処理、描画処理
	void Initialize();
	void Draw2DFront(Camera* camera);

private:

	// トータル
	uint32_t totalLabelTexHD_ = 0;
	unique_ptr<Sprite> totalLabelSprite_;
	WorldTransform totalLabelTransform_{};

	uint32_t totalNumTexHD_ = 0;
	vector<unique_ptr<Sprite>> totalNumSprite_;
	vector<WorldTransform> totalNumTransform_{};
	Vector2 totalNumTexSize_{};
	uint32_t totalNum_;
	uint32_t eachNum_[2]{};

	Vector2 labelTexSize_{};

	// レベル
	uint32_t levelTexHD_ = 0;
	vector<QuadVertex2> src_;
	Vector2 levelTexSize_{};
};

