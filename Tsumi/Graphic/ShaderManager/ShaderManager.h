#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <string>
#include <wrl.h>
#include <format>

#include "MyMath.h"



struct Shaders {
	IDxcUtils* Utils;
	IDxcCompiler3* Compiler;
	IDxcIncludeHandler* includeHandler;
};


struct ShadersMode {
	IDxcBlob* VertexBlob;
	IDxcBlob* PixelBlob;
};


struct ShadersType {

	ShadersMode Noraml;
	ShadersMode Sprite;
	ShadersMode Light;
	ShadersMode Lambert;
	ShadersMode Phong;
	ShadersMode PhongNormalMap;
	ShadersMode Particle;
	ShadersMode Line;
};



class ShaderManager {

public: // メンバ関数

	/// <summary>
	/// ShaderManagerのインスタンス取得
	/// </summary>
	static ShaderManager* GetInstance();

	
	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();


#pragma region Get 取得

	/// <summary>
	/// 
	/// </summary>
	ShadersType GetShaderType() { return ShaderManager::GetInstance()->shaders_; }

#pragma endregion


private: // メンバ関数

	/// <summary>
	/// CompileShader関数
	/// </summary>
	static IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile);


	/// <summary>
	/// DXCの初期化
	/// </summary>
	static void InitializeDXC();


	/// <summary>
	/// includeに対応するための設定
	/// </summary>
	static void SettingIncludeHandler();


	/// <summary>
	/// シェーダーをコンパイルする
	/// </summary>
	static void ShadersCompiles();



#pragma region Shaders

	/// <summary>
	/// 
	/// </summary>
	static void NormalShader();

	/// <summary>
	/// 
	/// </summary>
	static void SpriteShader();

	/// <summary>
	/// 
	/// </summary>
	static void LightShader();

	/// <summary>
	/// 
	/// </summary>
	static void LambertShader();

	/// <summary>
	/// 
	/// </summary>
	static void PhongShader();

	/// <summary>
	/// 
	/// </summary>
	static void PhongNormalMapShader();

	/// <summary>
	/// 
	/// </summary>
	static void ParticleShader();

	/// <summary>
	/// 
	/// </summary>
	static void LineShader();

#pragma endregion


private: // メンバ変数

	Shaders dxc_;
	ShadersType shaders_;

};