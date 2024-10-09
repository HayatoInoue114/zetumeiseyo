#pragma once

#include "GameObject.h"

/* SelectFrameクラス */
class SelectFrame {

public:

	// コンストラクタ、デストラクタ
	SelectFrame() {};
	~SelectFrame() {};

	// 初期化処理、更新処理、描画処理
	void Initialize();
	void Draw2DFront(uint32_t page, uint32_t num, Camera* camera);

	// 座標のセッター
	void levelVectors_push_back(Vector2 vector) { levelVectors_.push_back(vector); }
	void bulletVectors_push_back(Vector2 vector) { bulletVectors_.push_back(vector); }

private:

	uint32_t texHD_ = 0;
	unique_ptr<Sprite> sprite_;
	WorldTransform transform_{};

	uint32_t AtexHD_ = 0;
	unique_ptr<Sprite> Asprite_;
	WorldTransform Atransform_{};

	vector<Vector2> levelVectors_;
	vector<Vector2> bulletVectors_;
};

