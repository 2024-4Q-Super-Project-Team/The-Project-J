#include "Shared.fxh"

float4 main(QUAD_VS_OUTPUT output) : SV_TARGET
{
     // UV 보정: (0~1 범위의 기본 UV를 타일 UV 영역으로 변환)
    float2 newUV = lerp(uvMin, uvMax, output.uv);
    // 텍스처 샘플링
    return SpriteTexture.Sample(LinearWrapSampler, newUV);
}