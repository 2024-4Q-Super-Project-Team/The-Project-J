#include "Shared.fxh"

float4 main(QUAD_VS_OUTPUT output) : SV_TARGET
{
    return SpriteTexture.Sample(LinearWrapSampler, output.uv);
}