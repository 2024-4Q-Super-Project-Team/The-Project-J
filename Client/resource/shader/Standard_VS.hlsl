#include "Shared.fxh"

STD_VS_OUTPUT main(STD_VS_INPUT input)
{
    // �⺻ ���� ��������� �����Ʈ����
    matrix worldTransform = WorldMatrix;
    
    // �� ��Ű�� ������ ������ ��Ʈ�������� ������ ��Ʈ������ ����
    if (input.boneWeight[0] > 0.0f)
    {
        worldTransform  = input.boneWeight[0] * boneTransforms[input.boneID[0]];
        worldTransform += input.boneWeight[1] * boneTransforms[input.boneID[1]];
        worldTransform += input.boneWeight[2] * boneTransforms[input.boneID[2]];
        worldTransform += input.boneWeight[3] * boneTransforms[input.boneID[3]];
    }
    STD_VS_OUTPUT output;
    // ������������� ���� ������ ����
    output.pos = mul(input.pos, worldTransform);
    output.worldPos = output.pos;
    output.pos = mul(output.pos, ViewMatrix);
    output.pos = mul(output.pos, ProjectionMatrix);
    output.color = input.color;
    output.uv = input.uv;
    output.normal = normalize(mul(input.normal, (float3x3) worldTransform));
    output.tangent = normalize(mul(input.tangent, (float3x3) worldTransform));
    output.bitangent = normalize(mul(input.bitangent, (float3x3) worldTransform));

    return output;
}