#pragma once
#include "MyMath.h"
#include "Struct.h"
#include "DirectXCommon.h"
#include "LineGraphicPipeline.h"
#include "CreateResource.h"
#include "DescriptorManager.h"
#include "Camera.h"


/* DrawSyetemクラス */
class DrawSystem {

public: // メンバ関数

	DrawSystem() {};
	~DrawSystem() {};

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static DrawSystem* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画処理
	/// </summary>
	static void Line(Segment segment, Camera* camera, Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f });


private: // メンバ関数

	/// <summary>
	/// コマンドコール
	/// </summary>
	void CommandCallTypeLine(Camera* camera);

private: // メンバ変数

	// リソース
	ResourcePeroperty lineResource_{};
};

