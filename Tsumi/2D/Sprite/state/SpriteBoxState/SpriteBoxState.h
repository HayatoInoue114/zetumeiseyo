#pragma once

#include "ISpriteState.h"
#include "CreateResource.h"
#include "SpriteGraphicPipeline.h"
#include "ShaderManager.h"
#include "TextureManager.h"


/* SpriteBoxStateクラス */
class SpriteBoxState : public ISpriteState {

public:

	SpriteBoxState() {};
	~SpriteBoxState() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Sprite* pSprite) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(Sprite* pSprite, WorldTransform worldTransform, ViewProjection viewProjection) override;


private:



};
