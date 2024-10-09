#include "ObjDataResource.h"



// コンストラクタ
ObjDataResource::ObjDataResource(ObjData objData, uint32_t index) {

	objData_ = objData;
	index_ = index;
}



// デストラクタ
ObjDataResource::~ObjDataResource() {

	objData_.resource.Reset();
}
