#include "SpriteObject3d.hlsli"

struct TransformationMatrix {
	float4x4 World;
};
struct TransformationViewMatrix {
    float4x4 view;
    float4x4 viewProjection;
    float4x4 orthoGraphic;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<TransformationViewMatrix> gTransformationViewMatrix : register(b1);


struct VertexShaderInput {
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	
    float4x4 resultMat = mul(gTransformationMatrix.World, gTransformationViewMatrix.orthoGraphic);

	output.position = mul(input.position, resultMat);
	output.texcoord = input.texcoord;
	return output;
}
