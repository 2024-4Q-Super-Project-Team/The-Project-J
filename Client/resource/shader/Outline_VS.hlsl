#include "Shared.fxh"

OUTLINE_VS_OUTPUT main(STD_VS_INPUT input)
{
    OUTLINE_VS_OUTPUT output = (OUTLINE_VS_OUTPUT) 0;
    
    float3 outNormal = normalize(mul(input.normal, (float3x3) WorldMatrix));
    float4 WorldPos = mul(input.pos, WorldMatrix);
    float distToCam = length(CameraPosition - WorldPos);
    input.pos.xyz += outNormal * outlineOffset * distToCam;
    
    output.pos = mul(input.pos, WorldMatrix);
    output.pos = mul(output.pos, ViewMatrix);
    output.pos = mul(output.pos, ProjectionMatrix);
    
    return output;
}