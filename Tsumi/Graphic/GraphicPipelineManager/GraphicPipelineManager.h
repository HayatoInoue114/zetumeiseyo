#pragma once

#include <d3dx12.h>
#include <dxcapi.h>

#include "MyMath.h"
#include "Struct.h"
#include "DirectXCommon.h"
#include "ShaderManager.h"
#include "NormalGraphicPipeline.h"
#include "SpriteGraphicPipeline.h"
#include "LightGraphicPipeline.h"
#include "LambertGraphicPipeline.h"
#include "PhongGraphicPipeline.h"
#include "PhongNormalMap.h"
#include "ParticleGraphicPipeline.h"
#include "LineGraphicPipeline.h"


class GraphicPipelineManager {

public: // メンバ関数

	/// <summary>
	/// PSOクラスのインスタンス取得
	/// </summary>
	static GraphicPipelineManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();


private: // メンバ変数


};