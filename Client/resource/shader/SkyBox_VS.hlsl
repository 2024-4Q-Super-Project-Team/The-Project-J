#include "Shared.fxh"

SKY_VS_OUT main(STD_VS_INPUT input)
{
    SKY_VS_OUT output;

    // 월드 변환
    float4 worldPos = mul(input.pos, WorldMatrix);
    // UV 좌표 계산 (월드 좌표 정규화)
    output.uv = worldPos.xyz; //normalize(worldPos.xyz); 
    
    // 카메라 회전 반영 (뷰 행렬의 위치 제외)
    float3 rotatedPos = mul(worldPos.xyz, (float3x3) ViewMatrix);
    // 투영 변환
    output.pos = mul(float4(rotatedPos, 1.0f), ProjectionMatrix);
    return output;
}