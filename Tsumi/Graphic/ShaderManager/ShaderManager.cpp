#include "ShaderManager.h"



/// <summary>
/// ShaderManagerのインスタンス取得
/// </summary>
ShaderManager* ShaderManager::GetInstance() {

	static ShaderManager instance;
	return &instance;
}



/// <summary>
/// 初期化処理
/// </summary>
void ShaderManager::Initialize() {

	// DXCの初期化
	InitializeDXC();
	// includeに対応するための設定 
	SettingIncludeHandler();
	// シェーダーをコンパイルする
	ShadersCompiles();
}



/// <summary>
/// CompileShader関数
/// </summary>
IDxcBlob* ShaderManager::CompileShader(const std::wstring& filePath, const wchar_t* profile) {

	IDxcUtils* dxcUtils = ShaderManager::GetInstance()->dxc_.Utils;
	IDxcCompiler3* dxcCompiler = ShaderManager::GetInstance()->dxc_.Compiler;
	IDxcIncludeHandler* includeHandler = ShaderManager::GetInstance()->dxc_.includeHandler;
 
	/* --- 1. hlslファイルを読む --- */

	// これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	IDxcBlobEncoding* shaderSource = nullptr;
	// hlslを読み込む
	HRESULT hr_ = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読み込めなかったら止める
	assert(SUCCEEDED(hr_));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer{};
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTF8の文字コードであることを通知


	/* --- 2.Compilerする --- */

	LPCWSTR arguments[] = {
		filePath.c_str(),  // コンパイル対象のhlslファイル名
		L"-E", L"main",  // エントリーポイントの指定。基本的にmain以外にはしない
		L"-T", profile,  // ShaderProfileの設定
		L"-Zi", L"-Qembed_debug",  // デバッグ用の情報を埋め込む
		L"-Od",  // 最適化を外しておく
		L"-Zpr",  // メモリレイアウトは行優先
	};
	// 実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr_ = dxcCompiler->Compile(
		&shaderSourceBuffer,        // 読み込んだファイル
		arguments,					// コンパイルオプション
		_countof(arguments),		// コンパイルオプションの数
		includeHandler,				// includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)	// コンパイル結果
	);
	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr_));


	/* --- 3. 警告・エラーが出てないか確認する --- */

	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		// 警告・エラーゼッタイ
		Log(shaderError->GetStringPointer());
		assert(false);
	}


	/* --- 4. Compiler結果を受け取って返す --- */

	// コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr_ = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr_));
	// 成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeded, path{}, profile:{}\n", filePath, profile)));
	// もう使わないリソースを解放
	shaderResult->Release();
	shaderSource->Release();
	// 実行用のバイナリを返却
	return shaderBlob;
}



/// <summary>
/// DXCの初期化
/// </summary>
void ShaderManager::InitializeDXC() {

	HRESULT hr{};

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&ShaderManager::GetInstance()->dxc_.Utils));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&ShaderManager::GetInstance()->dxc_.Compiler));
	assert(SUCCEEDED(hr));
}



/// <summary>
/// includeに対応するための設定
/// </summary>
void ShaderManager::SettingIncludeHandler() {

	ShaderManager::GetInstance()->dxc_.Utils->CreateDefaultIncludeHandler(&ShaderManager::GetInstance()->dxc_.includeHandler);
}



/// <summary>
/// シェーダーをコンパイルする
/// </summary>
void ShaderManager::ShadersCompiles() {
	
	NormalShader();
	SpriteShader();
	LightShader();
	LambertShader();
	PhongShader();
	PhongNormalMapShader();
	ParticleShader();
	LineShader();
}



/// <summary>
/// 
/// </summary>
void ShaderManager::NormalShader() {

	ShadersMode shader{};

	shader.VertexBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/NormalObject3d.VS.hlsl", L"vs_6_0");
	shader.PixelBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/NormalObject3d.PS.hlsl", L"ps_6_0");

	ShaderManager::GetInstance()->shaders_.Noraml = shader;
}


/// <summary>
/// 
/// </summary>
void ShaderManager::SpriteShader() {

	ShadersMode shader{};

	shader.VertexBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/SpriteObject3d.VS.hlsl", L"vs_6_0");
	shader.PixelBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/SpriteObject3d.PS.hlsl", L"ps_6_0");

	ShaderManager::GetInstance()->shaders_.Sprite = shader;
}


/// <summary>
/// 
/// </summary>
void ShaderManager::LightShader() {

	ShadersMode shader{};

	shader.VertexBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/LightObject.VS.hlsl", L"vs_6_0");
	shader.PixelBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/LightObject.PS.hlsl", L"ps_6_0");

	ShaderManager::GetInstance()->shaders_.Light = shader;
}


/// <summary>
/// 
/// </summary>
void ShaderManager::LambertShader()
{
	ShadersMode shader{};

	shader.VertexBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/LambertObject.VS.hlsl", L"vs_6_0");
	shader.PixelBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/LambertObject.PS.hlsl", L"ps_6_0");

	ShaderManager::GetInstance()->shaders_.Lambert = shader;
}


/// <summary>
/// 
/// </summary>
void ShaderManager::PhongShader()
{
	ShadersMode shader{};

	shader.VertexBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/PhongObject.VS.hlsl", L"vs_6_0");
	shader.PixelBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/PhongObject.PS.hlsl", L"ps_6_0");

	ShaderManager::GetInstance()->shaders_.Phong = shader;
}


/// <summary>
/// 
/// </summary>
void ShaderManager::PhongNormalMapShader()
{
	ShadersMode shader{};

	shader.VertexBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/PhongNormalMap.VS.hlsl", L"vs_6_0");
	shader.PixelBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/PhongNormalMap.PS.hlsl", L"ps_6_0");

	ShaderManager::GetInstance()->shaders_.PhongNormalMap = shader;
}


/// <summary>
/// 
/// </summary>
void ShaderManager::ParticleShader() {

	ShadersMode shader{};

	shader.VertexBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/ParticleObject3D.VS.hlsl", L"vs_6_0");
	shader.PixelBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/ParticleObject3D.PS.hlsl", L"ps_6_0");

	ShaderManager::GetInstance()->shaders_.Particle = shader;
}


/// <summary>
/// 
/// </summary>
void ShaderManager::LineShader() {

	ShadersMode shader{};

	shader.VertexBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/LineObject3d.VS.hlsl", L"vs_6_0");
	shader.PixelBlob =
		ShaderManager::CompileShader(
			L"Resources/shaders/LineObject3d.PS.hlsl", L"ps_6_0");

	ShaderManager::GetInstance()->shaders_.Line = shader;

}
