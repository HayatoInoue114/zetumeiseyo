#include "ModelManager.h"



/// <summary>
/// インスタンスの取得
/// </summary>
ModelManager* ModelManager::Getinstance() {
	static ModelManager instance;
	return &instance;
}


/// <summary>
/// 初期化処理
/// </summary>
void ModelManager::Initialize() {}


/// <summary>
/// 解放処理
/// </summary>
void ModelManager::Finalize() {

	ModelManager::Getinstance()->objModelDatas_.clear();
}


/// <summary>
/// Objファイルを読み込む
/// </summary>
ObjData ModelManager::LoadObjFile(std::string filePath, const std::string& routeFilePath) {

	if (CheckObjData(filePath)) {

		// 初めてだったら加算
		ModelManager::Getinstance()->objHandle_++;
		uint32_t modelHandle = ModelManager::Getinstance()->objHandle_;


		/* 1. 中で必要となる変数の宣言 */

		ObjData objData;            // 構築するModelData
		std::vector<Vector4> positions; // 位置
		std::vector<Vector3> normals;   // 法線
		std::vector<Vector2> texcoords; // テクスチャ座標
		std::string line;               // ファイルから読んだ１行をを格納するもの



		/* 2. ファイルを開く */

		// ファイルを開く
		std::ifstream file("Resources/Obj/" + routeFilePath + "/" + filePath + "/" + filePath + ".obj");
		// とりあえず開けなかったら止める
		assert(file.is_open());



		/* 3. 実際にファイルを読み、ModelDataを構築していく */

		while (std::getline(file, line)) {

			std::string identifier;
			std::istringstream s(line);

			// 先頭の識別子を読む
			s >> identifier;


			// identifierに応じた処理
			// "v"  = 頂点位置
			// "vt" = 頂点テクスチャ座標 
			// "vn" = 頂点法線
			// "f"  = 面

			if (identifier == "v") {

				Vector4 position{};
				s >> position.x >> position.y >> position.z;
				position.x *= -1.0f;
				position.w = 1.0f;
				positions.push_back(position);
			}
			else if (identifier == "vt") {

				Vector2 texcoord{};
				s >> texcoord.x >> texcoord.y;
				texcoord.y *= -1.0f;
				texcoords.push_back(texcoord);
			}
			else if (identifier == "vn") {

				Vector3 normal{};
				s >> normal.x >> normal.y >> normal.z;
				normal.x *= -1.0f;
				normals.push_back(normal);
			}
			else if (identifier == "f") {

				VertexData triangle[3]{};

				// 面は三角形限定。その他は未対応
				for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {

					std::string vertexDefinition;
					s >> vertexDefinition;

					// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
					std::istringstream v(vertexDefinition);
					uint32_t elementIndex[3]{};

					for (int32_t element = 0; element < 3; ++element) {

						std::string index;
						std::getline(v, index, '/'); // "/"区切りでインデックスを読んでいく

						assert((!index.empty()) && "/ / の間に値が入ってない");
						elementIndex[element] = std::stoi(index);
						
					}

					// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
					Vector4 position = positions[static_cast<std::vector<Vector4, std::allocator<Vector4>>::size_type>(elementIndex[0]) - 1];
					Vector2 texcoord = texcoords[static_cast<std::vector<Vector2, std::allocator<Vector2>>::size_type>(elementIndex[1]) - 1];
					Vector3 normal = normals[static_cast<std::vector<Vector3, std::allocator<Vector3>>::size_type>(elementIndex[2]) - 1];
					triangle[faceVertex] = { position, texcoord, normal };
				}
				// 頂点を逆順で登録することで、回利潤を逆にする
				objData.vertices.push_back(triangle[2]);
				objData.vertices.push_back(triangle[1]);
				objData.vertices.push_back(triangle[0]);
			}
			else if (identifier == "mtllib") {

				// materialTemplateLibraryファイルの名前を取得する
				std::string materialFileName;
				s >> materialFileName;

				// 基本的にobjファイルを同じ階層にmtlは存在させるので、ディレクトリファイル名を渡す
				objData.material = ModelManager::Getinstance()->LoadMaterialTemplateFile(filePath, materialFileName, routeFilePath);
			}
		}
		// テクスチャを指定されたものにする
		uint32_t texHandle = TextureManager::LoadTexture(routeFilePath, filePath, true);
		objData.textureHD = texHandle;
		ModelManager::Getinstance()->objModelDatas_[filePath] = make_unique<ObjDataResource>(objData, modelHandle);
		

		/* 4. ModelHandleを返す */
		//return ModelManager::Getinstance()->objModelDatas_[filePath].get()->GetObjData();
	}

	return ModelManager::Getinstance()->objModelDatas_[filePath].get()->GetObjData();
}


/// <summary>
/// mtlファイルを読み込む関数
/// </summary>
MaterialData ModelManager::LoadMaterialTemplateFile(const std::string& filePath, const std::string& fileName, const std::string& routeFilePath) {

	/* 1. 中で必要となる変数の宣言 */

	MaterialData materialData{}; // 構築するMaterialData
	std::string line{};			 // ファイルから読んだ１行を格納するもの



	/* 2. ファイルを開く */

	// ファイルを開く
	std::ifstream file("Resources/Obj/" + routeFilePath + "/" + filePath + "/" + fileName);

	//とりあえず開けなかったら止める
	assert(file.is_open());



	/* 3. 実際にファイルを読み、MaterualDataを構築していく */

	while (std::getline(file, line)) {

		std::string identifier{};
		std::istringstream s(line);
		s >> identifier;


		// identifierに応じた処理
		// "map_Kd" = textureのファイル名が記載されている

		if (identifier == "map_Kd") {

			std::string textureFileName{};
			s >> textureFileName;

			// 連結してファイルパスにする
			materialData.textureFilePath = "Resources/Obj/" + filePath + "/" + textureFileName;
		}
	}



	/* 4. MaterialData を返す */
	return materialData;

}


/// <summary>
/// 一回読み込んだものは読み込まない
/// </summary>
bool ModelManager::CheckObjData(std::string filePath) {

	if (ModelManager::Getinstance()->objModelDatas_.find(filePath) == ModelManager::Getinstance()->objModelDatas_.end()) {

		return true;
	}

	return false;
}
