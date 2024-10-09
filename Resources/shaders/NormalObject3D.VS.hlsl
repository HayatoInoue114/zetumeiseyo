#include "NormalObject3d.hlsli"

struct TransformationMatrix{
    float4x4 World;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<TransformationViewMatrix> gTransformationViewMatrix : register(b1);


struct VertexShaderInput {
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 worldPosition : WORLDPOSITION0;
};


VertexShaderOutput main(VertexShaderInput input){
    VertexShaderOutput output;
    float4x4 resultMat; 

    float4x4 cameraMat = mul(gTransformationViewMatrix.view, gTransformationViewMatrix.viewProjection);
    
    resultMat = mul(gTransformationMatrix.World, cameraMat);

    output.position = mul(input.position, resultMat);

    output.worldPosition = mul(input.position, gTransformationMatrix.World).xyz;

    return output;
}