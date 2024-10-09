#pragma once

#include "GameObject.h"

/* BulletLavelクラス */
class BulletLavel {

public:

	// コンストラクタ、デストラクタ
	BulletLavel() {};
	~BulletLavel() {};

	// 初期化処理、更新処理、描画処理
	void Initialize(int num, Vector2 pos);
	void Draw2DFront(int nowPage, Camera* camera);

private:

	uint32_t texHD_;
	unique_ptr<Sprite> sprite_;
	WorldTransform transform_;

	Vector2 size_{};
};

