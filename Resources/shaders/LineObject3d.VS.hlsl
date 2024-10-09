#include "LineObject3d.hlsli"

struct TransformationMatrix
{
    float4x4 World;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<TransformationViewMatrix> gTransformationViewMatrix : register(b1);


struct VertexShaderInput
{
    float4 position : POSITION0;
};


VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    float4x4 cameraMat = mul(gTransformationViewMatrix.view, gTransformationViewMatrix.viewProjection);
    float4x4 resultMat = mul(gTransformationMatrix.World, cameraMat);

    output.position = mul(input.position, resultMat);

    return output;
}