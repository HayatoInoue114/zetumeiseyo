#include "ParticleObject3D.hlsli"

struct Material
{
    float4 color;
    float4x4 uvTransform;
};
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);


struct PixcelShaderOutput
{
    float4 color : SV_TARGET0;
};


PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;
    float4 transUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    if (textureColor.a == 0.0f)
    {
        discard;
    }
    output.color.rgb = gMaterial.color.rgb * textureColor.rgb * input.color.rgb;
    output.color.a = gMaterial.color.a * textureColor.a * input.color.a;

    return output;
}