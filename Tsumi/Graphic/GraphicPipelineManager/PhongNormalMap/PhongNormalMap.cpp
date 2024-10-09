#include "PhongNormalMap.h"



// PhongNormalMapクラスのインスタンス取得
PhongNormalMap* PhongNormalMap::GetInstance() {
	static PhongNormalMap instance;
	return &instance;
}



// 初期化処理
void PhongNormalMap::Initialize() {}



// PSOを構築する
void PhongNormalMap::SetupLightPso() {

	PhongNormalMap* phongNormalMapGraphicPipeline = PhongNormalMap::GetInstance();


	/* --- RootSignatureを作成 --- */
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	phongNormalMapGraphicPipeline->SetupRootSignature(descriptionRootSignature);


	/* --- InputLayoutを設定する --- */
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[5]{};
	phongNormalMapGraphicPipeline->SetupInputElementDescs(inputElementDescs[0], "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D12_APPEND_ALIGNED_ELEMENT);
	phongNormalMapGraphicPipeline->SetupInputElementDescs(inputElementDescs[1], "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, D3D12_APPEND_ALIGNED_ELEMENT);
	phongNormalMapGraphicPipeline->SetupInputElementDescs(inputElementDescs[2], "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, D3D12_APPEND_ALIGNED_ELEMENT);
	phongNormalMapGraphicPipeline->SetupInputElementDescs(inputElementDescs[3], "WORLDPOSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, D3D12_APPEND_ALIGNED_ELEMENT);
	phongNormalMapGraphicPipeline->SetupInputElementDescs(inputElementDescs[4], "CAMERAPOSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, D3D12_APPEND_ALIGNED_ELEMENT);

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	phongNormalMapGraphicPipeline->SetupInputLayout(inputLayoutDesc, inputElementDescs, _countof(inputElementDescs));


	/* --- BlendStateを設定する --- */
	// 全ての色要素を書き込む
	D3D12_BLEND_DESC blendDesc{};
	D3D12_RENDER_TARGET_BLEND_DESC& pBlendDesc = blendDesc.RenderTarget[0];
	phongNormalMapGraphicPipeline->SetupBlendState(pBlendDesc, BlendNone);


	/* --- RasiterzerStateを設定する --- */
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	phongNormalMapGraphicPipeline->SetupRasterizerState(rasterizerDesc);


	/* --- Shaderをcompileする --- */
	// Shaderをコンパイルする
	IDxcBlob* vertexShaderBlob = nullptr;
	IDxcBlob* pixelShaderBlob = nullptr;
	phongNormalMapGraphicPipeline->CompileShaders(vertexShaderBlob, pixelShaderBlob);


	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = PhongNormalMap::GetInstance()->phongNormalMapPso_.rootSignature; // RootSignature

	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout

	graphicsPipelineStateDesc.BlendState = blendDesc; // BlendState

	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizeState

	graphicsPipelineStateDesc.VS = {
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize() }; // VertexShader

	graphicsPipelineStateDesc.PS = {
		pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize() }; // PixelShader


	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;


	// 利用するトポロジ(形状)もタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


	// どのように画面に色を打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込む
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


	// 実際に生成
	HRESULT hr{};
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
		&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&PhongNormalMap::GetInstance()->phongNormalMapPso_.graphicsPipelineState));
	assert(SUCCEEDED(hr));
}



// RootSignatureのセットアップ
void PhongNormalMap::SetupRootSignature(D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature) {

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


	// 色に関する
	D3D12_ROOT_PARAMETER rootParameters[6]{};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号0とバインド


	// 頂点位置に関する
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;// VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;// レジスタ番号を0にバインド

	descriptionRootSignature.pParameters = rootParameters; // ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters); // 配列の長さ


	// Viewに関する
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[2].Descriptor.ShaderRegister = 1;


	// テクスチャに関する
	D3D12_DESCRIPTOR_RANGE descriptorRange[1]{};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // offsetを自動計算

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixShaderで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する


	// 光に関する
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[4].Descriptor.ShaderRegister = 1; // レジスタ番号0とバインド


	// テクスチャに関する
	D3D12_DESCRIPTOR_RANGE descriptorRangeA[1]{};
	descriptorRangeA[0].BaseShaderRegister = 1; // 0から始まる
	descriptorRangeA[0].NumDescriptors = 1; // 数は1つ
	descriptorRangeA[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // offsetを自動計算

	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixShaderで使う
	rootParameters[5].DescriptorTable.pDescriptorRanges = descriptorRangeA; // Tableの中身の配列を指定
	rootParameters[5].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeA); // Tableで利用する


	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1]{};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う

	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// シリアライズしてバイナリにする
	HRESULT hr_ = D3D12SerializeRootSignature(
		&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&PhongNormalMap::GetInstance()->phongNormalMapPso_.signatureBlob,
		&PhongNormalMap::GetInstance()->phongNormalMapPso_.errorBlob);
	if (FAILED(hr_)) {
		Log(reinterpret_cast<char*>(PhongNormalMap::GetInstance()->phongNormalMapPso_.errorBlob->GetBufferPointer()));
		assert(false);
	}


	// バイナリを元に生成
	hr_ = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(
		0,
		PhongNormalMap::GetInstance()->phongNormalMapPso_.signatureBlob->GetBufferPointer(),
		PhongNormalMap::GetInstance()->phongNormalMapPso_.signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&PhongNormalMap::GetInstance()->phongNormalMapPso_.rootSignature));
	assert(SUCCEEDED(hr_));
}


// InputLayoutのセットアップ
void PhongNormalMap::SetupInputElementDescs(D3D12_INPUT_ELEMENT_DESC& inputElementDescs, LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format, UINT AlignedByteOffset) {

	inputElementDescs.SemanticName = SemanticName;
	inputElementDescs.SemanticIndex = SemanticIndex;
	inputElementDescs.Format = Format;
	inputElementDescs.AlignedByteOffset = AlignedByteOffset;
}
void PhongNormalMap::SetupInputLayout(D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements) {

	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = numInputElements;
}


// BlendStateのセットアップ
void PhongNormalMap::SetupBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode) {

	blendDesc.RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.BlendEnable = true;


	switch (blendMode)
	{
	case BlendNone:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;

	case BlendAdd:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendSubtruct:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case BlendMultiply:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_ZERO;
		blendDesc.DestBlend = D3D12_BLEND_SRC_COLOR;
		break;

	case BlendScreen:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;

	default:
		break;
	}
}


// RasterizerStateのセットアップ
void PhongNormalMap::SetupRasterizerState(D3D12_RASTERIZER_DESC& rasterizerDesc) {

	// 裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
}


// Shadersのコンパイル
void PhongNormalMap::CompileShaders(IDxcBlob*& vertexShaderBlob, IDxcBlob*& pixelShaderBlob) {

	vertexShaderBlob = ShaderManager::GetInstance()->GetShaderType().PhongNormalMap.VertexBlob;
	assert(vertexShaderBlob != nullptr);

	pixelShaderBlob = ShaderManager::GetInstance()->GetShaderType().PhongNormalMap.PixelBlob;
	assert(pixelShaderBlob != nullptr);
}