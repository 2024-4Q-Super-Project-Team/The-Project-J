#include "Shared.fxh"

float4 main(QUAD_VS_OUTPUT input) : SV_Target
{
    float4 color = particleTexture.Sample(LinearWrapSampler, input.uv);

    // ���� �� ó�� (��: ������ ������ �ȼ� ����)
    if (color.a < 0.1)
        discard;

    return color;
}