#pragma once

#include "GameObject.h"

/* BackArrowクラス */
class BackArrow {

public:

	// コンストラクタ、デストラクタ
	BackArrow() {};
	~BackArrow() {};

	// 初期化処理、更新処理、描画処理
	void Initialize();
	void Draw2DFront(int nowPage, Camera* camera);

private:

	size_t size_ = 0;
	vector<uint32_t> texHD_;
	vector<unique_ptr<Sprite>> sprite_;
	vector<WorldTransform> transform_;

	uint32_t labelTexHD_ = 0;
	unique_ptr<Sprite> labelSprite_;
	WorldTransform labelWt_{};
};

