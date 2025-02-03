#include "Shared.fxh"

OUTLINE_VS_OUTPUT main(STD_VS_INPUT input)
{
    OUTLINE_VS_OUTPUT output;
    
    float3 outNormal = normalize(mul(input.normal, (float3x3) WorldMatrix));
    
    float4 worldPos = mul(float4(input.pos.xyz, 1.f), WorldMatrix);
    
    float distToCam = length(CameraPosition.xyz - worldPos.xyz);
    
    worldPos.xyz += outNormal * outlineOffset * 2.f * distToCam;
    
    float4 viewPos = mul(worldPos, ViewMatrix);
    output.pos = mul(viewPos, ProjectionMatrix);
    
    return output;
}