#pragma once

#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include "MyMath.h"
#include "Struct.h"

#include <cassert>


/* ObjDataResourceクラス */
class ObjDataResource {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ObjDataResource(ObjData objData, uint32_t index);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ObjDataResource();


#pragma region Get

	/// <summary>
	/// Objデータ
	/// </summary>
	ObjData GetObjData() { return objData_; }

	/// <summary>
	/// テクスチャハンドルの取得
	/// </summary>
	uint32_t GetObjHandle() { return objData_.index; }

#pragma endregion 


private:

	ObjData objData_{};
	uint32_t index_;

};