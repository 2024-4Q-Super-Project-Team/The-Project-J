#include "Shared.fxh"

struct SHADOW_VS_OUT
{
    float4 pos : SV_POSITION;
};

SHADOW_VS_OUT main(STD_VS_INPUT input)
{
    matrix worldTransform = worldMatrix;
    // �� ��Ű�� ������ ������ ��Ʈ�������� ������ ��Ʈ������ ����
    if (input.boneWeight[0] > 0.0f)
    {
        worldTransform  = input.boneWeight[0] * boneTransforms[input.boneID[0]];
        worldTransform += input.boneWeight[1] * boneTransforms[input.boneID[1]];
        worldTransform += input.boneWeight[2] * boneTransforms[input.boneID[2]];
        worldTransform += input.boneWeight[3] * boneTransforms[input.boneID[3]];
    }
    SHADOW_VS_OUT output;
    output.pos = mul(input.pos, worldTransform);
    output.pos = mul(output.pos, viewMatrix);
    output.pos = mul(output.pos, projectionMatrix);
    return output;
}