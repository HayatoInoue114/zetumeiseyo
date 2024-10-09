#pragma once

#include "Tsumi.h"
#include "IScene.h"
#include "TextLog.h"
#include "DebugScene/DebugScene.h"

#include "TitleScene/TitleScene.h"
#include "SelectScene/SelectScene.h"
#include "ManualScene/ManualScene.h"
#include "GameScene/GameScene.h"
#include "ClearScene/ClearScene.h"
#include "OverScene/OverScene.h"


/* GameManagerクラス */
class GameManager {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameManager();

	/// <summary>
	/// ゲームループ
	/// </summary>
	void Run();

	/// <summary>
	/// シーンチェンジ
	/// </summary>
	void ChangeSceneState(IScene* newScene);

private:

	IScene* Scene_ = nullptr;
};