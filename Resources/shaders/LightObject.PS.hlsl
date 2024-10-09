#include "LightObject.hlsli"

struct Material {
    float4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct DirectionalLight{
    float4 color;
    float3 direction;
    float intensity;
    float shininess;
    bool enableLighting;
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<TransformationViewMatrix> gTransformationViewMatrix : register(b2);


Texture2D<float4> gTexture : register(t0);
Texture2D<float4> gTextureNormal : register(t1);
SamplerState gSampler : register(s0);

struct PixcelShaderOutput {
    float4 color : SV_TARGET0;
};


PixcelShaderOutput main(VertexShaderOutput input) {
    
    PixcelShaderOutput output;
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    if (gDirectionalLight.enableLighting != 0) {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);

        if (textureColor.a == 0.0f)
        {
            discard;
        }

        output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else {
        if (textureColor.a == 0.0f)
        {
            discard;
        }
         
        output.color = gMaterial.color * textureColor;
    }
    
    return output;
}