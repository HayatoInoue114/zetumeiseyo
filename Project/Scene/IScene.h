#pragma once
#include <memory>

class GameManager;

class IPlayerBullet;

/* ISceneクラス */
class IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IScene() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IScene() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(GameManager* state) = 0;

	/// <summary>
	/// 背景スプライトの描画処理
	/// </summary>
	virtual void BackSpriteDraw() = 0;

	/// <summary>
	/// ３Dオブジェクトの描画処理
	/// </summary>
	virtual void ModelDraw() = 0;

	/// <summary>
	/// 前景スプライトの描画処理
	/// </summary>
	virtual void FrontSpriteDraw() = 0;


	/// <summary>
	/// プレイヤーバレットリストの追加
	/// </summary>
	virtual void AddPlayerBullets(std::shared_ptr<IPlayerBullet> bullet) = 0;

};