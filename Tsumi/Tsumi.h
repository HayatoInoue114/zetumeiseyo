#pragma once

#include "WinApp.h"
#include "DirectXCommon.h"
#include "ShaderManager.h"
#include "GraphicPipelineManager.h"
#include "ImGuimanager.h"
#include "TextureManager.h"
#include "Input.h"
#include "Audio.h"
#include "DescriptorManager.h"
#include "DrawSystem.h"
#include "ParticleManager.h"



/// Tsumi Engine 
/// 努力の「積」み重ねのエンジンになるか
/// ゴミ性能の「罪」なエンジンになるか
/// 自分の努力次第。頑張れ


/* Tsumiクラス */
class Tsumi {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Tsumi() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Tsumi() {};

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static Tsumi* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// フレームの開始
	/// </summary>
	static void BeginFlame();

	/// <summary>
	/// フレームの終了
	/// </summary>
	static void EndFlame();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	static bool ProcessMessage();

private:


};