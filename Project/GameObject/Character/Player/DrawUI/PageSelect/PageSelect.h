#pragma once

#include "GameObject.h"

/* PageSelectクラス */
class PageSelect {

public:

	// コンストラクタ、デストラクタ
	PageSelect() {};
	~PageSelect() {};

	// 初期化処理、更新処理、描画処理
	void Initialize();
	void Draw2DFront(int nowPage, Camera* camera);

	void SetTranslate(const Vector3& translate) { translate_ = translate; }
private:

	size_t size_ = 0;
	uint32_t texHD_ = 0;
	vector<unique_ptr<Sprite>> sprite_;
	vector<WorldTransform> transform_;
	vector<WorldTransform> tTransform_;
	Vector3 translate_;
};

