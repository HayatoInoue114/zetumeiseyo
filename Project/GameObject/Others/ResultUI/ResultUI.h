#pragma once

#include "GameObject.h"
#include "Player/PlayerBaseStatus/PlayerBaseStatus.h"
#include "SaveValue/SaveValue.h"

/* ResultUIクラス */
class ResultUI {

public:

	// コンストラクタ、デストラクタ
	ResultUI() {};
	~ResultUI() {};

	// 初期化処理、描画処理
	void Initialize(int label, int level, Vector2 pos);
	void Draw2DFront(Camera* camera);


private:

	// ラベル
	uint32_t labelTexHD_ = 0;
	unique_ptr<Sprite> labelSprite_;
	WorldTransform labelTransform_{};
	Vector2 labelTexSize_{};

	// レベル
	uint32_t levelTexHD_ = 0;
	unique_ptr<Sprite> levelSprite_;
	WorldTransform levelTransform_{};
	vector<QuadVertex2> src_;
	Vector2 levelTexSize_{};
	int levelNum_;
};

