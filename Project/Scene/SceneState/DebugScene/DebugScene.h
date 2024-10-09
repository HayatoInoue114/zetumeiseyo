//#pragma once
//
//#include "IScene.h"
//#include "GameManager.h"
//#include "ViewProjection.h"
//#include "EngineManual.h"
//#include "Player/Player.h"
//#include "Ground/Ground.h"
//
//
//class DebugScene : public IScene {
//
//public:
//
//	/// <summary>
//	/// コンストラクタ
//	/// </summary>
//	DebugScene() {};
//
//	/// <summary>
//	/// デストラクタ
//	/// </summary>
//	~DebugScene();
//
//	/// <summary>
//	/// 初期化処理
//	/// </summary>
//	void Initialize() override;
//
//	/// <summary>
//	/// 更新処理
//	/// </summary>
//	void Update(GameManager* state) override;
//
//	/// <summary>
//	/// 背景スプライトの描画処理
//	/// </summary>
//	void BackSpriteDraw() override;
//
//	/// <summary>
//	/// ３Dオブジェクトの描画処理
//	/// </summary>
//	void ModelDraw() override;
//
//	/// <summary>
//	/// 前景スプライトの描画処理
//	/// </summary>
//	void FrontSpriteDraw() override;
//
//	
//private:
//
//	ViewProjection viewProjection_{};
//
//	std::unique_ptr<EngineManual> engineManual_ = nullptr;
//
//	std::unique_ptr<Player> player_;
//
//	std::unique_ptr<Ground> ground_;
//};