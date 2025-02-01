#include "Shared.fxh"

OUTLINE_VS_OUTPUT main(STD_VS_INPUT input)
{
    OUTLINE_VS_OUTPUT output;
    
    float3 outNormal = normalize(mul(input.normal, (float3x3) WorldMatrix)) * outlineOffset;
    float3 outlinePos = input.pos.xyz + outNormal;

    float4 worldPos = mul(float4(outlinePos, 1.f), WorldMatrix);
    float4 viewPos = mul(worldPos, ViewMatrix);
    output.pos = mul(viewPos, ProjectionMatrix);
   
    return output;
}