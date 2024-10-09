#pragma once

#include "GameObject.h"

/* WavewCountUIクラス */
class WaveCountUI {

public:

	// コンストラクタ、デストラクタ
	WaveCountUI() {};
	~WaveCountUI() {};

	// 初期化処理、描画処理
	void Initialize(int nowCount, int waveMax);
	void Draw2DFront(int nowCount, Camera* camera);

private:

	// ウェーブ
	uint32_t waveNumTexHD_ = 0;
	vector<unique_ptr<Sprite>> waveNumSprite_;
	vector<WorldTransform> waveNumTransform_{};
	Vector2 waveNumTexSize_{};
	uint32_t waveNum_;
	uint32_t eachNum_[2]{};

	// スラッシュ
	uint32_t slashTexHD_ = 0;
	unique_ptr<Sprite> slashSprite_;
	WorldTransform slashTransform_{};

	// ナンバー
	uint32_t numberTexHD_ = 0;
	vector<QuadVertex2> src_;
	Vector2 numberTexSize_{};


	Vector2 spriteSize_{};
};

