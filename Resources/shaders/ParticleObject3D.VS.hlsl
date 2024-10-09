#include "ParticleObject3D.hlsli"

struct TransformationMatrix
{
    float4x4 World;
    float4 color;
    float4x4 uvTransform;
};
StructuredBuffer<TransformationMatrix> gTransformationMatrix : register(t0);


struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR0;
};


VertexShaderOutput main(VertexShaderInput input, uint instanceID : SV_InstanceID)
{
    VertexShaderOutput output;
    float4x4 resultMat;
    
    output.position = mul(input.position, gTransformationMatrix[instanceID].World);
    
    float4 texPos;
    texPos.rg = input.texcoord.rg;
    texPos.b = 0;
    texPos.a = 1;
    texPos = mul(texPos, gTransformationMatrix[instanceID].uvTransform);
    output.texcoord = texPos.rg;
    
    output.color = gTransformationMatrix[instanceID].color;
    
    return output;
}