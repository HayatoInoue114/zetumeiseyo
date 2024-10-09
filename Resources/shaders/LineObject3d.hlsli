struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float3 worldPosition : WORLDPOSITION0;
};

struct TransformationViewMatrix
{
    float4x4 view;
    float4x4 viewProjection;
    float3 cameraPosition;
};