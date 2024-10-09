#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>

#include <wrl.h>
using Microsoft::WRL::ComPtr;
using namespace std;

#include <vector>
#include <iostream>
#include <cstdint>

#include "MyMath.h"



// 頂点データ
struct VertexData {
	Vector4 position;
	Vector2 texCoord;
	Vector3 normal;
};


// スプライト構造体
struct SpriteData {
	Vector4 topLeft;
	Vector4 bottomLeft;
	Vector4 topRight;
	Vector4 bottomRight;
};


// uvTransform
struct UVTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// 球
struct SphereData {
	Vector3 center;
	float radius;
};


// マテリアル
struct Material {
	Vector4 color;
};
struct MaterialSprite {
	Vector4 color;
	Matrix4x4 uvTransform;
};
struct MaterialParticle {
	Vector4 color;
	Matrix4x4 uvTransform;
};


// マテリアルデータ
struct MaterialData {
	std::string textureFilePath;
};


// モデルデータ
struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};


// 平行光源
struct DirectionalLight {
	Vector4 color;          // ライトの色
	Vector3 direction;      // ライトの向き
	//Vector3 SpecularFColor; // 鏡面反射色
	float intensity;        // 輝度
	float sininess;         // 光沢度
	bool enableLightting;   // ライティングするか
};


// CompileShader
struct CompileShaders {
	IDxcBlobEncoding* Source;
	DxcBuffer SourceBuffer{};
	IDxcResult* Result;
	IDxcBlobUtf8* Error;
	IDxcBlob* Blob;
};


// Objデータ
struct ObjData {
	uint32_t index;
	uint32_t textureHD;
	ComPtr<ID3D12Resource> resource;
	vector<VertexData> vertices;
	MaterialData material;
};


//
//struct TransformationMatrix {
//	Matrix4x4 WVP;
//	Matrix4x4 World;
//};

//
//struct TransformationViewMatrix {
//	Matrix4x4 view;
//	Matrix4x4 viewProjection;
//	Matrix4x4 orthoGraphic;
//	Vector3 cameraPosition;
//};


struct ParticleTransformationMatrix {
	Matrix4x4 World;
	Vector4 Color;
	Matrix4x4 uvTansform;
};


enum class SpriteOrigin {
	TopLeft,
	Center,
};
