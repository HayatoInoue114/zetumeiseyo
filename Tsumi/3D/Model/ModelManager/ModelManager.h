#pragma once

#include "MyMath.h"
#include "Struct.h"
#include "TextureManager.h"
#include "ObjDataResource.h"
#include <map>
#include <fstream>
#include <sstream>

/* ModelManagerクラス */
class ModelManager {

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModelManager() {};
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ModelManager() {};

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static ModelManager* Getinstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// Objファイルを読み込む
	/// </summary>
	static ObjData LoadObjFile(std::string filePath, const std::string& routeFilePath);


private: // メンバ関数

	/// <summary>
	/// 一回読み込んだものは読み込まない
	/// </summary>
	static bool CheckObjData(std::string filePath);

	/// <summary>
	/// mtlファイルを読み込む関数
	/// </summary>
	MaterialData LoadMaterialTemplateFile(const std::string& filePath, const std::string& fileName, const std::string& routeFilePath);


private: // メンバ変数

	// ObjDataResource
	map<std::string, unique_ptr<ObjDataResource>> objModelDatas_;

	uint32_t objHandle_ = 0;

	ModelData modelData_{};
};

