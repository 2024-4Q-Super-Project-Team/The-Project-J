#include "Shared.fxh"

float4 main(SKY_VS_OUT input) : SV_TARGET
{
    //float4 FinalColor = IBLEnvironmentMap.SampleLevel(LinearSampler, input.uv, 1);
    float4 FinalColor = IBLEnvironmentMap.Sample(LinearWrapSampler, input.uv);
    
    return FinalColor;
}