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

	void SetTranslate(Vector3 translate) { translate_ = translate; }
private:

	size_t size_ = 0;
	std::vector<uint32_t> texHD_;
	std::vector<std::unique_ptr<Sprite>> sprite_;
	std::vector<WorldTransform> transform_;
	Vector3 translate_;

	uint32_t labelTexHD_ = 0;
	std::unique_ptr<Sprite> labelSprite_;
	WorldTransform labelWt_{};
};

