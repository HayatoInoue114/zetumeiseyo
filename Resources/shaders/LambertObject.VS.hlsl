#include "LambertObject.hlsli"


struct TransformationMatrix
{
    float4x4 World;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<TransformationViewMatrix> gTransformationViewMatrix : register(b1);


struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : WORLDPOSITION0;
    float3 cameraPosition : CAMERAPOSITION0;
};


VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    float4x4 resultMat;

    float4x4 cameraMat = mul(gTransformationViewMatrix.view, gTransformationViewMatrix.viewProjection);
    

    resultMat = mul(gTransformationMatrix.World, cameraMat);


    output.position = mul(input.position, resultMat);
    output.texcoord = input.texcoord;

    output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrix.World));
    output.worldPosition = mul(input.position, gTransformationMatrix.World).xyz;
    
    output.cameraPosition = gTransformationViewMatrix.cameraPosition;
    
    return output;
}