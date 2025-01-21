#include "Shared.fxh"

float4 main(QUAD_VS_OUTPUT input) : SV_Target
{
    float4 color = particleTexture.Sample(LinearWrapSampler, input.uv);

    // 알파 값 처리 (예: 완전히 투명한 픽셀 제외)
    if (color.a < 0.1)
        discard;

    return color;
}