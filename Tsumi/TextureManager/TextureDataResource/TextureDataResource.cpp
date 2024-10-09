#include "TextureDataResource.h"



// コンストラクタ
TextureDataResource::TextureDataResource(std::string filePath, TextureData textureData) {

	filePath_ = filePath;
	textureData_ = textureData;
}



// デストラクタ
TextureDataResource::~TextureDataResource() {

	textureData_.resource.Reset();
}
