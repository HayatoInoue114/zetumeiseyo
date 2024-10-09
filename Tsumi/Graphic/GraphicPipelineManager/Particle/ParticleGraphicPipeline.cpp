#include "ParticleGraphicPipeline.h"



// ParticleGraphicPipelineクラスのインスタンス取得
ParticleGraphicPipeline* ParticleGraphicPipeline::GetInstance() {
	static ParticleGraphicPipeline instance;
	return &instance;
}



// 初期化処理
void ParticleGraphicPipeline::Initialize() {}



// PSOを構築する
void ParticleGraphicPipeline::SetupParticlePso() {

	ParticleGraphicPipeline* particleGraphicPipeline = ParticleGraphicPipeline::GetInstance();


	/* --- RootSignatureを作成 --- */
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	particleGraphicPipeline->SetupRootSignature(descriptionRootSignature);


	/* --- InputLayoutを設定する --- */
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3]{};
	particleGraphicPipeline->SetupInputElementDescs(inputElementDescs[0], "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D12_APPEND_ALIGNED_ELEMENT);
	particleGraphicPipeline->SetupInputElementDescs(inputElementDescs[1], "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, D3D12_APPEND_ALIGNED_ELEMENT);
	particleGraphicPipeline->SetupInputElementDescs(inputElementDescs[2], "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D12_APPEND_ALIGNED_ELEMENT);

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	particleGraphicPipeline->SetupInputLayout(inputLayoutDesc, inputElementDescs, _countof(inputElementDescs));


	/* --- BlendStateを設定する --- */
	// 全ての色要素を書き込む
	D3D12_BLEND_DESC blendDesc{};
	D3D12_RENDER_TARGET_BLEND_DESC& pBlendDesc = blendDesc.RenderTarget[0];
	particleGraphicPipeline->SetupBlendState(pBlendDesc, BlendNone);


	/* --- RasiterzerStateを設定する --- */
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	particleGraphicPipeline->SetupRasterizerState(rasterizerDesc);


	/* --- Shaderをcompileする --- */
	// Shaderをコンパイルする
	IDxcBlob* vertexShaderBlob = nullptr;
	IDxcBlob* pixelShaderBlob = nullptr;
	particleGraphicPipeline->CompileShaders(vertexShaderBlob, pixelShaderBlob);


	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = ParticleGraphicPipeline::GetInstance()->particlePso_.rootSignature; // RootSignature

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
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


	// 実際に生成
	HRESULT hr{};
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
		&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&ParticleGraphicPipeline::GetInstance()->particlePso_.graphicsPipelineState));
	assert(SUCCEEDED(hr));
}



// RootSignatureのセットアップ
void ParticleGraphicPipeline::SetupRootSignature(D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature) {

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


	// Instancing
	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	// PixelDescriptorRanged
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	// 色に関する
	D3D12_ROOT_PARAMETER rootParameters[3]{};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号0とバインド


	// 頂点位置に関する
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;// VertexShaderで使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);


	// テクスチャに関する
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // VertexShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する


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


	descriptionRootSignature.pParameters = rootParameters; // ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters); // 配列の長さ

	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// シリアライズしてバイナリにする
	HRESULT hr_ = D3D12SerializeRootSignature(
		&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&ParticleGraphicPipeline::GetInstance()->particlePso_.signatureBlob,
		&ParticleGraphicPipeline::GetInstance()->particlePso_.errorBlob);
	if (FAILED(hr_)) {
		Log(reinterpret_cast<char*>(ParticleGraphicPipeline::GetInstance()->particlePso_.errorBlob->GetBufferPointer()));
		assert(false);
	}


	// バイナリを元に生成
	hr_ = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(
		0,
		ParticleGraphicPipeline::GetInstance()->particlePso_.signatureBlob->GetBufferPointer(),
		ParticleGraphicPipeline::GetInstance()->particlePso_.signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&ParticleGraphicPipeline::GetInstance()->particlePso_.rootSignature));
	assert(SUCCEEDED(hr_));
}


// InputLayoutのセットアップ
void ParticleGraphicPipeline::SetupInputElementDescs(D3D12_INPUT_ELEMENT_DESC& inputElementDescs, LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format, UINT AlignedByteOffset) {

	inputElementDescs.SemanticName = SemanticName;
	inputElementDescs.SemanticIndex = SemanticIndex;
	inputElementDescs.Format = Format;
	inputElementDescs.AlignedByteOffset = AlignedByteOffset;
}
void ParticleGraphicPipeline::SetupInputLayout(D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements) {

	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = numInputElements;
}


// BlendStateのセットアップ
void ParticleGraphicPipeline::SetupBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode) {

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
void ParticleGraphicPipeline::SetupRasterizerState(D3D12_RASTERIZER_DESC& rasterizerDesc) {

	// 裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
}


// Shadersのコンパイル
void ParticleGraphicPipeline::CompileShaders(IDxcBlob*& vertexShaderBlob, IDxcBlob*& pixelShaderBlob) {

	vertexShaderBlob = ShaderManager::GetInstance()->GetShaderType().Particle.VertexBlob;
	assert(vertexShaderBlob != nullptr);

	pixelShaderBlob = ShaderManager::GetInstance()->GetShaderType().Particle.PixelBlob;
	assert(pixelShaderBlob != nullptr);
}



