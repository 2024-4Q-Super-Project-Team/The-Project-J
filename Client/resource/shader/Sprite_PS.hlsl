#include "Shared.fxh"

float4 main(QUAD_VS_OUTPUT output) : SV_TARGET
{
     // UV ����: (0~1 ������ �⺻ UV�� Ÿ�� UV �������� ��ȯ)
    float2 newUV = lerp(uvMin, uvMax, output.uv);
    // �ؽ�ó ���ø�
    return SpriteTexture.Sample(LinearWrapSampler, newUV);
}