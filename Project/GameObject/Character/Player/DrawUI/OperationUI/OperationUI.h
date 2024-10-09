#pragma once

#include "GameObject.h"

/* OperationUIクラス */
class OperationUI {

public:

	// コンストラクタ、デストラクタ
	OperationUI() {};
	~OperationUI() {};

	// 初期化処理、描画処理
	void Initialize();
	void Draw2DFront(Camera* camera);

private:

	// 操作ＵＩ
	vector<uint32_t> texHD_;
	vector<unique_ptr<Sprite>> sprite_;
	vector<WorldTransform> transform_;
	size_t vectorSize_;

	Vector2 texSize_{};
};

