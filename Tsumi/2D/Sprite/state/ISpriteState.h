#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"


// Spriteの前方宣言
class Sprite;


/* ISpriteStateクラス */
class ISpriteState {

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ISpriteState() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(Sprite* state) = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(Sprite* sprite, WorldTransform worldTransform, ViewProjection viewProjection) = 0;
};

