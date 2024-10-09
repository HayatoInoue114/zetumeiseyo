#pragma once

#include "GameObject.h"


/* BackScreenクラス */
class BackScreen {

public:

	// コンストラクタ、デストラクタ
	BackScreen() {};
	~BackScreen() {};

	// 初期化処理、更新処理、描画処理
	void Initialize();
	void Draw2DFront(Camera* camera);

private:

	uint32_t texHD_ = 0;
	unique_ptr<Sprite> sprite_;
	WorldTransform transform_{};
};

