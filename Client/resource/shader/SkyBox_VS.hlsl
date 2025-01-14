#include "Shared.fxh"

SKY_VS_OUT main(STD_VS_INPUT input)
{
    SKY_VS_OUT output;

    // ���� ��ȯ
    float4 worldPos = mul(input.pos, WorldMatrix);
    // UV ��ǥ ��� (���� ��ǥ ����ȭ)
    output.uv = worldPos.xyz; //normalize(worldPos.xyz); 
    
    // ī�޶� ȸ�� �ݿ� (�� ����� ��ġ ����)
    float3 rotatedPos = mul(worldPos.xyz, (float3x3) ViewMatrix);
    // ���� ��ȯ
    output.pos = mul(float4(rotatedPos, 1.0f), ProjectionMatrix);
    return output;
}